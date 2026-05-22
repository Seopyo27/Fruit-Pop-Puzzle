#pragma once
#include "NzWndBase.h"
#include <unordered_map>
#include "Types.h"

class GameTimer;

namespace renderHelp
{
	class BitmapInfo;
}

namespace EHEngine {
	
	class GameObject;

	enum class Key
	{
		NONE,
		MOUSE_LEFT_CLICK,
		MOUSE_RIGHT_CLICK,
		MOUSE_MOVING
	};

	struct Input
	{
		Key key = Key::NONE;
		MOUSE_POS mousePos = { 0, 0 };
	};
	
	class GameApp : public NzWndBase
	{
	public:
		GameApp() = default;
		~GameApp() override = default;

		bool Initialize();
		void Run();
		void Finalize();

		// 스크립트에서 객채 생성 및 파괴하기 위해
		bool AddGameObject(GameObject* obj);
		GameObject* CreateGameObject();
		bool DeleteGameObject(uint64_t id);

		renderHelp::BitmapInfo* GetBitmapInfo(std::string bitMapName);

		const Input& GetInput() { return m_input; }

	private:
		void LoadResource();
		GameObject* GetGameObject(uint64_t id);

		renderHelp::BitmapInfo* AddBitmapInfo(std::string bitMapName, LPCWSTR filename);
		
		void Update();
		void FixedUpdate();
		void LogicUpdate();

		void Render();
		
		void OnResize(int width, int height) override;
		void OnClose() override;
		void OnMouseMove(int x, int y);
		void OnLButtonDown(int x, int y);
		void OnRButtonDown(int x, int y);

	private:
		//*************************************************
		//입력
		//************************************************* 

		Input m_input = { Key::NONE, {0, 0} };
		MOUSE_POS m_MousePos = { 0, 0 };
		MOUSE_POS m_MousePosPrev = { 0, 0 };


		//*************************************************
		//렌더링, HDC
		//************************************************* 
		HDC m_hFrontDC = nullptr;
		HDC m_hBackDC = nullptr;
		HBITMAP m_hBackBitmap = nullptr;
		HBITMAP m_hDefaultBitmap = nullptr;


		//*************************************************
		//타이머
		//************************************************* 
		GameTimer* m_pGameTimer = nullptr;
		float m_fDeltaTime = 0.0f;
		float m_fFrameCount = 0.0f;
		float m_fDelayTime = 0.0f;


		//*************************************************
		//자원관리
		//************************************************* 
		// 오브젝트
		std::unordered_map<uint64_t, GameObject*> m_GameObjPtrTable;
		uint64_t m_GameObjectCount = 0;
		// 비트맵
		std::unordered_map<std::string, renderHelp::BitmapInfo*> m_pBitmapInfoTable;
	};



}


