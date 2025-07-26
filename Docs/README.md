# Protogame3D

A modern 3D game engine and prototype game built with C++ and custom engine architecture, featuring JavaScript scripting support and professional audio integration.

## 🎮 Overview

Protogame3D is a comprehensive 3D game development project that demonstrates modern game engine architecture patterns. The project serves as both a functional game prototype and a showcase of custom engine development techniques, including modular design, scripting integration, and professional audio systems.

## ✨ Features

### Core Engine Features
- **Custom 3D Engine**: Built-in game engine with modular architecture
- **JavaScript Integration**: Dynamic scripting system for gameplay logic
- **Professional Audio**: FMOD audio engine integration for immersive sound
- **Modern Rendering**: Support for 3D models, shaders, and texture mapping
- **Entity System**: Flexible entity-component architecture
- **Resource Management**: Efficient loading and management of game assets

### Development Features
- **Cross-Platform**: C++ codebase with x64 architecture support
- **Visual Studio Integration**: Complete solution and project files
- **Debug/Release Builds**: Optimized build configurations
- **Git Submodules**: Modular dependency management
- **Configurable Settings**: XML-based game configuration system

## 🏗️ Project Structure

```
Protogame3D/
├── Code/
│   └── Game/                    # Core game logic
│       ├── Game.cpp/hpp         # Main game class
│       ├── Player.cpp/hpp       # Player system
│       ├── Entity.cpp/hpp       # Entity management
│       ├── Prop.cpp/hpp         # Game objects/props
│       ├── JavaScriptManager.*  # Scripting system
│       ├── Framework/           # Game framework
│       └── Subsystem/           # Engine subsystems
├── Run/
│   ├── Data/                    # Game assets
│   │   ├── Audio/               # Sound effects and music
│   │   ├── Fonts/               # Typography resources
│   │   ├── Images/              # Textures and UI graphics
│   │   ├── Models/              # 3D model assets
│   │   ├── Scripts/             # JavaScript game scripts
│   │   ├── Shaders/             # Rendering shaders
│   │   └── GameConfig.xml       # Game configuration
│   ├── Protogame3D_Debug_x64.exe    # Debug build
│   ├── Protogame3D_Release_x64.exe  # Release build
│   └── fmod*.dll                # FMOD audio libraries
├── Engine/                      # Custom game engine (submodule)
├── Docs/                        # Project documentation
└── Protogame3D.sln             # Visual Studio solution
```

## 🚀 Getting Started

### Prerequisites
- **Visual Studio 2019/2022** with C++ development tools
- **Git** for version control and submodule management
- **Windows 10/11** (x64)

### Building the Project

1. **Clone the repository with submodules:**
   ```bash
   git clone --recursive https://github.com/yourusername/Protogame3D.git
   cd Protogame3D
   ```

2. **Update submodules (if needed):**
   ```bash
   git submodule update --init --recursive
   ```

3. **Open in Visual Studio:**
   ```bash
   start Protogame3D.sln
   ```

4. **Build the solution:**
   - Select your desired configuration (Debug/Release)
   - Build → Build Solution (Ctrl+Shift+B)

5. **Run the game:**
   - Navigate to `Run/` directory
   - Execute `Protogame3D_Debug_x64.exe` or `Protogame3D_Release_x64.exe`

## 🎯 Game Configuration

The game can be customized through `Run/Data/GameConfig.xml`:

```xml
<GameConfig>
    <WindowClose>false</WindowClose>
    <screenSizeX>1600</screenSizeX>
    <screenSizeY>800</screenSizeY>
    <screenCenterX>800</screenCenterX>
    <screenCenterY>400</screenCenterY>
</GameConfig>
```

### Configurable Options
- **Screen Resolution**: Customize window size and positioning
- **Window Behavior**: Control window close behavior
- **Graphics Settings**: Adjust rendering parameters
- **Audio Settings**: Configure sound and music preferences

## 🛠️ Development

### Architecture Overview
- **Modular Design**: Separation between engine and game logic
- **Component System**: Entity-component architecture for game objects
- **Event-Driven**: Event system for loose coupling between systems
- **Resource Pipeline**: Efficient asset loading and management
- **Scripting Layer**: JavaScript integration for rapid prototyping

### Key Components
- **Game Class**: Main game loop and state management
- **Player System**: Character controller and player interactions
- **Entity Management**: Dynamic object creation and lifecycle
- **Prop System**: Interactive game objects and environment elements
- **JavaScript Manager**: Script execution and C++ binding

### Adding New Features
1. Create new classes in `Code/Game/`
2. Add corresponding headers and implement functionality
3. Register with the main game loop in `Game.cpp`
4. Add any required assets to `Run/Data/`
5. Update configuration files as needed

## 📦 Dependencies

### External Libraries
- **FMOD**: Professional audio engine for sound and music
- **Custom Engine**: Proprietary game engine (included as submodule)

### Engine Features
- Rendering system with shader support
- Input handling and device management
- Resource loading and caching
- Debug rendering and tools
- Math utilities and transformations

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Guidelines
- Follow existing code style and conventions
- Add appropriate comments and documentation
- Test your changes in both Debug and Release builds
- Update README.md if adding new features or dependencies

## 📄 License

This project is licensed under the Apache License 2.0 - see the [LICENSE](../LICENSE) file for details.

## 🔗 Links

- **Engine Repository**: [Custom Game Engine](https://github.com/dadavidtseng/Engine)
- **FMOD Audio**: [Official Website](https://www.fmod.com/)
- **Issue Tracker**: Report bugs and request features

## 🙏 Acknowledgments

- FMOD by Firelight Technologies for professional audio capabilities
- Custom engine development team
- Open source community for inspiration and support

---

**Built with ❤️ using C++ and modern game development practices**
