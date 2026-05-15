#pragma once
#include "Component.h"
#include "RenderHelp.h"

class Animation : Component
{
	using BitmapInfo = renderHelp::BitmapInfo;

public:
	void UpdateFrame(float deltaTime);
	void SetBitmapInfo(BitmapInfo* bitmapInfo)
	{
		m_pBitmapInfo = bitmapInfo;
	}

private:
	struct FrameFPos
	{
		int x;
		int y;
	};

	FrameFPos m_frameXY[10] = { { 0, 0 }, };
	int m_frameIndex = 0;
	int m_frameCount = 1;
	float m_frameTime = 0.0f;
	float m_frameDuration = 35.0f; // 임의 설정
	BitmapInfo* m_pBitmapInfo = nullptr;
};