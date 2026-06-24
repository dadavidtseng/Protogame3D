<a id="readme-top"></a>

<!-- PROJECT SHIELDS -->
![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![DirectX](https://img.shields.io/badge/DirectX-11-107C10?style=for-the-badge&logo=xbox&logoColor=white)
![FMOD](https://img.shields.io/badge/FMOD-000000?style=for-the-badge&logo=fmod&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
[![Apache 2.0 License][license-shield]][license-url]

<!-- PROJECT TITLE -->
<div align="center">
  <h1>Protogame3D</h1>
  <p>A 3D game template built on the Daemon Engine with free-fly camera, procedural props, and a debug render toolkit</p>
</div>

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [How to Install](#how-to-install)
- [How to Use](#how-to-use)
- [Project Structure](#project-structure)
- [License](#license)
- [Contact](#contact)

## Overview

Protogame3D is a starter template for building 3D games on the custom [Daemon Engine](https://github.com/dadavidtseng/Engine). It extends the 2D protogame scaffold into 3D space with a free-fly player camera (keyboard/mouse + Xbox controller), procedurally generated props (cubes, sphere, grid), world coordinate axes with labeled basis vectors, and a full debug render toolkit for visualizing lines, points, spheres, cylinders, billboards, and basis transforms in world space.

Designed as a boilerplate that can be cloned and extended into a full 3D game project. Includes Blinn-Phong and Bloom shaders out of the box. The V8 scripting subsystem is disabled by default via `EngineBuildPreferences.hpp` to keep the build lightweight.

## Features

- **Free-fly 3D camera** — WASD + mouse look with keyboard and Xbox controller support, model-to-world transform pipeline
- **Procedural 3D props** — Cubes (rotating, color-pulsing), UV sphere, and reference grid generated from `Vertex_PCU` data
- **Debug render toolkit** — 7 debug draw types bound to numpad keys: world lines, points, wire spheres, basis transforms, billboard text, cylinders, and screen messages
- **World coordinate system** — Labeled X-Forward / Y-Left / Z-Up basis with persistent world-space text
- **Time controls** — Pause (P), single-frame step (O), slow-mo at 0.1× (T hold), real-time FPS/time/scale HUD

## How to Install

### Prerequisites

- Visual Studio 2022 (or 2019) with C++ desktop development workload
- Windows 10/11 (x64)
- DirectX 11 compatible GPU
- [Daemon Engine](https://github.com/dadavidtseng/Engine) cloned as a sibling directory

### Build

```bash
# Clone both repos side by side
git clone https://github.com/dadavidtseng/Engine.git
git clone https://github.com/dadavidtseng/Protogame3D.git

# Directory layout should be:
# ├── Engine/
# └── Protogame3D/
```

1. Open `Protogame3D.sln` in Visual Studio
2. Set configuration to `Debug | x64`
3. Build the solution (the Engine project is referenced automatically)
4. The executable is deployed to `Run/` via post-build event

## How to Use

### Controls

| Action | Key |
|--------|-----|
| Move | W / A / S / D |
| Look | Mouse |
| Start game | Space |
| Return to attract / Quit | ESC |
| Restart game | F8 |

### Debug Render Keys (In-Game)

| Key | Debug Draw |
|-----|------------|
| 1 | World line (forward ray, 20 units, x-ray) |
| 2 | World point (ground position, hold) |
| 3 | Wire sphere (2 units ahead, green to red fade) |
| 4 | World basis (player transform) |
| 5 | Billboard text (position + orientation) |
| 6 | World cylinder (at player position) |
| 7 | Screen message (camera orientation) |

### Time Controls

| Action | Key |
|--------|-----|
| Pause / Unpause | P |
| Step single frame | O |
| Slow-mo (0.1x) | T (hold) |

### Running

Launch `Run/Protogame3D_Debug_x64.exe` from the `Run/` directory (working directory must be `Run/` for asset loading).

## Project Structure

```
Protogame3D/
├── Code/Game/
│   ├── Framework/                 # Application framework
│   │   ├── Main_Windows.cpp       # WinMain entry point
│   │   ├── App                    # Application lifecycle (Startup -> RunMainLoop -> Shutdown)
│   │   └── GameCommon             # Global pointers, debug draw helpers, GAME_SAFE_RELEASE
│   ├── Gameplay/                  # Game logic
│   │   ├── Game                   # State machine (Attract / Game), prop spawning, debug draw bindings
│   │   ├── Entity                 # Base class (position, velocity, orientation, color)
│   │   ├── Player                 # Free-fly camera controller (keyboard/mouse + gamepad)
│   │   └── Prop                   # Procedural geometry (cube, sphere, grid, world axes, text)
│   └── EngineBuildPreferences.hpp # Engine feature toggles (V8 scripting disabled)
├── Run/                           # Runtime directory
│   ├── Data/Audio/                # Sound effects (TestSound.mp3)
│   ├── Data/Fonts/                # Bitmap fonts
│   ├── Data/Images/               # Textures (TestUV.png)
│   └── Data/Shaders/              # HLSL shaders (BlinnPhong, Bloom, Default)
├── Docs/                          # Documentation
└── Protogame3D.sln                # Visual Studio solution
```

## License

Copyright 2025 Yu-Wei Tseng

Licensed under the [Apache License, Version 2.0](../LICENSE).

## Contact

**Yu-Wei Tseng**
- Portfolio: [dadavidtseng.info](https://dadavidtseng.info)
- GitHub: [@dadavidtseng](https://github.com/dadavidtseng)
- LinkedIn: [dadavidtseng](https://www.linkedin.com/in/dadavidtseng/)
- Email: dadavidtseng@gmail.com

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- REFERENCE-STYLE LINKS -->
[license-shield]: https://img.shields.io/github/license/dadavidtseng/Protogame3D.svg?style=for-the-badge
[license-url]: https://github.com/dadavidtseng/Protogame3D/blob/main/LICENSE
