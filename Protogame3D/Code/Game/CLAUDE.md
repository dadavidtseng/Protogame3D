[Root Directory](../../CLAUDE.md) > [Code](../) > **Game**

# Game Module Documentation

## Changelog

**2025-09-22**: Initial module documentation - Main game application structure and subsystems

---

## Module Responsibilities

The Game module serves as the main application layer built on top of the Engine. It implements:

- **Application Lifecycle**: Main entry point, initialization, and game loop management
- **Game Logic**: Player systems, entity management, and game state handling
- **Game-Specific Subsystems**: Console, lighting, and widget management
- **Entity Management**: Player, props, and interactive game objects
- **Game State Management**: Attract mode and gameplay state transitions

## Entry and Startup

### Main Entry Point
- **File**: `Framework/Main_Windows.cpp`
- **Function**: `WinMain()` - Windows application entry point
- **Flow**: Creates App instance → Startup → RunMainLoop → Shutdown → Cleanup

### Application Framework
- **File**: `Framework/App.hpp/.cpp`
- **Class**: `App` - Main application controller
- **Key Methods**:
  - `Startup()`: Initialize all systems and create game instance
  - `RunMainLoop()`: Main game loop with frame timing
  - `RunFrame()`: Single frame execution (BeginFrame → Update → Render → EndFrame)
  - `DeleteAndCreateNewGame()`: Game reset functionality

### Game Instance
- **File**: `Game.hpp/.cpp` 
- **Class**: `Game` - Core game logic controller
- **States**: `eGameState::ATTRACT` and `eGameState::GAME`
- **Key Systems**: Player management, entity spawning, camera control

## External Interfaces

### Global Accessors (GameCommon.hpp)
```cpp
extern App*                   g_app;              // Main application instance
extern Game*                  g_game;             // Current game instance  
extern Renderer*              g_renderer;         // Graphics renderer
extern AudioSystem*           g_audio;            // Audio system
extern ResourceSubsystem*     g_resourceSubsystem; // Asset management
extern LightSubsystem*        g_lightSubsystem;   // Lighting system
```

### Event System Integration
- **File**: `Framework/App.hpp`
- **Event Handlers**: `OnCloseButtonClicked()` for window close events
- **Event System**: Uses engine's EventSystem for decoupled communication

### Debug Interface
- **Debug Rendering**: Custom debug drawing functions in GameCommon.hpp
- **Console Integration**: ConsoleSubsystem for runtime commands and debugging

## Key Dependencies and Configuration

### Project Configuration
- **File**: `Game.vcxproj`
- **Type**: Visual Studio C++ Application Project
- **Platform**: Windows (Win32/x64)
- **Dependencies**: Links against Engine static library

### Engine Dependencies
```cpp
#include "Engine/Core/EngineCommon.hpp"     // Core engine systems
#include "Engine/Core/EventSystem.hpp"     // Event communication
#include "Engine/Resource/ResourceHandle.hpp" // Asset management
```

### Game Configuration
- **File**: `Run/Data/GameConfig.xml`
- **Settings**: Screen resolution, window behavior
- **Loading**: Via NamedStrings blackboard system

### Build Preferences
- **File**: `EngineBuildPreferences.hpp`
- **Purpose**: Compile-time engine feature toggles
- **Usage**: Controls which engine features are compiled

## Data Models

### Core Game Entities

#### Player System
- **File**: `Player.hpp/.cpp`
- **Purpose**: Player character controller and state management
- **Integration**: Managed by Game class, uses Entity base

#### Entity System  
- **File**: `Entity.hpp/.cpp`
- **Purpose**: Base class for all interactive game objects
- **Pattern**: Entity-component architecture foundation

#### Prop System
- **File**: `Prop.hpp/.cpp` 
- **Purpose**: Static and dynamic game objects (cubes, spheres, environment)
- **Examples**: Tutorial boxes, geometric shapes, grid objects

### Game State Management
```cpp
enum class eGameState : uint8_t
{
    ATTRACT,  // Attraction/demo mode
    GAME      // Active gameplay
};
```

### Game Object Instances (from Game.hpp)
```cpp
Camera*    m_screenCamera;  // Main viewport camera
Player*    m_player;        // Player character
Prop*      m_firstCube;     // Example game props
Prop*      m_secondCube;
Prop*      m_sphere;
Prop*      m_grid;
Clock*     m_gameClock;     // Game timing system
```

## Testing and Quality

### Current Testing Approach
- **Integration Testing**: Through main executable gameplay
- **Manual Testing**: Interactive debugging via console system
- **Visual Testing**: Debug rendering systems for spatial debugging

### Debug Systems
- **Debug Rendering**: Custom 2D debug drawing functions
  - `DebugDrawRing()`, `DebugDrawLine()`, `DebugDrawGlowCircle()`
- **Console System**: Runtime command execution and variable inspection
- **Developer Camera**: Separate camera for development/debugging

### Memory Management
- **Custom Macros**: `GAME_SAFE_RELEASE()` for safe pointer cleanup
- **RAII Patterns**: Constructor/destructor resource management
- **Global Cleanup**: Systematic shutdown in App destructor

## Subsystem Architecture

### Console Subsystem
- **Path**: `Subsystem/Console/`
- **Purpose**: Debug console and command system
- **Interface**: Basic lifecycle methods (StartUp, Update, Render, ShutDown)

### Light Subsystem
- **Path**: `Subsystem/Light/`
- **Purpose**: Game-specific lighting management
- **Features**: Light addition/removal, constant buffer management
- **Integration**: Separate from engine's core lighting for game-specific effects

### Widget Subsystem
- **Path**: `Subsystem/Widget/`
- **Purpose**: UI and HUD element management
- **Status**: Basic structure defined

## FAQ

### Q: How does the Game module relate to the Engine?
A: The Game module builds on top of the Engine static library, providing game-specific implementations while the Engine provides core systems like rendering, audio, and input.

### Q: What's the difference between Entity and Prop?
A: Entity appears to be the base class for all interactive objects, while Prop is a specific implementation for static/dynamic game objects like cubes and environment pieces.

### Q: How is game configuration handled?
A: Configuration is loaded from `Run/Data/GameConfig.xml` into the engine's NamedStrings blackboard system, allowing XML-driven settings.

### Q: What rendering capabilities are available?
A: The game has access to the full Engine renderer including 3D models, shaders, textures, lighting, and debug rendering utilities.

## Related File List

### Core Framework
- `Framework/Main_Windows.cpp` - Application entry point
- `Framework/App.hpp/.cpp` - Main application class
- `Framework/GameCommon.hpp/.cpp` - Shared game utilities and globals

### Game Logic
- `Game.hpp/.cpp` - Core game controller
- `Player.hpp/.cpp` - Player character system  
- `Entity.hpp/.cpp` - Base entity class
- `Prop.hpp/.cpp` - Game object implementation

### Configuration
- `EngineBuildPreferences.hpp` - Build-time engine configuration
- `Game.vcxproj` - Visual Studio project file

### Subsystems
- `Subsystem/Console/ConsoleSubsystem.hpp/.cpp`
- `Subsystem/Light/LightSubsystem.hpp/.cpp` 
- `Subsystem/Widget/WidgetSubsystem.hpp/.cpp`

---

*Module documentation generated: 2025-09-22*