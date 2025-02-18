//----------------------------------------------------------------------------------------------------
// Entity.hpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#pragma once
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/Vec3.hpp"

//----------------------------------------------------------------------------------------------------
class Game;

//----------------------------------------------------------------------------------------------------
class Entity
{
public:
    Entity(Game* owner);
    virtual ~Entity() = default;

    virtual void Update(float deltaSeconds) = 0;
    virtual void Render() const = 0;

    Game*       m_game = nullptr;
    Vec3        m_position;
    Vec3        m_velocity;
    EulerAngles m_orientation;
    EulerAngles m_angularVelocity;
};
