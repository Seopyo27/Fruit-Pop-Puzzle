#pragma once
#include "External/INC_Windows.h"

class NzWndBase;

namespace EHEngine
{
	class RenderContext
	{
	public:
		~RenderContext();
		
		void InitRenderContext(NzWndBase* window);
		HDC GetFrontDC() { return m_hFrontDC; }
		HDC GetBackDC() { return m_hBackDC; }
		
	private:
		void FinalizeRenderContext();

	private:
		NzWndBase* m_window = nullptr;
		HDC m_hFrontDC = nullptr;
		HDC m_hBackDC = nullptr;
		HBITMAP m_hBackBitmap = nullptr;
		HBITMAP m_hDefaultBitmap = nullptr;
	};
}


