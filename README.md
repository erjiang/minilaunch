# Minimal X11 Application Launcher

A lightweight X11-based application launcher for Linux, written in C++. This minimal launcher provides a simple interface for running commands, evaluating math expressions, and performing unit conversions.

It's intended to launch very quickly (in a couple ms, usually) and have a small file size.

![minilaunch screenshot](https://github.com/user-attachments/assets/471c7785-a981-4954-bcba-1595e5364c49)

---

## Features

- **Command Execution**: Type and run shell commands directly.
- **Open Directories**: If the input is a directory, it will be opened using `xdg-open`.
- **Math Evaluation**: Automatically detects and evaluates basic math expressions.
- **Unit Conversion**: Convert between units using expressions like `100 cm in m`. This calls out to the `units` program.
- **Keyboard Shortcuts**:
  - `Ctrl+A`: Clear input.
  - `Enter`: Execute command.
  - `Esc`: Exit the application.
  - `Backspace`: Delete the last character.
- **Minimal UI**: Simple and responsive interface with fast load times.

---

## Installation

### Prerequisites

Ensure the following dependencies are installed:

- **X11 Development Libraries**: Required for compiling and running the application.
- **CMake**: For building the project.
- **GCC**: Or another C++ compiler.

### Clone the Repository

```bash
git clone git@github.com:erjiang/minilaunch.git
cd minilaunch
```

### Build Instructions

1. Create a `build` directory and navigate to it:
   ```bash
   mkdir build && cd build
   ```
2. Configure and build the project using CMake:
   ```bash
   cmake ..
   make
   ```
3. The compiled binary will be available as `minilaunch` in the `build` directory.

---

## Usage

1. Run the application:
   ```bash
   ./build/Launcher
   ```
2. Enter commands, math expressions, or unit conversions in the input field:
   - **Example Commands**:
     - `ls` (list files in the current directory)
     - `firefox` (launch Firefox browser)
   - **Example Math Expressions**:
     - `2+2`
     - `5*10/2`
   - **Example Unit Conversions**:
     - `10 km in m`
     - `100 cm in inches`

---

## Development

### Project Structure

- **Source Files**:
  - `main.cpp`: Main application logic.
  - `launchmath.cpp` and `units.cpp`: Math and unit conversion utilities.

---

## License

This project is licensed under the MIT License. See `LICENSE` for details.
