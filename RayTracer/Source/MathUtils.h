#pragma once
#include <cmath>
#include <glm/glm.hpp>

namespace Math
{
	template<typename T>
	inline T Lerp(const T& a, const T& b, float t) 
	{ 
		return static_cast<T>(a + (t * (b - a))); 
	}

	template<typename T>
	inline T Clamp(const T& value, const T& min, const T& max)
	{
		return (value < min) ? min : (value > max) ? max : value;
	}

	inline glm::vec3 Cross(const glm::vec3& v1, const glm::vec3& v2)
	{
		glm::vec3 result;

		result.x = v1.y * v2.z - v2.y * v1.z;
		result.y = v1.z * v2.x - v2.z * v1.x;
		result.z = v1.x * v2.y - v2.x * v1.y;

		return result;
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
		float a = static_cast<float>(std::pow(one_minus_t, 3)); // one_minus_t^3
		float b = static_cast<float>(3 * std::pow(one_minus_t, 2) * t);
		float c = 3 * one_minus_t * t * t;
		float d = t * t * t;

		x = static_cast<int>((a * x1) + (b * x2) + (c * x3) + (d * x4));
		y = static_cast<int>((a * y1) + (b * y2) + (c * y3) + (d * x4));
	}
}