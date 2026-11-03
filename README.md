# TPaint

> **Disclaimer:** TPaint is a self-learning project, not production-ready
> software. It may contain bugs, logic errors, or inefficient code, and it is
> provided as-is. Use it with that in mind.

TPaint is a small graphics editor that runs in your terminal. Use the arrow keys
to move around a colored canvas, choose from 16 terminal colors, and draw by
painting cells. It is a simpler, keyboard-driven take on a paint program.

This is a self-learning project created to strengthen my C++ skills. It is built
with C++20 and the standard library, with operating-system-specific APIs used
for non-blocking keyboard input.

## Requirements

- **A C++20-capable compiler.** Use a recent version of a compiler with C++20
  support, such as GCC, Clang, or Microsoft Visual C++ (MSVC). CMake will use
  the compiler installed and available on your system.
- **CMake 3.20 or newer** to configure the project and generate build files.
- **A compatible interactive terminal.** TPaint needs a real terminal that
  accepts arrow-key input and supports ANSI/VT escape sequences for cursor
  positioning and colors (including the standard and bright 16-color palette).
  Examples include GNOME Terminal or Konsole on Linux, Terminal or iTerm2 on
  macOS, and Windows Terminal or a modern Windows console. IDE output panes,
  redirected input, and other non-interactive consoles may not work correctly.

The interface uses a 20-row by 100-column canvas, so use a terminal window wide
and tall enough to show it.

## Build and run

From the project directory, configure and build a Release version.

On Linux and macOS:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Run it from a terminal:

```sh
./build/tpaint
```

On Windows, configure and build with:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

Run `build\Release\tpaint.exe` from Command Prompt, PowerShell, or Windows
Terminal.

### Optional: add TPaint to PATH

Adding the folder containing the executable to your `PATH` lets you launch
TPaint by typing `tpaint` in a terminal. Use the full path to the folder where
you built the program; keep the project and build folder in place afterward.

- **Linux:** Add this line to your shell startup file, such as `~/.bashrc` or
  `~/.zshrc` (replace the example path):
  ```sh
  export PATH="$PATH:/absolute/path/to/tpaint/build"
  ```
- **macOS:** Add the same line to `~/.zshrc` (the default shell), replacing the
  example path with the folder containing `tpaint`, usually the project's
  `build` folder.
- **Windows:** Open **Edit environment variables for your account**, select
  **Path** under user variables, choose **Edit** > **New**, and add the full
  path to `build\Release` inside the project. Open a new terminal afterward.

On Linux and macOS, open a new terminal or reload your shell configuration
after editing it. Then launch the program with:

```sh
tpaint
```

## Controls

| Key | Action |
| --- | --- |
| Arrow keys | Move the cursor in Normal and Draw modes; move through the color palette in Color mode |
| `n` | Switch to Normal mode. Move the cursor without drawing. |
| `d` | Switch to Draw mode. Moving paints cells with the selected color. |
| `c` | Switch to Color mode. Use the arrow keys to select a color. |
| `q` | Quit TPaint. |

The cursor is shown as a `+` on the canvas. The selected color is displayed
above the palette. The palette contains the standard eight terminal colors and
their bright variants.

## Current limitations

TPaint is intentionally minimal: it has no save or load, undo, or other editing
tools. Artwork is lost when you quit. Its colors depend on the terminal's color
scheme, so they may look different across terminals.
