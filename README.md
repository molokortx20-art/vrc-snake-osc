# VRChat Snake over OSC

[![C++17](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)](https://microsoft.com/windows)
[![VRChat](https://img.shields.io/badge/VRChat-OSC%20Chatbox-ff4500.svg)](https://docs.vrchat.com/docs/osc-overview)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Interactive Snake game streamed directly into the **VRChat Chatbox** via local **OSC (Open Sound Control)** in real-time. Works on **any avatar** in any world without needing avatar modifications or uploads.

---

## Features

- **No Avatar Modifications Required**: Streams the game board directly to `/chatbox/input` via UDP (`127.0.0.1:9000`).
- **High-Performance C++17 Engine**: Zero heap allocations in the game loop, minimal CPU usage.
- **Fully Customizable (`config.ini`)**: Change grid size, food count, restart key, update tick rate, and emoji themes.
- **Silent Background Execution**: Runs unobtrusively in the background while you play in VRChat.
- **Instant Restart**: Instant board refresh upon pressing the restart key.

---

## Controls

| Key | Action |
| :--- | :--- |
| **`W / A / S / D`** or **Arrows** | Steer the Snake (Up, Down, Left, Right) |
| **`F`** *(configurable)* | Restart game after Game Over |
| **`Esc`** | Exit application |

---

## Configuration (`config.ini`)

All game settings can be modified without recompiling:

```ini
[Game]
// Grid width (default: 7)
grid_width = 7

// Grid height (default: 7)
grid_height = 7

// Simultaneous food count (default: 1)
food_count = 1

// Update interval in seconds (default: 1.15)
tick_interval = 1.15

[Controls]
// Restart key (default: F)
restart_key = F

[Appearance]
// Head symbol (default: 🐱)
head_symbol = 🐱

// Body symbol (default: ⚪)
body_symbol = ⚪

// Food symbol (default: 🐟)
food_symbol = 🐟

// Empty tile symbol (default: ▫️)
empty_symbol = ▫️
```

---

## Building from Source

### Prerequisites
- Windows 10 / 11
- Microsoft Visual Studio 2022 (with C++ Desktop Development workload)

### Build in One Click
Run the included batch script from the project root:

```cmd
build.bat
```

The compiled binary will be placed in `bin/vrc_snake_osc.exe`.

### Manual Compilation
```cmd
cl.exe /nologo /O2 /std:c++17 /EHsc /W3 /I src src\main.cpp /Fe"bin\vrc_snake_osc.exe" /link ws2_32.lib user32.lib
```

---

## Usage

1. Launch **VRChat** and ensure OSC is enabled:
   - Open Action Menu -> **Options** -> **OSC** -> **Enabled**.
2. Run `vrc_snake_osc.exe`:
   ```cmd
   bin\vrc_snake_osc.exe
   ```
3. Play the game directly using your keyboard!

---

## Author & Contacts

- **Developer**: [molokortx20-art](https://github.com/molokortx20-art)
- **Discord**: `thedeathforxxx`
- **GitHub**: [github.com/molokortx20-art](https://github.com/molokortx20-art)

---

## License

This project is licensed under the [MIT License](LICENSE).
