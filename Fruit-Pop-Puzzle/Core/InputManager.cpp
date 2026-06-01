#include "InputManager.h"


namespace EHEngine
{
	void InputManager::ObserveInput(const MSG& msg)
	{
		// 입력 처리
		if (msg.message == WM_LBUTTONDOWN)
		{
			m_input.key = KeyState::MOUSE_LEFT_CLICK;
			m_input.mousePos = { LOWORD(msg.lParam), HIWORD(msg.lParam) };
		}
		else if (msg.message == WM_MOUSEMOVE)
		{
			m_input.key = KeyState::MOUSE_MOVING;
			m_input.mousePos = { LOWORD(msg.lParam), HIWORD(msg.lParam) };
		}
	}

	void InputManager::InitInput()
	{
		m_input.key = KeyState::NONE;
	}
}