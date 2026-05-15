#include "INC_Windows.h"
#include "MyFirstWndGame.h"
#include "GameTimer.h"
#include "Collider.h"
#include "GameObject.h"
#include "RenderHelp.h"
#include "Board.h"
#include <iostream>
#include <assert.h>
#include "NewGameObject.h"
#include "Transform.h"
#include "Sprite.h"

using namespace learning;

constexpr int MAX_GAME_OBJECT_COUNT = 1000;

bool MyFirstWndGame::Initialize()
{
    m_pGameTimer = new GameTimer();
    m_pGameTimer->Reset();

    const wchar_t* className = L"MyFirstWndGame";
    const wchar_t* windowName = L"MyFirstWndGame";

    if (false == __super::Create(className, windowName, 1024, 720))
    {
        return false;
    }


    RECT rcClient = {};
    GetClientRect(m_hWnd, &rcClient);
    m_width = rcClient.right - rcClient.left;
    m_height = rcClient.bottom - rcClient.top;

    m_hFrontDC = GetDC(m_hWnd);
    m_hBackDC = CreateCompatibleDC(m_hFrontDC);
    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

    m_hDefaultBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////

    // 리소스 로드
    LoadResource();

    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    
    // 게임 보드 객체 생성
    CreateGameObject("Board");
    // 게임 컴포넌트 설정
    NewGameObject* board = GetGameObject("Board");
    // 게임 보드, 트랜스폼 추가
    Transform* boardTransform = board->AddComponent<Transform>();
    boardTransform->SetPosition(512, 360);
    boardTransform->SetWidth(800);
    boardTransform->SetHeight(800);
    // 게임 보드, 스프라이트 추가
    Sprite* boardSprite = board->AddComponent<Sprite>();
    // 비트맵 설정
    boardSprite->SetBitmapInfo(GetBitmapInfo("Board"));

	// 보드 생성
    CreateBoard(800, 800, 94, 95, 6, 6, 102, 93, 6);

    // 과일 비트맵 테이블 보드에 설정
    m_pBoard->SetPFruitBitmapInfoTable(m_pBitmapInfoTable);

    // 과일 초기 전체 생성
    m_pBoard->RefillAllFruit();

    //클릭 포인터 생성
    CreateClickPointer(80, 80);
    

    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////

    for (auto it : m_GameObjectPtrTable)
    {
        NewGameObject* gameObject = it.second;
        Script* s = gameObject->GetComponent<Script>();
        if (s == nullptr) continue;
        s->Start();
    }

    return true;
}

