//----------------------------------------------------------------------------------------------------
// Entity.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Entity.hpp"

//----------------------------------------------------------------------------------------------------
Entity::Entity(Game* owner)
    : m_game(owner)
{
}

//----------------------------------------------------------------------------------------------------
Mat44 Entity::GetModelToWorldTransform() const
{
    Mat44 m2w;

    m2w.AppendZRotation(m_orientation.m_yawDegrees);
    m2w.AppendYRotation(m_orientation.m_pitchDegrees);
    m2w.AppendXRotation(m_orientation.m_rollDegrees);

    m2w.AppendTranslation3D(m_position);

    return m2w;
}
