#include "RenderableComponent.h"
#include "Core/Scene.h"

namespace EHEngine
{
	RenderableComponent::RenderableComponent(GameObject* Owner, Scene* scene) : Component(Owner, scene)
	{
		m_scene->AddRenderableComponent(this);
	}

	RenderableComponent::~RenderableComponent()
	{
		m_scene->RemoveRenderableComponent(this);
	}
}