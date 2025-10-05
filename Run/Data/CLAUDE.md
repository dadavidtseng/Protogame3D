[Root Directory](../../../CLAUDE.md) > [Run](../../) > **Data**

# Assets Module Documentation

## Changelog

**2025-09-22**: Initial module documentation - Game asset organization and pipeline

---

## Module Responsibilities

The Data module contains all game assets and runtime configuration:

- **3D Models**: Game objects and environment geometry (OBJ, FBX formats)
- **Textures**: Diffuse, normal, specular maps and UI graphics  
- **Shaders**: HLSL rendering shaders for different material types
- **Audio**: Sound effects and music files
- **Fonts**: Bitmap fonts for text rendering
- **Configuration**: Game settings and runtime parameters

## Asset Organization

### Models Directory (`Models/`)

#### Geometric Primitives (`Models/Cube/`)
- **Cube_v.obj** - Vertex-only cube
- **Cube_vi.obj** - Cube with vertex indices  
- **Cube_vn.obj** - Cube with vertex normals
- **Cube_vni.obj** - Cube with vertex normals and indices
- **Purpose**: Basic geometric shapes for testing and gameplay

#### Character Assets (`Models/Woman/`)
- **Woman.obj** - 3D character model
- **Woman_Diffuse.png** - Character texture (albedo)
- **Woman_Normal.png** - Normal map for surface detail
- **Purpose**: Example character asset with complete material pipeline

#### Tutorial Assets (`Models/TutorialBox_Phong/`)
- **Tutorial_Box.obj** - Example box model
- **Tutorial_Box.FBX** - Alternative format
- **Tutorial_Box_Diffuse.tga** - Color texture
- **Tutorial_Box_Normal.tga** - Normal mapping
- **Tutorial_Box_SpecGlossEmit.tga** - Specular/gloss/emission maps
- **Purpose**: Complete Blinn-Phong material demonstration

### Shader Directory (`Shaders/`)

#### Core Shaders
- **Default.hlsl** - Basic vertex/pixel shader for simple rendering
- **BlinnPhong.hlsl** - Complete Blinn-Phong lighting model  
- **Bloom.hlsl** - Post-processing bloom effect shader
- **Purpose**: Fundamental rendering techniques and post-processing

### Audio Directory (`Audio/`)
- **TestSound.mp3** - Example audio file for testing audio system integration
- **Purpose**: Sound effect and music asset storage

### Font Directory (`Fonts/`)
- **SquirrelFixedFont.png** - Bitmap font texture for fixed-width text
- **DaemonFont.png** - Alternative bitmap font texture
- **Purpose**: Text rendering resources for UI and debug output

### Images Directory (`Images/`)
- **TestUV.png** - UV mapping test texture for shader and model validation
- **Purpose**: Utility textures and UI graphics

## Asset Pipeline Integration

### Resource Loading
Assets are loaded through the Engine's ResourceSubsystem using type-safe ResourceHandle system:
```cpp
ResourceHandle<TextureResource> texture = resourceSystem->LoadResource<TextureResource>("Data/Images/TestUV.png");
ResourceHandle<ModelResource> model = resourceSystem->LoadResource<ModelResource>("Data/Models/Cube/Cube_vni.obj");
```

### Supported Formats

#### 3D Models
- **OBJ**: Primary model format with full material support
- **FBX**: Alternative format for complex models
- **Features**: Vertices, normals, UV coordinates, indices

#### Textures  
- **PNG**: Lossless images for UI and alpha textures
- **TGA**: High-quality textures with optional compression
- **Support**: Diffuse, normal, specular, emission maps

#### Shaders
- **HLSL**: DirectX 11 High Level Shader Language
- **Compilation**: Runtime shader compilation and error handling
- **Features**: Vertex, pixel, and geometry shader support

#### Audio
- **MP3**: Compressed audio for music and sound effects
- **Integration**: FMOD audio engine for 3D positional audio

#### Fonts
- **PNG**: Bitmap font textures with character mapping
- **Usage**: Fixed-width and proportional font rendering

## Configuration Management

### Game Configuration (`GameConfig.xml`)
```xml
<GameConfig>
    <WindowClose>false</WindowClose>
    <screenSizeX>1600</screenSizeX>
    <screenSizeY>800</screenSizeY>
    <screenCenterX>800</screenCenterX>
    <screenCenterY>400</screenCenterY>
</GameConfig>
```

