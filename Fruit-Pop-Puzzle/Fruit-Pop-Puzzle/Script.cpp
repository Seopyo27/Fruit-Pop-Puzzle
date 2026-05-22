#include "Script.h"
#include "GameApp.h"
#include "GameObject.h"

namespace EHEngine
{
    GameObject* Script::Instantiate(const std::string& name, GameObject* prefab)
    {
        if (m_pGame == nullptr || prefab == nullptr) return nullptr;

        GameApp* scene = m_pOwnerObj->GetScene();

        // 새로운 오브젝트 복사 생성
        GameObject* newObj = new GameObject(scene);

        // 메인 게임의 테이블에 등록
        if (scene->AddGameObject(newObj))
        {
            return newObj;
        }

        // 등록 실패 시 메모리 해제
        delete newObj;
        return nullptr;
    }


}
