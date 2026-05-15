#pragma once
#include <Windows.h>
#include "Component.h"
#include "RenderHelp.h"

class NewGameObject;
class Transform;

class Sprite : public Component
{
	using BitmapInfo = renderHelp::BitmapInfo;

public:
	Sprite(NewGameObject* pOwner);
	
	void SetBitmapInfo(BitmapInfo* bitmapInfo);

	void Render(HDC hdc) override {}

private:
	Transform* m_pTransform = nullptr;
	BitmapInfo* m_pBitmapInfo = nullptr;
	int m_frameWidth = 0;
	int m_frameHeight = 0;
}