#include "ResourceManager.h"

namespace EHEngine
{
	void ResourceManager::LoadResource()
	{
		RM_LoadBitmap();
	}

	renderHelp::BitmapInfo* ResourceManager::GetBitmapInfo(std::string bitMapName)
	{
		auto it = m_pBitmapInfoTable.find(bitMapName);
		if (it == m_pBitmapInfoTable.end()) return nullptr;
		return it->second;
	}

	void ResourceManager::RM_LoadBitmap()
	{
		// 보드
		AddBitmapInfo("Board", L"./Resource/Board.png");

		// 스코어 보드
		AddBitmapInfo("ScoreBoard", L"./Resource/ScoreBoard.png");

		// 과일
		AddBitmapInfo("AppleSpriteSheet", L"./Resource/AppleSpriteSheet.png");
		AddBitmapInfo("BananaSpriteSheet", L"./Resource/BananaSpriteSheet.png");
		AddBitmapInfo("GrapesSpriteSheet", L"./Resource/GrapesSpriteSheet.png");
		AddBitmapInfo("WaterMelonSpriteSheet", L"./Resource/WaterMelonSpriteSheet.png");

		// 마우스 포인터
		AddBitmapInfo("CurrentPointer", L"./Resource/CurrentPointer.png");
		AddBitmapInfo("SelectedPointer", L"./Resource/SelectedPointer.png");
		
		// 콤보 이펙트
		AddBitmapInfo("Monkey", L"./Resource/Monkey.png");
		AddBitmapInfo("Parrot", L"./Resource/Parrot.png");
		AddBitmapInfo("Chameleon", L"./Resource/Chameleon.png");
		AddBitmapInfo("Hippo", L"./Resource/Hippo.png");
		AddBitmapInfo("Animals", L"./Resource/Animals.png");

		// 콤보 텍스트
		AddBitmapInfo("Good", L"./Resource/Good.png");
		AddBitmapInfo("Great", L"./Resource/Great.png");
		AddBitmapInfo("Awesome", L"./Resource/Awesome.png");
		AddBitmapInfo("Fantastic", L"./Resource/Fantastic.png");
		AddBitmapInfo("Fever", L"./Resource/Fever.png");
	}

	void ResourceManager::AddBitmapInfo(std::string bitMapName, LPCWSTR filename)
	{
		renderHelp::BitmapInfo* bitMapInfo = renderHelp::CreateBitmapInfo(filename);
		if (bitMapInfo == nullptr) return;
		m_pBitmapInfoTable[bitMapName] = bitMapInfo;
	}
}