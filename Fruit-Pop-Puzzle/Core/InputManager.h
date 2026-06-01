#pragma once
#include "Types/Types.h"
#include "External/NzWndBase.h"

namespace EHEngine
{
	enum class KeyState
	{
		NONE,
		MOUSE_LEFT_CLICK,
		MOUSE_MOVING
	};

	struct Input
	{
		KeyState key = KeyState::NONE;
		MousePos mousePos = { 0, 0 };
	};

	class InputManager
	{
	public:
		void ObserveInput(const MSG& msg);
		void InitInput();
		const Input& GetInput() { return m_input; }
	private:
		Input m_input = { KeyState::NONE, {0, 0} };

	};
}


