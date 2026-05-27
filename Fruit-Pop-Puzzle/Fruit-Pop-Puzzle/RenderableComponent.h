#pragma once
#include "Component.h"

namespace EHEngine
{
	class RenderableComponent : public Component
	{
	public:
		RenderableComponent(GameObject* pOwner, GameApp* pGame) : Component(pOwner, pGame) {}

		int GetOrderInLayer() { return m_orderInLayer; }
		void SetOrderInLayer(int order) { m_orderInLayer = order; }

	private:
		int m_orderInLayer = 0;
		bool m_isVisible = true;
	};
}


