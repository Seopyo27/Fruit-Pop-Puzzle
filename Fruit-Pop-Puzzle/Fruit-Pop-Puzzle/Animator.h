#pragma once
#include "Component.h"
#include "RenderHelp.h"
#include "Utillity.h"
#include <vector>

namespace EHEngine
{
	// 스프라이트 필수 컴포넌트
	class Sprite;
	
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
		Animator(GameObject* pOwner, GameApp* pGame);

		void InitAnimationClip(renderHelp::BitmapInfo* pSheetBitmapInfo, const SpriteSheetLayout& layout);

		void Update(float dT) override;

		void Play();
		void Stop();

	private:
		// 연동된 스프라이트 컴포넌트
		Sprite* m_sprite = nullptr;

		// 스프라이트 시트 저장
		renderHelp::BitmapInfo* m_pSheetBitmapInfo = nullptr;
		// 애니메이션이 끝나면 원래 스프라이트로 돌려놓는 용도
		renderHelp::BitmapInfo* m_pSpriteBitmapInfo = nullptr;
		// 스프라이트 시트 레이아웃
		SpriteSheetLayout m_layout;

		// 애니메이션 클립 데이터
		int m_SpriteCount = 0;
		std::vector<learning::Pos> m_frameXY;
		int m_spriteIndex = 0;

		// 타이머
		float Timer = 0;

		// 옵션
		float m_frameTime = 0.033f; 
		bool isLoop = false;


	};
}


