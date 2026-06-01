#pragma once
namespace EHEngine
{
	struct Pos
	{
		int x = 0;
		int y = 0;

		bool operator!=(const Pos& other) const
		{
			return (x != other.x || y != other.y);
		}

		template<typename T>
		T ToVector2() const
		{
			return T{
				static_cast<float>(x),
				static_cast<float>(y)
			};
		}
	};

	using MousePos = Pos;
	using ScreenPos = Pos;
}