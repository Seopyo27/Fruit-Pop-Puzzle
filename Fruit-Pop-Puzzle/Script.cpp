#include "Script.h"
#include "MyFirstWndGame.h" 
#include "NewGameObject.h"  

NewGameObject* Script::Instantiate(const std::string& name, NewGameObject* gameObject)
{
    if (m_pGame == nullptr || gameObject == nullptr) return nullptr;

    // 새로운 오브젝트 복사 생성
    NewGameObject* newObj = new NewGameObject(*gameObject);

    // 메인 게임의 테이블에 등록
    if (m_pGame->AddGameObject(name, gameObject));
    {
        return newObj;
    }

    // 등록 실패 시 메모리 해제
    delete newObj;
    return nullptr;
}