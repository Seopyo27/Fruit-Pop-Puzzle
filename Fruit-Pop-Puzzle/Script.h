#pragma once
#include "Component.h"
class GameObject;

// !! gameObject 사라졌을때 대처 해야됨

class Script : public Component
{
public:
	Script(NewGameObject* pOwner) : Component(pOwner) {}
	virtual ~Script() {}
	virtual void Start() {}
	virtual void Update(float dT) {}
	virtual void FixedUpdate() {}
	virtual void End() {}
};