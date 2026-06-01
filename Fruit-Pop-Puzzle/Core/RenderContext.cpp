#include "RenderContext.h"
#include "External/NzWndBase.h"

namespace EHEngine
{
	RenderContext::~RenderContext()
	{   
        FinalizeRenderContext();
	}

    void RenderContext::InitRenderContext(NzWndBase* window)
    {
        m_window = window;

        if (m_window == nullptr || m_window->GetHandle() == nullptr) return;

        m_hFrontDC = ::GetDC(m_window->GetHandle());
        if (!m_hFrontDC) return;

        m_hBackDC = ::CreateCompatibleDC(m_hFrontDC);
        if (!m_hBackDC) return;

        m_hBackBitmap = ::CreateCompatibleBitmap(m_hFrontDC, m_window->GetWidth(), m_window->GetHeight());
        if (!m_hBackBitmap) return;

        m_hDefaultBitmap = (HBITMAP)::SelectObject(m_hBackDC, m_hBackBitmap);
    }

    void RenderContext::FinalizeRenderContext()
    {
        if (m_hBackDC && m_hDefaultBitmap)
        {
            ::SelectObject(m_hBackDC, m_hDefaultBitmap);
            m_hDefaultBitmap = nullptr;
        }

        if (m_hBackBitmap)
        {
            ::DeleteObject(m_hBackBitmap);
            m_hBackBitmap = nullptr;
        }

        if (m_hBackDC)
        {
            ::DeleteDC(m_hBackDC);
            m_hBackDC = nullptr;
        }

        if (m_hFrontDC && m_window && m_window->GetHandle())
        {
            ::ReleaseDC(m_window->GetHandle(), m_hFrontDC);
            m_hFrontDC = nullptr;
        }
    }
}