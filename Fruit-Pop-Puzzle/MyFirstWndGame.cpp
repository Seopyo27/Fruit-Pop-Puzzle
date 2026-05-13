#include "INC_Windows.h"
#include "MyFirstWndGame.h"
#include "GameTimer.h"
#include "Collider.h"
#include "GameObject.h"
#include "RenderHelp.h"
#include "Board.h"
#include <iostream>
#include <assert.h>


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

    m_GameObjectPtrTable = new GameObjectBase * [MAX_GAME_OBJECT_COUNT];

    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        m_GameObjectPtrTable[i] = nullptr;
    }

    // 필요한 리소스를 로드해볼까요
#pragma region resource

    // 파일의 절대 경로와 상대 경로 구분
    // IDE 에서 인지하는 현재 경로와 실제 실행 파일을 바로 실행했을 때의 경로 기준이 달라요.
    m_pBoardBitmapInfo = renderHelp::CreateBitmapInfo(L"./Resource/samplegrid.png");

#pragma endregion

    // 리소스를 먼저 로드한 후에 세팅을 합니다.

	// 보드 생성
    CreateBoard(800, 800, 94, 95, 6, 6, 102, 93, 6);

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

}

void MyFirstWndGame::LogicUpdate()
{
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Update(m_fDeltaTime);
        }
    }
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
  

   m_GameObjectPtrTable[0] = pNewBoard;
   m_GameObjectCount += 1;

   // 보드 인스턴스를 자주 사용하기위해 따로 포인터를 저장
   m_pBoard = pNewBoard;
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
    std::cout << '\n' << '\n' << '\n' << std::flush;
    std::cout << __FUNCTION__ << std::endl;

    // 윈도우 좌표 마우스 위치 출력
    std::cout << "mousePos: ( " << x << ", " << y << " )" << std::endl;

    Point boardPos = { 0, 0 };
    // 보드를 클릭했으면 mousePos -> boardPos 변환 후 저장
    if (!ConvertScreenToBoard(x, y, boardPos))
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

void MyFirstWndGame::OnRButtonDown(int x, int y)
{
    std::cout << '\n' << '\n' << '\n' << std::flush;
	std::cout << __FUNCTION__ << std::endl;
    std::cout << "mousePos: ( " << x << ", " << y << " )" << std::endl;
}

bool MyFirstWndGame::ConvertScreenToBoard(int mouseX, int mouseY, Point& boardPos)
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