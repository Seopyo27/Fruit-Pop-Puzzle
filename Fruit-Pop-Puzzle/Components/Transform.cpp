#include "Transform.h"
#include "Core/GameObject.h"

namespace EHEngine
{
    learning::Vector2f Transform::GetWorldPosition()
    {
        if (nullptr == m_ownerObj->GetParent())
        {
            return m_pos;
        }

        return m_ownerObj
            ->GetParent()
            ->GetComponent<Transform>()
            ->GetWorldPosition()
            + m_pos;
    }
}
