#pragma once
#include "Component.h"

namespace EHEngine
{
	class RenderableComponent : public Component
	{
	public:
		RenderableComponent(GameObject* Owner, Scene* scene);
		~RenderableComponent() override;

		int GetOrderInLayer() const { return m_orderInLayer; }
		void SetOrderInLayer(int order) { m_orderInLayer = order; }
		bool GetIsVisible() const { return m_isVisible; }
		void SetIsVisible(bool on) { m_isVisible = on; }


	private:
		int m_orderInLayer = 0;  // ·»´õ¸µ ¼ø¼­ (ÀÛÀ» ¼ö·Ï »¡¸® ·»´õ¸µ)
		bool m_isVisible = true; // ·»´õ¸µ ¿©ºÎ
	};
}

