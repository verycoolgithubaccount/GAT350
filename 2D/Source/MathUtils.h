#pragma once

namespace Math
{
	template<typename T>
	inline T Lerp(T a, T b, float t) 
	{ 
		return static_cast<T>(a + (t * (b - a))); 
	}

	inline void QuadraticPoint(int x1, int y1, int x2, int y2, int x3, int y3, float t, int& x, int& y)
	{
		float one_minus_t = 1 - t;
		float a = one_minus_t * one_minus_t;
		float b = 2 * one_minus_t * t;
		float c = t * t;

		x = static_cast<int>((a * x1) + (b * x2) + (c * x3));
		y = static_cast<int>((a * y1) + (b * y2) + (c * y3));
	}

	inline void CubicPoint(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, float t, int& x, int& y)
	{
		float one_minus_t = 1 - t;
		float a = std::pow(one_minus_t, 3); // one_minus_t^3
		float b = 3 * std::pow(one_minus_t, 2) * t;
		float c = 3 * one_minus_t * t * t;
		float d = t * t * t;

		x = static_cast<int>((a * x1) + (b * x2) + (c * x3) + (d * x4));
		y = static_cast<int>((a * y1) + (b * y2) + (c * y3) + (d * x4));
	}
}