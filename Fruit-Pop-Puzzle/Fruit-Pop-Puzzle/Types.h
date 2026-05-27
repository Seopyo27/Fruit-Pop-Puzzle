#pragma once
namespace EHEngine
{
	struct POS
	{
		int x = 0;
		int y = 0;

		bool operator!=(const POS& other) const
		{
			return (x != other.x || y != other.y);
		}
	};

	using MOUSE_POS = POS;
	using SCREEN_POS = POS;

	struct BoardLayout
	{
		int boardWidth; //  전체 이미지 너비
		int boardHeight; // 전체 이미지 높이
		int cellWidth; // 그리드 너비
		int cellHeight; // 그리드 높이
		int maxRow; // 행 개수
		int maxCol; // 열개수
		int gridOffsetX; // 그리드 시작 X 좌표
		int gridOffsetY; // 그리드 시작 Y 좌표
		int gridGap; // 그리드 간격
	};
}