void MyFirstWndGame::Run()
{
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_LBUTTONDOWN)
            {
                MyFirstWndGame::OnLButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else if (msg.message == WM_RBUTTONDOWN)
            {
                MyFirstWndGame::OnRButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else if (msg.message == WM_MOUSEMOVE)
            {
                MyFirstWndGame::OnMouseMove(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            Update();
            Render();
        }
    }
}

void MyFirstWndGame::Finalize()
{
    for (auto it : m_GameObjectPtrTable)
    {
        NewGameObject* gameObject = it.second;
        Script* s = gameObject->GetComponent<Script>();
        if (s == nullptr) continue;
        s->End();
    }

    delete m_pGameTimer;
    m_pGameTimer = nullptr;

    if (m_GameObjectPtrTable)
    {
        for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
        {
            if (m_GameObjectPtrTable[i])
            {
                delete m_GameObjectPtrTable[i];
                m_GameObjectPtrTable[i] = nullptr;
            }
        }
        delete m_GameObjectPtrTable;
    }

    __super::Destroy(); 
}

void MyFirstWndGame::FixedUpdate()
{
    for (auto it : m_GameObjectPtrTable)
    {
        NewGameObject* gameObject = it.second;
        Script* s = gameObject->GetComponent<Script>();
        if (s == nullptr) continue;
        s->FixedUpdate();
    }
}

void MyFirstWndGame::LogicUpdate()
{
    for (auto it : m_GameObjectPtrTable)
    {
        NewGameObject* gameObject = it.second;
        Script* s = gameObject->GetComponent<Script>();
        if (s == nullptr) continue;
        s->Update(m_fDeltaTime);
    }

    LoopPuzzleGame();
}

void MyFirstWndGame::CreateBoard(int boardWidth, int boardHeight, int cellWidth, int cellHeight, int maxRow, int maxCol, int gridOffsetX, int gridOffsetY, int gridGap)
{
    Board* pNewBoard = new Board(ObjectType::BOARD);
    
    pNewBoard->SetName("Board");
    pNewBoard->SetPosition(512, 360);
    pNewBoard->SetSpeed(0.0f);

    pNewBoard->SetWidth(boardWidth);
    pNewBoard->SetHeight(boardHeight);

    pNewBoard->SetBitmapInfo(m_pBoardBitmapInfo);

    BoardLayout layout
    {
        boardWidth,
        boardHeight,
        cellWidth,
        cellHeight,
        maxRow,
        maxCol,
        gridOffsetX,
        gridOffsetY,
        gridGap
    };

   pNewBoard->InitBoard(layout);
  
   m_GameObjectPtrTable[m_GameObjectCount++] = pNewBoard;

   // 보드 인스턴스를 자주 사용하기위해 따로 포인터를 저장
   m_pBoard = pNewBoard;
}

void MyFirstWndGame::CreateClickPointer(int width, int height)
{
    GameObject* pNewClickPointer = new GameObject(ObjectType::POINTER);

    pNewClickPointer->SetName("ClickPointer");
    pNewClickPointer->SetPosition(0, 0);
    pNewClickPointer->SetWidth(width);
    pNewClickPointer->SetHeight(height);

    pNewClickPointer->SetColliderBox(width, height);

    pNewClickPointer->SetVisible(false);

    m_GameObjectPtrTable[m_GameObjectCount++] = pNewClickPointer;

    m_pClickPointer = pNewClickPointer;
}


void MyFirstWndGame::Update()
{
    m_pGameTimer->Tick();

    LogicUpdate();

    m_fDeltaTime = m_pGameTimer->DeltaTimeMS();
    m_fFrameCount += m_fDeltaTime;

    while (m_fFrameCount >= 16.6f)
    {
        FixedUpdate();
        m_fFrameCount -= 16.6f;
    }
}

void MyFirstWndGame::Render()
{
    //Clear the back buffer
    ::PatBlt(m_hBackDC, 0, 0, m_width, m_height, WHITENESS);

    //메모리 DC에 그리기
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Render(m_hBackDC);
        }
    }

    //메모리 DC에 그려진 결과를 실제 DC(m_hFrontDC)로 복사
    BitBlt(m_hFrontDC, 0, 0, m_width, m_height, m_hBackDC, 0, 0, SRCCOPY);
}

void MyFirstWndGame::OnResize(int width, int height)
{
    std::cout << __FUNCTION__ << std::endl;

    learning::SetScreenSize(width, height);

    __super::OnResize(width, height);

    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

    HANDLE hPrevBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

    DeleteObject(hPrevBitmap);
}

void MyFirstWndGame::OnClose()
{
    std::cout << __FUNCTION__ << std::endl;

    SelectObject(m_hBackDC, m_hDefaultBitmap);

    DeleteObject(m_hBackBitmap);
    DeleteDC(m_hBackDC);

    ReleaseDC(m_hWnd, m_hFrontDC);
}

void MyFirstWndGame::OnMouseMove(int x, int y)
{
    /*   std::cout << __FUNCTION__ << std::endl;
       std::cout << "x: " << x << ", y: " << y << std::endl;*/
    m_MousePosPrev = m_MousePos;
    m_MousePos = { x, y };
}

void MyFirstWndGame::OnLButtonDown(int x, int y)
{
    // 디버깅
    PrintBoardClickDebug(x, y);

    m_leftClickedMousePos.x = x;
    m_leftClickedMousePos.y = y;
}

void MyFirstWndGame::OnRButtonDown(int x, int y)
{
    /// 디버깅용
    std::cout << '\n' << '\n' << '\n' << std::flush;
	std::cout << __FUNCTION__ << std::endl;
    std::cout << "[ BOARD ]" << std::endl;
    
    for (int row = 0; row < m_pBoard->GetMaxRow(); row++)
    {
        for (int col = 0; col < m_pBoard->GetMaxCol(); col++)
        {
            Fruit* pFruit = m_pBoard->GetFruitAt({ row, col });
            if (pFruit == nullptr) continue;
            std::cout << static_cast<int>(pFruit->GetFruitType());
        }
        std::cout << std::endl;
    }
}

