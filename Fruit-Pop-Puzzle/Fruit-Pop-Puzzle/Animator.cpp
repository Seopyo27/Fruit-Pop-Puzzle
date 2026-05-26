#include "Animator.h"
#include "Gameobject.h"
#include "Sprite.h"

namespace EHEngine
{
	Animator::Animator(GameObject* pOwner, GameApp* pGame) : Component(pOwner, pGame)
	{
		m_sprite = pOwner->GetComponent<Sprite>();
		m_isEnabled = false;
	}

	void Animator::Update(float dT)
	{
		if (m_sprite == nullptr) return;
		if (m_SpriteCount == 0) return;

		m_Timer += dT;
		if (m_Timer < m_frameTime) return;
		m_Timer -= m_frameTime;

		// 종료조건 확인
		if (m_spriteIndex >= m_SpriteCount)
		{
			// 애니메이션 반복
			if (m_isLoop)
			{
				m_spriteIndex = 0;
			}

			// 애니메이션 종료
			else
			{
				Stop();
				return;
			}
		}

		// 스프라이트에 정보 전송
		m_sprite->SetOffsetX(m_frameXY[m_spriteIndex].x);
		m_sprite->SetOffsetY(m_frameXY[m_spriteIndex].y);
		m_spriteIndex++;
	}

	void Animator::Play()
	{
		// 연동된 스프라이트가 없다면 리턴
		if (m_sprite == nullptr) return;
		// 애니메이션 클립 데이터가 없다면 시작되지 않음.
		if (m_frameXY.empty()) return;

		if (!m_isEnabled) // 이미 play()한상태에서 또 누르면 원본 비트맵 유실되기 때문에 방지
		{
			// 원본 스프라이트 저장
			m_pSpriteBitmapInfo = m_sprite->GetBitmapInfo();
		}

		

		// 스프라이트 시트 전송
		m_sprite->SetBitmapInfo(m_pSheetBitmapInfo, m_layout.spriteWidth, m_layout.spriteHeight, m_frameXY[0].x, m_frameXY[0].y);

		// 스프라이트 인덱스 초기화
		m_spriteIndex = 1;

		// 타이머 초기화
		m_Timer = 0.0f;

		// 스프라이트에 전송 시작
		m_isEnabled = true;
	}

	void Animator::Stop()
	{
		if (m_sprite == nullptr) return;

		// 원본 스프라이트 시트 복구
		m_sprite->SetBitmapInfo(m_pSpriteBitmapInfo);
		// 스프라이트에 전송 중지
		m_isEnabled = false;
	}

	void Animator::InitAnimationClip(renderHelp::BitmapInfo* pSheetBitmapInfo, const SpriteSheetLayout& layout)
	{
		// 스프라이트 시트를 찾을 수 없다면 리턴
		if (pSheetBitmapInfo == nullptr) return;

		// 스프라이트 시트 등록
		m_pSheetBitmapInfo = pSheetBitmapInfo;

		// 스프라이트 시트 레이아웃 설정
		m_layout = layout;

		// 애니메이션 클립 데이터 저장 시작
		
		// 스프라이트 시작 위치 벡터 초기화
		m_frameXY.clear();

		// 스프라이트 개수만큼 벡터 공간 할당
		m_frameXY.reserve(m_layout.spriteCount);

		// 스프라이트 시작위치 각각 저장
		int count = 0;

		for (int row = 0; row < m_layout.RowCount && count < m_layout.spriteCount; row++)
		{
			for (int col = 0; col < m_layout.ColumnCount && count < m_layout.spriteCount; col++)
			{
				int x = m_layout.offsetX + col * (m_layout.spriteWidth + m_layout.gapX);
				int y = m_layout.offsetY + row * (m_layout.spriteHeight + m_layout.gapY);

				m_frameXY.push_back({ x, y });

				count++;
			}
		}

		m_SpriteCount = static_cast<int>(m_frameXY.size());
		
	}



}