#### Configuration Parameters
- **Window Settings**: Size, position, close behavior
- **Screen Configuration**: Resolution and centering
- **Loading**: Parsed via TinyXML2 into NamedStrings blackboard

## Asset Creation Guidelines

### 3D Model Standards
- **Format**: Prefer OBJ for simplicity, FBX for complex models
- **Coordinate System**: Right-handed coordinate system
- **UV Mapping**: 0-1 UV coordinates for texture mapping
- **Normals**: Include vertex normals for proper lighting
- **Scale**: Use consistent world units across all models

### Texture Guidelines  
- **Resolution**: Power-of-2 dimensions for optimal GPU performance
- **Formats**: PNG for UI/alpha, TGA for high-quality textures
- **Naming Convention**: `ModelName_MapType.extension` (e.g., `Tutorial_Box_Diffuse.tga`)
- **Map Types**: Diffuse, Normal, Specular, Emission

### Shader Development
- **HLSL Version**: Target DirectX 11 feature level
- **Entry Points**: Standard `VertexMain` and `PixelMain` functions
- **Constants**: Use constant buffers for shader parameters
- **Testing**: Include fallback paths for shader compilation errors

### Audio Integration
- **Format**: MP3 for compressed audio, WAV for uncompressed
- **Naming**: Descriptive names indicating usage context
- **3D Audio**: Consider positional audio requirements for spatial sounds

## Development Workflow

### Asset Addition Process
1. **Place Asset**: Add file to appropriate Data subdirectory
2. **Update Code**: Reference asset in C++ code via ResourceSubsystem
3. **Test Loading**: Verify asset loads correctly through debug console
4. **Performance**: Check memory usage and loading times

### Asset Validation
- **Model Validation**: Verify UV coordinates, normals, vertex counts
- **Texture Validation**: Check resolution, format, transparency
- **Shader Validation**: Compile-test shaders for syntax errors
- **Audio Validation**: Test playback through FMOD integration

### Version Control
- **Large Assets**: Consider Git LFS for large binary files
- **Organization**: Maintain consistent directory structure
- **Documentation**: Document asset sources and usage requirements

## Performance Considerations

### Memory Management
- **Resource Caching**: Assets cached by ResourceSubsystem to prevent duplicate loading
- **Streaming**: Large assets loaded asynchronously to prevent frame drops
- **Cleanup**: Automatic cleanup when ResourceHandles go out of scope

### Loading Optimization
- **Preloading**: Critical assets loaded during game initialization
- **Async Loading**: Background loading for non-critical assets
- **Compression**: Appropriate compression for different asset types

### Runtime Efficiency
- **Texture Binding**: Minimize texture state changes during rendering
- **Model Batching**: Group similar models for efficient rendering
- **Shader Switching**: Minimize shader program changes per frame

## FAQ

### Q: How do I add a new 3D model to the game?
A: Place the OBJ file in an appropriate Models subdirectory, then load it using `ResourceSubsystem->LoadResource<ModelResource>("Data/Models/YourModel/model.obj")`.

### Q: What texture formats are supported?
A: PNG and TGA formats are supported through the STB image library. PNG is preferred for UI elements, TGA for high-quality textures.

### Q: How do I create custom shaders?
A: Write HLSL shaders in the Shaders directory, then load them using the ShaderResource system. The engine will compile them at runtime.

### Q: Can I use compressed audio formats?
A: Yes, MP3 is supported through FMOD integration. The audio system handles format conversion automatically.

### Q: How are fonts rendered?
A: Fonts use bitmap textures with character mapping. Place PNG font textures in the Fonts directory and load through the FontResource system.

## Related File List

### Configuration
- `GameConfig.xml` - Main game configuration file

### 3D Models
- `Models/Cube/` - Basic geometric primitives
- `Models/Woman/` - Character model example  
- `Models/TutorialBox_Phong/` - Complete material example

### Shaders
- `Shaders/Default.hlsl` - Basic rendering shader
- `Shaders/BlinnPhong.hlsl` - Phong lighting model
- `Shaders/Bloom.hlsl` - Post-processing effect

### Textures
- `Images/TestUV.png` - UV mapping test texture
- Various model textures in Models subdirectories

### Audio
- `Audio/TestSound.mp3` - Example sound file

### Fonts
- `Fonts/SquirrelFixedFont.png` - Fixed-width bitmap font
- `Fonts/DaemonFont.png` - Alternative bitmap font

---

*Module documentation generated: 2025-09-22*