bool MyFirstWndGame::ConvertScreenToBoard(int mouseX, int mouseY, Pos& boardPos)
{
    if (m_pBoard == nullptr) return false;

    // 화면에서 보드 시작점 계산
    int boardStartX = m_pBoard->GetPosition().x - (m_pBoard->GetBoardWidth() / 2);
    int boardStartY = m_pBoard->GetPosition().y - (m_pBoard->GetBoardHeight() / 2);

    // 보드를 클릭했는가?
    if ((mouseX < boardStartX || boardStartX + m_pBoard->GetBoardWidth() < mouseX) ||
        (mouseY < boardStartY || boardStartY + m_pBoard->GetBoardHeight() < mouseY))
    {
        return false;
    }

    // 보드 좌표로 변환
    boardPos.x = mouseX - boardStartX;
    boardPos.y = mouseY - boardStartY;
    return true;
}



void MyFirstWndGame::PrintBoardClickDebug(int mousePosX, int mousePosY)
{
    std::cout << '\n' << '\n' << '\n' << std::flush;
    std::cout << __FUNCTION__ << std::endl;

    // 윈도우 좌표 마우스 위치 출력
    std::cout << "mousePos: ( " << mousePosX << ", " << mousePosY << " )" << std::endl;

    Pos boardPos = { 0, 0 };
    // 보드를 클릭했으면 mousePos -> boardPos 변환 후 저장
    if (!ConvertScreenToBoard(mousePosX, mousePosY, boardPos))
    {
        std::cout << "보드밖 클릭" << std::endl;
        return;
    }
    // 보드 좌표 클릭 위치 출력
    std::cout << "boardPos: ( " << boardPos.x << ", " << boardPos.y << " )" << std::endl;

    Index cellIndex = { -1, -1 };
    // 셀을 클릭했으면 cellIndex에 인덱스 저장
    if (!(m_pBoard->GetClickedCellIndex(boardPos, cellIndex)))
    {
        std::cout << "보드안 셀 밖 클릭" << std::endl;
        return;
    }
    // 셀 인덱스 출력
    std::cout << "cellIndex: ( " << cellIndex.row << ", " << cellIndex.col << " )" << std::endl;
}


void MyFirstWndGame::LoopPuzzleGame()
{
    switch (m_gameState)
    {
    case GameState::None:
    {
        m_pBoard->InitFruitMatchedList();
        m_leftClickedMousePos = { -1, -1 };
        m_pClickPointer->SetVisible(false);
        m_gameState = GameState::Waiting;

        std::cout << "셀 클릭 준비 완료" << std::endl;
    }
    case GameState::Waiting:
    {
        if (m_leftClickedMousePos.x == -1 || m_leftClickedMousePos.y == -1) return;

        Index cellIndex;
        if (!GetScreenPosToCellIndex(m_leftClickedMousePos.x, m_leftClickedMousePos.y, cellIndex))
        {
            return;
        }
        m_firstSelectedCell = cellIndex;

        if (m_pBoard->GetFruitAt(m_firstSelectedCell) == nullptr)
        {
            std::cout << "첫번째 셀 과일없음!!" << std::endl;
            m_gameState = GameState::None;
            break;
        }

        ShowCellClickPointer(cellIndex);

        m_leftClickedMousePos = { -1, -1 };
        m_gameState = GameState::SelectedFirstCell;

        std::cout << "첫번째 셀 클릭완료" << std::endl;
        break;
    }
    case GameState::SelectedFirstCell:
    {
       
        if (m_leftClickedMousePos.x == -1 || m_leftClickedMousePos.y == -1) return;

        Index cellIndex;
        if (!GetScreenPosToCellIndex(m_leftClickedMousePos.x, m_leftClickedMousePos.y, cellIndex))
        {
            return;
        }
        m_secondSelectedCell = cellIndex;

        if (m_pBoard->GetFruitAt(m_secondSelectedCell) == nullptr)
        {
            std::cout << "두번째 셀 과일없음!!" << std::endl;
            m_gameState = GameState::None;
            break;
        }

        ShowCellClickPointer(cellIndex);

        m_leftClickedMousePos = { -1, -1 };
        m_gameState = GameState::SwapingCell;

        std::cout << "두번째 셀 클릭완료" << std::endl;

        break;
    }
    case GameState::SwapingCell:
    {
        m_pClickPointer->SetVisible(false);
        if (m_pBoard->IsAdjacent(m_firstSelectedCell, m_secondSelectedCell))
        {
            m_pBoard->SwapFruit(m_firstSelectedCell, m_secondSelectedCell);
            std::cout << "셀 스왑 완료" << std::endl;
        }
        else
        {
            std::cout << "셀 스왑 실패" << std::endl;
            m_gameState = GameState::None;
            break;
        }

        m_gameState = GameState::Delay;
        m_reservedGameState = GameState::MatchingFruit;

        break;
    }
    case GameState::MatchingFruit:
    {
        m_pBoard->FindMathes();
        std::cout << "과일 매칭 완료" << std::endl;

        if (m_pBoard->ExistMatchedFruit())
        {
            std::cout << "매칭된 과일 발견" << std::endl;
            m_pBoard->DeleteMatchedFruit();
            std::cout << "매칭된 과일 삭제 완료" << std::endl;

            m_gameState = GameState::Delay;
            m_reservedGameState = GameState::SpawnFruit;
        }
        else
        {
            m_gameState = GameState::None;
        }

        break;
    }
    case GameState::SpawnFruit:
        {
        m_pBoard->FillFruitEmptySpaces();
        m_pBoard->InitFruitMatchedList();
        m_gameState = GameState::Delay;
        m_reservedGameState = GameState::MatchingFruit;
        std::cout << "빈 곳에 과일 채우기 완료" << std::endl;
        break;
        }

    case GameState::Delay:
        {
        // 지연시간 측정
        m_fDelayTime += m_pGameTimer->DeltaTimeMS();

        if (m_fDelayTime >= 1000.0f)
        {
            // 딜레이 후
            m_fDelayTime = 0.0f;

            // 다음 상태로 전환
            m_gameState = m_reservedGameState;
        }


        }

    }
}

