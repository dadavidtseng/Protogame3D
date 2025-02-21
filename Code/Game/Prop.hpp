//----------------------------------------------------------------------------------------------------
// Prop.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include <vector>

#include "Engine/Core/Rgba8.hpp"
#include "Game/Entity.hpp"

//----------------------------------------------------------------------------------------------------
class Texture;
struct Vertex_PCU;

//----------------------------------------------------------------------------------------------------
class Prop : public Entity
{
public:
    Prop(Game* owner);

    void Update(float deltaSeconds) override;
    void Render() const override;
    void InitializeLocalVerts();

    std::vector<Vertex_PCU> m_vertexes;
    std::vector<Vertex_PCU> m_vertexesClone;
    // Rgba8                   m_color   = Rgba8::WHITE;
    Texture*                m_texture = nullptr;
};
