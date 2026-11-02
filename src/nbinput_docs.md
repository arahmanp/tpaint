# nbinput — Non-blocking keyboard input for C++ CLI programs

`nbinput.hpp` is a tiny, header-only C++17 library that lets a console program read keystrokes **without blocking**. Your main loop keeps running (rendering, game logic, network polling, animations) and picks up keys as they arrive.

- **Header-only** — drop one file into your project.
- **Cross-platform** — Linux, macOS, and Windows.
- **Zero dependencies** — only the C++ standard library and the OS's own console APIs (`termios`/`select` on POSIX, `conio.h` on Windows).
- **RAII** — the terminal is put into the right mode when you create the object and restored when it is destroyed.
- **Normalized keys** — Enter, Backspace, and arrow keys have the same codes on every platform.
- **Scope** — plain ASCII characters plus arrow keys, Home, End, and Delete. No Unicode, no mouse, no function keys.

---

## Table of contents

1. [Quick start](#1-quick-start)
2. [Requirements and building](#2-requirements-and-building)
3. [Core concepts](#3-core-concepts)
4. [API reference](#4-api-reference)
5. [Key codes](#5-key-codes)
6. [Usage recipes](#6-usage-recipes)
7. [Platform behavior](#7-platform-behavior)
8. [Error handling](#8-error-handling)
9. [Limitations and caveats](#9-limitations-and-caveats)
10. [Troubleshooting / FAQ](#10-troubleshooting--faq)
11. [Design notes](#11-design-notes)

---

## 1. Quick start

Copy `nbinput.hpp` into your project and include it:

```cpp
#include "nbinput.hpp"

int main() {
    nbi::Input in;                          // enables non-blocking input

    for (bool running = true; running;) {
        int k = in.poll();                  // returns immediately
        if (k == 'q') running = false;
        if (k == nbi::Key::Up) { /* ... */ }

        // ... do other work here; the loop is never blocked by input ...
    }
}   // `in` goes out of scope: terminal is restored
```

Build:

```sh
g++ -std=c++17 main.cpp -o main
```

A complete runnable demo is in `example.cpp`.

---

## 2. Requirements and building

| Item | Requirement |
|------|-------------|
| Language standard | C++17 or newer (uses `inline constexpr` variables) |
| Compilers | GCC, Clang, MSVC, MinGW (only GCC on Linux was actually compiled) |
| Linux / macOS | A POSIX terminal. Links against nothing extra |
| Windows | Windows console (cmd.exe, PowerShell, Windows Terminal). Links against nothing extra |
| Third-party libraries | None |

> The library was compile-checked on Linux with `g++ -std=c++17 -Wall -Wextra` (no warnings). The Windows and macOS code paths follow documented platform APIs but should be verified on your own target machines.

Build commands:

```sh
# Linux / macOS
g++ -std=c++17 -O2 main.cpp -o main
clang++ -std=c++17 -O2 main.cpp -o main

# Windows (MSVC, from a Developer Command Prompt)
cl /std:c++17 /EHsc main.cpp

# Windows (MinGW)
g++ -std=c++17 main.cpp -o main.exe
```

With CMake:

```cmake
cmake_minimum_required(VERSION 3.10)
project(myapp CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(myapp main.cpp)
target_include_directories(myapp PRIVATE path/to/dir/containing/nbinput)
```

---

## 3. Core concepts

### 3.1 Non-blocking vs. blocking input

`std::cin >> c` and `getchar()` **wait** until the user presses Enter (line-buffered terminal) and block your whole program while they wait. `nbi::Input` switches the terminal into a mode where:

- keys are delivered **immediately**, without needing Enter (no line buffering);
- typed characters are **not echoed** by the terminal (you decide what to display);
- reading a key returns right away with "nothing" if no key is waiting.

### 3.2 The `Input` object is the "session"

Creating an `nbi::Input` *starts* the session; destroying it *ends* it. There is no separate `init()`/`shutdown()`. Keep the object alive for as long as you want non-blocking input — usually for the entire lifetime of `main()` or of one interactive screen.

### 3.3 Return values are plain `int`

All read functions return an `int`:

- `nbi::NONE` (`-1`) — no input available;
- `0 … 127` — an ASCII character (`'a'`, `' '`, `'\n'`, a control code such as `1` for Ctrl+A, …);
- `1000 …` — a special key from `nbi::Key` (arrows, Home, End, Delete).

Because it is an `int`, you can compare directly with character literals and `nbi::Key` constants:

```cpp
int k = in.poll();
if (k == 'y' || k == 'Y') { ... }
if (k == nbi::Key::Enter) { ... }
```

### 3.4 One key per call

Each call to `poll()` or `wait()` returns **at most one key**. If several keys are pending, call `poll()` repeatedly (see [drain pattern](#61-game--animation-loop-drain-all-pending-keys)).

---

## 4. API reference

All symbols live in namespace `nbi`.

### 4.1 `nbi::NONE`

```cpp
inline constexpr int NONE = -1;
```

Returned by `poll()` and `wait()` when no key is available.

---

### 4.2 `class nbi::Input`

Enables non-blocking input for its lifetime.

#### Constructor

```cpp
Input();
```

Enters non-blocking mode.

| Platform | What happens |
|----------|--------------|
| POSIX | Saves the current `termios` settings of `stdin`, disables canonical mode (`ICANON`) and echo (`ECHO`), sets `VMIN=0`/`VTIME=0`. Registers an `atexit` handler and handlers for `SIGINT`, `SIGTERM`, `SIGHUP` that restore the terminal. |
| Windows | Nothing needs to be changed; the console functions used (`_kbhit`/`_getch`) are already non-echoing and unbuffered. |

**Throws**

| Exception | When |
|-----------|------|
| `std::logic_error` | Another `nbi::Input` instance is currently alive. |
| `std::runtime_error` | (POSIX) `stdin` is not a terminal, e.g. input is piped or redirected. |

#### Destructor

```cpp
~Input();
```

Restores the terminal to its original state (POSIX) and releases the "one instance" lock.

#### Copy / move

```cpp
Input(const Input&)            = delete;
Input& operator=(const Input&) = delete;
```

Not copyable. (Also not movable, as no move operations are declared.) Create it once and pass it around by reference.

---

#### `int Input::poll()`

```cpp
int poll();
```

Returns the next pending key, or `nbi::NONE` immediately if there is none. **Never blocks** (apart from a tiny ≤ 20 ms look-ahead on POSIX when an `ESC` byte arrives, used to tell the Esc key from an arrow-key sequence; see [§7.2](#72-escape-key-vs-escape-sequences)).

**Returns**

| Value | Meaning |
|-------|---------|
| `nbi::NONE` | No key available (or an unsupported/unrecognized key was consumed and ignored) |
| `0–127` | ASCII character / control code |
| `nbi::Key::*` (≥ 1000) | Special key |

**Example**

```cpp
while (running) {
    int k = in.poll();
    if (k != nbi::NONE) handle(k);
    update();
}
```

---

#### `int Input::wait(int timeout_ms = -1)`

```cpp
int wait(int timeout_ms = -1);
```

Blocks until a key arrives or the timeout expires.

| `timeout_ms` | Behavior |
|--------------|----------|
| `-1` (default) | Wait indefinitely until a key is pressed |
| `0` | Equivalent to `poll()` |
| `> 0` | Wait up to that many milliseconds |

**Returns** the key, or `nbi::NONE` if the timeout expired.

On POSIX, waiting is done with `select()`, so it costs no CPU while idle. On Windows it polls `_kbhit()` with `Sleep(1)` between checks (≈ negligible CPU).

Typical uses: a frame limiter that also reacts instantly to input, "press any key to continue", or a prompt with a timeout.

```cpp
// Sleep ~100 ms, but wake up immediately if a key is pressed.
int k = in.wait(100);
```

---

#### `bool Input::pressed()`

```cpp
bool pressed();
```

Returns `true` if there is at least one byte/event waiting to be read. **Does not consume it.** Useful for quick checks such as "is the user trying to interrupt this long operation?".

> Caveat: `pressed()` may return `true` for input that `poll()` later ignores (for example an unsupported non-ASCII byte). Treat it as a hint; use `poll()`'s return value as the truth.

```cpp
for (auto& item : big_list) {
    process(item);
    if (in.pressed()) { in.flush(); break; }   // user pressed something: cancel
}
```

---

#### `void Input::flush()`

```cpp
void flush();
```

Discards all pending input. Call it before asking a question so that stray keystrokes typed earlier ("type-ahead") are not mistaken for the answer.

```cpp
in.flush();
std::cout << "Delete everything? (y/n) ";
int answer = in.wait();
```

---

### 4.3 `bool nbi::is_printable(int k)`

```cpp
inline constexpr bool is_printable(int k);
```

Returns `true` for the printable ASCII range `32 … 126` (space through `~`). Use it to decide whether a key should be appended to a text buffer. It returns `false` for `NONE`, control codes, `Enter`, `Backspace`, `Tab`, and all `Key::*` special keys.

```cpp
if (nbi::is_printable(k)) text.push_back(static_cast<char>(k));
```

---

## 5. Key codes

### 5.1 `namespace nbi::Key`

| Constant | Value | Notes |
|----------|-------|-------|
| `Key::Tab` | `'\t'` (9) | |
| `Key::Enter` | `'\n'` (10) | Always `'\n'`; carriage return (`\r`) is normalized |
| `Key::Escape` | `27` | The Esc key pressed on its own |
| `Key::Backspace` | `127` | Always 127; Windows' `8` and POSIX' `8` are normalized |
| `Key::Up` | `1000` | Arrow up |
| `Key::Down` | `1001` | Arrow down |
| `Key::Right` | `1002` | Arrow right |
| `Key::Left` | `1003` | Arrow left |
| `Key::Home` | `1004` | |
| `Key::End` | `1005` | |
| `Key::Delete` | `1006` | The forward-delete key |

### 5.2 Ordinary characters

Any printable key is returned as its ASCII value, honoring Shift and Caps Lock as produced by the OS:

```cpp
k == 'a'   // lowercase a
k == 'A'   // Shift+a
k == '1'   // digit key
k == '!'   // Shift+1 on a US layout
k == ' '   // space
```

### 5.3 Control characters

Ctrl+letter combinations arrive as the ASCII control codes `1 … 26` (Ctrl+A = `1`, Ctrl+B = `2`, …). A few are handled by the OS and never reach you; see [§7.3](#73-control-key-combinations).

```cpp
constexpr int ctrl(char c) { return c & 0x1F; }   // ctrl('a') == 1

if (k == ctrl('l')) redraw_screen();
```

---

## 6. Usage recipes

### 6.1 Game / animation loop (drain all pending keys)

Process every pending key each frame so that fast typing never lags behind:

```cpp
#include <chrono>
#include <thread>
#include "nbinput.hpp"

int main() {
    nbi::Input in;
    int x = 10, y = 5;

    for (bool running = true; running;) {
        // 1. Input: drain everything that arrived since the last frame.
        for (int k; (k = in.poll()) != nbi::NONE;) {
            switch (k) {
                case nbi::Key::Up:    --y; break;
                case nbi::Key::Down:  ++y; break;
                case nbi::Key::Left:  --x; break;
                case nbi::Key::Right: ++x; break;
                case 'q':             running = false; break;
            }
        }

        // 2. Update + render
        render(x, y);

        // 3. Frame pacing (~30 FPS)
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
}
```

### 6.2 Frame limiter that wakes instantly on input

Replacing `sleep_for` with `wait()` makes the program react immediately to a key instead of after the sleep ends:

```cpp
for (bool running = true; running;) {
    render();

    int k = in.wait(33);            // sleeps up to 33 ms, wakes early on a key
    if (k == 'q') running = false;
    // handle other keys...
}
```

> Note: this paces the loop at *up to* 33 ms per iteration, but key presses shorten the iteration. If you need a strict frame rate, use the drain pattern in 6.1 with a fixed-step timer.

### 6.3 Single-line text input with editing

A minimal line editor that echoes characters itself and supports Backspace and Enter, while a background task keeps running:

```cpp
#include <iostream>
#include <string>
#include "nbinput.hpp"

// Returns true when a full line has been entered (stored in `out`).
bool poll_line(nbi::Input& in, std::string& buffer, std::string& out) {
    for (int k; (k = in.poll()) != nbi::NONE;) {
        if (k == nbi::Key::Enter) {
            out = buffer;
            buffer.clear();
            std::cout << '\n';
            return true;
        }
        if (k == nbi::Key::Backspace) {
            if (!buffer.empty()) {
                buffer.pop_back();
                std::cout << "\b \b" << std::flush;   // erase last char on screen
            }
        } else if (nbi::is_printable(k)) {
            buffer.push_back(static_cast<char>(k));
            std::cout << static_cast<char>(k) << std::flush;
        }
    }
    return false;
}

int main() {
    nbi::Input in;
    std::string buffer, line;

    std::cout << "name> " << std::flush;
    while (!poll_line(in, buffer, line)) {
        do_background_work();
    }
    std::cout << "Hello, " << line << "!\n";
}
```

### 6.4 "Press any key to continue"

```cpp
void pause() {
    nbi::Input in;                      // scoped: terminal restored on return
    std::cout << "Press any key to continue..." << std::flush;
    in.flush();                         // ignore earlier type-ahead
    in.wait();                          // block until a key
    std::cout << '\n';
}
```

### 6.5 Yes/No prompt with a timeout

```cpp
bool confirm(nbi::Input& in, int seconds, bool default_answer) {
    in.flush();
    std::cout << "Continue? [y/n] (auto "
              << (default_answer ? "yes" : "no") << " in " << seconds << "s) " << std::flush;

    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(seconds);
    for (;;) {
        auto left = std::chrono::duration_cast<std::chrono::milliseconds>(
                        deadline - std::chrono::steady_clock::now()).count();
        if (left <= 0) return default_answer;

        int k = in.wait(static_cast<int>(left));
        if (k == 'y' || k == 'Y') return true;
        if (k == 'n' || k == 'N') return false;
        if (k == nbi::Key::Enter) return default_answer;
        // any other key: keep waiting
    }
}
```

### 6.6 Menu navigation with arrow keys

```cpp
int run_menu(nbi::Input& in, const std::vector<std::string>& items) {
    int sel = 0;
    for (;;) {
        draw_menu(items, sel);

        switch (in.wait()) {
            case nbi::Key::Up:    sel = (sel + items.size() - 1) % items.size(); break;
            case nbi::Key::Down:  sel = (sel + 1) % items.size();                break;
            case nbi::Key::Enter: return sel;
            case nbi::Key::Escape:return -1;                                     // cancelled
        }
    }
}
```

### 6.7 Cancelling a long operation

```cpp
bool run_long_job(nbi::Input& in) {
    for (int i = 0; i < 1'000'000; ++i) {
        do_step(i);
        if (in.pressed() && in.poll() == nbi::Key::Escape) return false;  // user cancelled
    }
    return true;
}
```

### 6.8 Mixing with `std::cin` / `std::getline`

`std::cin` expects a normal (line-buffered, echoing) terminal. Scope the `Input` object so the terminal is restored before using `std::cin`:

```cpp
{
    nbi::Input in;
    run_interactive_screen(in);
}   // terminal restored here

std::string name;
std::getline(std::cin, name);   // works normally again
```

### 6.9 Reusable keyboard class wrapper (optional pattern)

If you want a global, easily-accessible instance, wrap it yourself (the library intentionally does not impose a singleton):

```cpp
class App {
public:
    void run() {
        while (running_) {
            handle_input();
            update();
            render();
        }
    }
private:
    void handle_input() {
        for (int k; (k = in_.poll()) != nbi::NONE;) { /* dispatch */ }
    }
    nbi::Input in_;     // lives as long as App
    bool running_ = true;
};
```

---

## 7. Platform behavior

### 7.1 Implementation summary

| Aspect | Linux / macOS | Windows |
|--------|---------------|---------|
| Mechanism | `termios` (`ICANON`/`ECHO` off, `VMIN=0`, `VTIME=0`) + `select()` + `read()` | `_kbhit()` + `_getch()` from `<conio.h>` |
| `poll()` | `select()` with zero timeout, then `read()` of 1 byte | `_kbhit()` then `_getch()` |
| `wait()` | `select()` with timeout (no CPU use while idle) | Loop of `poll()` + `Sleep(1)` |
| Terminal restoration | Destructor, `atexit`, and signal handlers | Not required |
| Special keys | Parsed from ANSI escape sequences (`ESC [ A`, `ESC O A`, `ESC [ 3 ~`, …) | Two-byte codes (`0`/`0xE0` prefix + scan code) |
| Echo | Disabled by the library | `_getch` does not echo |
| Output processing | Left enabled (`OPOST`), so `'\n'` still moves to the next line properly | Unchanged |

### 7.2 Escape key vs. escape sequences

On POSIX terminals, arrow keys are transmitted as multi-byte sequences beginning with the byte `27` (ESC), e.g. Up = `ESC [ A`. The Esc key itself sends just `27`.

To tell them apart, after receiving `27` the library waits up to **20 ms** for the following byte. If none arrives, it reports `Key::Escape`; otherwise it decodes the sequence. Consequences:

- `Key::Escape` is reported ~20 ms after the physical press (not noticeable by humans).
- Over very slow links (e.g. high-latency SSH), the bytes of an arrow key could in theory be separated by more than 20 ms, producing a spurious `Escape` followed by stray characters. This is rare on normal local use.
- Unrecognized sequences (e.g. function keys such as F5) are consumed and reported as `nbi::NONE`.

### 7.3 Control key combinations

| Combination | POSIX behavior | Windows behavior |
|-------------|----------------|------------------|
| Ctrl+C | The terminal generates `SIGINT` (default action: terminate). The library restores the terminal first, then the process terminates | `_getch` returns `3`; the library raises `SIGINT` so the behavior matches POSIX |
| Ctrl+Z | Suspends the process (`SIGTSTP`); terminal stays in raw mode while suspended | Returned as key `26` |
| Ctrl+\\ | `SIGQUIT` (core dump / terminate) | Returned as key `28` |
| Ctrl+D | Returned as key `4` (it is **not** EOF, since canonical mode is off) | Returned as key `4` |
| Ctrl+S / Ctrl+Q | Software flow control may freeze/unfreeze terminal output (terminal default) | Returned as keys `19` / `17` |
| Ctrl+A … Ctrl+Y (others) | Returned as `1 … 25` | Returned as `1 … 25` |
| Ctrl+J | Returned as `'\n'` (indistinguishable from Enter) | Returned as `'\n'` |
| Ctrl+I | Returned as `'\t'` (same as Tab) | Returned as `'\t'` |

### 7.4 Terminal state restoration (POSIX)

The original terminal settings are restored in these situations:

1. The `nbi::Input` object is destroyed (normal case).
2. The program calls `std::exit()` or returns from `main()` while the object is still alive (via `atexit`).
3. The process receives `SIGINT`, `SIGTERM`, or `SIGHUP` (the library restores the terminal, resets the handler to default, and re-raises the signal so the process exits with the expected status).

Not covered: `SIGKILL` (cannot be caught), crashes such as segmentation faults, and `std::abort()`. If that happens the shell may appear to "not echo" typed text; run `reset` or `stty sane` to fix it.

---

## 8. Error handling

The library reports problems only from the constructor, using standard exceptions:

```cpp
try {
    nbi::Input in;
    // ...
} catch (const std::logic_error& e) {
    // "nbi::Input: only one instance is allowed"
} catch (const std::runtime_error& e) {
    // "nbi::Input: stdin is not a terminal (TTY)"
}
```

A common reason for `runtime_error` is running with redirected input:

```sh
./myapp < input.txt        # stdin is a file: constructor throws
echo hi | ./myapp          # stdin is a pipe: constructor throws
```

If your program should also support non-interactive use, handle this explicitly:

```cpp
#include <iostream>

int main() {
    try {
        nbi::Input in;
        run_interactive(in);
    } catch (const std::runtime_error&) {
        run_batch_from_stdin();     // fall back to std::cin
    }
}
```

`poll()`, `wait()`, `pressed()`, and `flush()` do not throw. Read errors are reported as `nbi::NONE`.

---

## 9. Limitations and caveats

| Topic | Detail |
|-------|--------|
| ASCII only | Bytes ≥ 128 (accented letters, emoji, CJK, …) are ignored and reported as `NONE`. |
| Limited key set | Supported special keys: arrows, Home, End, Delete, Tab, Enter, Backspace, Esc. Not supported: Insert, Page Up/Down, F1–F12, Alt combinations, mouse. |
| One instance | Only one `nbi::Input` may exist at once (`std::logic_error` otherwise). |
| Not thread-safe | Call all functions from a single thread. |
| Custom signal handlers | The constructor installs handlers for `SIGINT`, `SIGTERM`, `SIGHUP` on POSIX, replacing any previously installed ones. If you install your own handler *after* constructing `Input`, make sure the program still exits normally (so `atexit`/destructors run) or restore the terminal yourself. |
| No echo | Characters are never echoed. You must print what you want the user to see. |
| Suspend (Ctrl+Z) | On POSIX the terminal remains in raw mode while the process is suspended (shells normally reset it themselves). |
| TTY required (POSIX) | `stdin` must be a terminal. |
| Windows event types | Only keyboard input is handled; mouse/resize/focus events are ignored. |
| Windows scan codes | `_getch` special-key handling covers the keys listed above. Numeric-keypad keys with NumLock off report the same scan codes as the dedicated keys. |
| Windows ANSI output | The library does not enable ANSI escape output on Windows. If you print ANSI codes (colors, cursor movement), enable virtual-terminal processing yourself with `SetConsoleMode(..., ENABLE_VIRTUAL_TERMINAL_PROCESSING)`. |
| End of input | If the terminal is closed or hits EOF on POSIX, `wait()` may return `NONE` repeatedly and quickly; treat repeated `NONE` from `wait(-1)` as a signal to exit. |

---

## 10. Troubleshooting / FAQ

**My terminal doesn't show what I type after my program crashed.**
The terminal was left in raw mode (e.g. after `SIGKILL` or a crash). Type `reset` (or `stty sane`) and press Enter, even if you can't see the characters.

**The constructor throws `stdin is not a terminal (TTY)`.**
You are redirecting or piping `stdin`, or running inside an environment without a real terminal (some IDE "run" consoles, CI). Run the program from an actual terminal, or catch the exception and fall back to batch mode (see [§8](#8-error-handling)).

**Nothing happens when I press keys in my IDE's output window.**
Many IDE consoles are not real terminals and do not forward keystrokes immediately. Run the program in the system terminal (Terminal.app, GNOME Terminal, Windows Terminal, cmd.exe).

**I see `^[[A` printed when I press the arrow keys.**
The library is not active at that moment (object not created yet / already destroyed), or you are reading with `std::cin` instead of `poll()`/`wait()`.

**Text I print with `\n` appears staircased (each line starts further right).**
Output post-processing was disabled by something else. This library keeps `OPOST` enabled, so check other code that modifies terminal settings (`cfmakeraw`, `stty raw`, other libraries).

**Pressing Esc has a tiny delay.**
That's the 20 ms look-ahead that distinguishes Esc from arrow-key sequences (see [§7.2](#72-escape-key-vs-escape-sequences)).

**Two keys pressed quickly — one is lost.**
Nothing is lost: pending keys queue up in the OS. `poll()` returns one key per call, so drain with a loop (see [§6.1](#61-game--animation-loop-drain-all-pending-keys)).

**Characters typed while the program was busy show up later.**
That is type-ahead. Call `in.flush()` before reading an answer to a prompt if you want to discard it.

**Can I use it with multiple threads?**
Not safely. Read input in one thread and communicate with other threads through your own thread-safe queue.

**Can I get Alt/Ctrl+arrow, F-keys, Page Up/Down?**
Not in this version. On POSIX these arrive as longer escape sequences that the parser currently consumes and reports as `NONE`. Extending `parse_escape()` (POSIX) and the scan-code `switch` (Windows) is straightforward if you need them.

---

## 11. Design notes

- **Why `int` instead of an enum or `std::optional<char>`?** A single `int` lets ASCII characters, special keys, and "no input" share one simple type with no wrapping, which keeps call sites short (`if (k == 'q')`, `if (k == nbi::Key::Up)`).
- **Why normalized codes?** Terminals and Windows disagree about Enter (`\r` vs `\n`), Backspace (`8` vs `127`), and special keys. Normalizing in the library keeps your application code free of `#ifdef`s.
- **Why RAII?** Raw terminal mode is global state that must always be undone. Tying it to object lifetime makes correct cleanup the default, even when exceptions propagate.
- **Why keep `ISIG` on?** Ctrl+C should keep working as users expect. The cost is that the library installs signal handlers to restore the terminal first.
- **Why `select()` and not `poll()`/`epoll`?** `select()` is available on every POSIX system including macOS (where `poll()` on terminals has historically had quirks) and is sufficient for a single file descriptor.
- **Why header-only?** One file to copy, no build-system integration, and everything is `inline` so it is safe to include from multiple translation units.

---

## File overview

| File | Purpose |
|------|---------|
| `nbinput.hpp` | The library (header-only) |
| `example.cpp` | Minimal runnable demo: ticking counter, text buffer, arrow-key events |
| `README.md` | This documentation |