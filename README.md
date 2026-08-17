# VRChat Snake over OSC

[![C++17](https://img.shields.io/badge/C%2B%2B17-blue.svg)](https://isocpp.org/)
[![VRChat](https://img.shields.io/badge/VRChat-OSC-orange.svg)](https://docs.vrchat.com/docs/osc-overview)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

Real-time Snake game rendered inside the VRChat Chatbox via local OSC. Supports both VR controllers and keyboard.

## Controls

| Device | Action |
| :--- | :--- |
| **VR Controller (Thumbstick)** | Steer the Snake (Up, Down, Left, Right) |
| **VR Controller (Jump)** | Restart game |
| **Keyboard (`W/A/S/D` or Arrows)** | Steer the Snake |
| **Keyboard (`F` / configurable)** | Restart game |

## Configuration (`config.ini`)

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
// Head symbol (default: ??)
head_symbol = ??

// Body symbol (default: ?)
body_symbol = ?

// Food symbol (default: ??)
food_symbol = ??

// Empty tile symbol (default: ??)
empty_symbol = ??
```

## Build

Requires Visual Studio 2022 with MSVC:

```cmd
build.bat
```

Output binary: `bin/vrc_snake_osc.exe`.

## Usage

1. Enable OSC in VRChat: **Action Menu -> Options -> OSC -> Enabled**.
2. Run `bin/vrc_snake_osc.exe`.

## Contacts

- **Discord**: `thedeathforxxx`
- **GitHub**: [molokortx20-art](https://github.com/molokortx20-art)

## License

[MIT](LICENSE)
