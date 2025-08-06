//----------------------------------------------------------------------------------------------------
// Prop.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include <vector>

#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/VertexUtils.hpp"
#include "Game/Entity.hpp"

//----------------------------------------------------------------------------------------------------
class Texture;
struct Vertex_PCU;

//----------------------------------------------------------------------------------------------------
class Prop : public Entity
{
public:
    Prop(Game* owner, Texture const* texture = nullptr);

    void Update(float deltaSeconds) override;
    void Render() const override;
    void InitializeLocalVertsForCube();
    void InitializeLocalVertsForSphere();
    void InitializeLocalVertsForGrid();
    void InitializeLocalVertsForCylinder();
    void InitializeLocalVertsForWorldCoordinateArrows();
    void InitializeLocalVertsForText2D();

private:
    std::vector<Vertex_PCU> m_vertexes;
    Texture const* m_texture = nullptr;
};
