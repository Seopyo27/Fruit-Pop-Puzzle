#pragma once
#include <unordered_map>
#include "External/RenderHelp.h"

namespace EHEngine
{
	class ResourceManager
	{
	public:
		void LoadResource();
		renderHelp::BitmapInfo* GetBitmapInfo(std::string bitMapName);

	private:
		void RM_LoadBitmap();
		void AddBitmapInfo(std::string bitMapName, LPCWSTR filename);
		
	private:
		std::unordered_map<std::string, renderHelp::BitmapInfo*> m_pBitmapInfoTable;
	};
}