bool MyFirstWndGame::GetScreenPosToCellIndex(int screenX, int screenY, Index& cellIndex)
{
    // 스크린 좌표 -> 보드 좌표
    Pos boardPos;
    if (!ConvertScreenToBoard(screenX, screenY, boardPos))
    {
        // 보드 이미지 밖
        return false;
    }

    // 보드 좌표 -> 셀 인덱스
    if (!(m_pBoard->GetClickedCellIndex(boardPos, cellIndex)))
    {
        // 보드 이미지 안이지만 셀 범위 밖
        return false;
    }

    return true;
}

void MyFirstWndGame::ShowCellClickPointer(const Index& cellIndex)
{
    Pos cellCenterPos = m_pBoard->GetCellCenterPos(cellIndex);
    Pos ScreenCellCenterPos = {
        (m_pBoard->GetPosition().x - m_pBoard->GetBoardWidth() / 2) + cellCenterPos.x,
        (m_pBoard->GetPosition().y - m_pBoard->GetBoardHeight() / 2) + cellCenterPos.y };

    if (!m_pClickPointer) return;
    m_pClickPointer->SetPosition(ScreenCellCenterPos.x, ScreenCellCenterPos.y);
    m_pClickPointer->SetVisible(true);
}

void MyFirstWndGame::AddScript(Script* newScript)
{
    m_scripts.push_back(newScript);
}

bool MyFirstWndGame::CreateGameObject(std::string name)
{
    auto it = m_GameObjectPtrTable.find(name);

    // 이미 같은 이름을 가진 게임 오브젝트가 있는지 확인.
    if (it == m_GameObjectPtrTable.end())
    {
        return false;
    }

    // 게임 오브젝트 생성 후 등록
    NewGameObject* gameObject = new NewGameObject();
    m_GameObjectPtrTable[name] = gameObject;
    return true;
}


NewGameObject* MyFirstWndGame::GetGameObject(std::string name)
{
    auto it = m_GameObjectPtrTable.find(name);

    // 이미 같은 이름을 가진 게임 오브젝트가 있는지 확인.
    if (it == m_GameObjectPtrTable.end())
    {
        return nullptr;
    }

    return it->second;
}

// 비트맵 등록
bool MyFirstWndGame::AddBitmapInfo(std::string bitMapName, LPCWSTR filename)
{
    BitmapInfo* bitMapInfo = renderHelp::CreateBitmapInfo(filename);
    if (bitMapInfo == nullptr)
    {
        return false;
    }
    m_pBitmapInfoTable[bitMapName] = bitMapInfo;
    return true;
}

// 비트맵 조회
renderHelp::BitmapInfo* MyFirstWndGame::GetBitmapInfo(std::string bitMapName)
{
    auto it = m_pBitmapInfoTable.find(bitMapName);
    if (it == m_pBitmapInfoTable.end()) return nullptr;
    return it->second;
}

// 리소스 로드
void MyFirstWndGame::LoadResource()
{
    AddBitmapInfo("Board", L"./Resource/samplegrid.png");
    AddBitmapInfo("Apple", L"./Resource/apple.png");
    AddBitmapInfo("Banana", L"./Resource/banana.png");
    AddBitmapInfo("Grapes", L"./Resource/grapes.png");
}



