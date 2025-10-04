//----------------------------------------------------------------------------------------------------
// Entity.cpp
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
#include "Game/Gameplay/Entity.hpp"

//----------------------------------------------------------------------------------------------------
Entity::Entity(Game* owner)
    : m_game(owner)
{
}

//----------------------------------------------------------------------------------------------------
Entity::~Entity()
{
}

//----------------------------------------------------------------------------------------------------
Mat44 Entity::GetModelToWorldTransform() const
{
    Mat44 m2w;

    m2w.SetTranslation3D(m_position);

    m2w.AppendZRotation(m_orientation.m_yawDegrees);
    m2w.AppendYRotation(m_orientation.m_pitchDegrees);
    m2w.AppendXRotation(m_orientation.m_rollDegrees);

    // m2w.Append(m_orientation.GetAsMatrix_IFwd_JLeft_KUp());

    return m2w;
}
