#pragma once
#include "Core/Component.h"
#include "External/RenderHelp.h"
#include "External/Utillity.h"
#include <vector>

namespace EHEngine
{
	class SpriteRenderer;

	struct SpriteSheetLayout
	{
		int sheetWidth;
		int sheetHeight;
		int offsetX;
		int offsetY;
		int spriteWidth;
		int spriteHeight;
		int gapX;
		int gapY;
		int RowCount;
		int ColumnCount;
		int spriteCount;
	};

	class Animator : public Component
	{
	public:
		Animator(GameObject* ownerObj, Scene* scene);

		void InitAnimationClip(renderHelp::BitmapInfo* pSheetBitmapInfo, const SpriteSheetLayout& layout);

		void Update(float deltaTime) override;

		void SetFrameTime(float t) { m_frameTime = t; }
		void SetIsLoop(bool b) { m_isLoop = b; }

		void Play();
		void Stop();

	private:
		SpriteSheetLayout m_layout; // 스프라이트 시트 레이아웃
		renderHelp::BitmapInfo* m_pSheetBitmapInfo = nullptr; // 스프라이트 시트 저장

		SpriteRenderer* m_sprite = nullptr;   // 연동된 스프라이트 컴포넌트
		renderHelp::BitmapInfo* m_pSpriteBitmapInfo = nullptr; // 연동된 스프라이트의 원본 스프라이트

		// 애니메이션 클립 데이터
		int m_SpriteCount = 0;
		std::vector<learning::Pos> m_frameXY;
		int m_spriteIndex = 0;

		// 타이머
		float m_Timer = 0;

		// 옵션
		float m_frameTime = 30.0f;
		bool m_isLoop = false;
	};
}
