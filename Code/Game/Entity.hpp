//----------------------------------------------------------------------------------------------------
// Entity.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/Vec3.hpp"

//----------------------------------------------------------------------------------------------------
class Game;

//----------------------------------------------------------------------------------------------------
class Entity
{
public:
    explicit Entity(Game* owner);
    virtual  ~Entity();

    virtual void  Update(float deltaSeconds) = 0;
    virtual void  Render() const = 0;
    virtual Mat44 GetModelToWorldTransform() const;

    Game*       m_game            = nullptr;
    Vec3        m_position        = Vec3::ZERO;
    Vec3        m_velocity        = Vec3::ZERO;
    EulerAngles m_orientation     = EulerAngles::ZERO;
    EulerAngles m_angularVelocity = EulerAngles::ZERO;
    Rgba8       m_color           = Rgba8::WHITE;
};
