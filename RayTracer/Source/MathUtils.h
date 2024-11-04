#pragma once
#define FLT_EPSILON 1.192092896e-07F 
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

	inline float Dot(const glm::vec3& v1, const glm::vec3& v2)
	{
		return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	}

	inline float Angle(const glm::vec3& v1, const glm::vec3& v2)
	{
		return glm::acos(Dot(glm::normalize(v1), glm::normalize(v2)));
	}

	inline glm::vec3 Reflect(const glm::vec3& incident, const glm::vec3& normal)
	{
		return incident - (normal * Dot(normal, incident)) * 2.0f;
	}

	inline bool Refract(const glm::vec3& incident, const glm::vec3& normal, float refractiveIndex, glm::vec3& refractedRay)
	{
		glm::vec3 normalizedIncident = glm::normalize(incident);

		float cosine = Dot(normalizedIncident, normal);

		float descriminant = 1 - (refractiveIndex * refractiveIndex) * (1 - (cosine * cosine));

		if (descriminant > 0)
		{
			refractedRay = refractiveIndex * (normalizedIncident - (normal * cosine)) - (normal * glm::sqrt(descriminant));
			return true;
		}

		return false;
	}

	inline float Schlick(float cosine, float index)
	{
		// Step 1: Calculate the base reflectance at zero incidence (angle = 0)
		// This is the reflection coefficient when the light hits the surface straight on
		float r0 = (1.0f - index) / (1.0f + index);
		r0 = r0 * r0;

		// Step 2: Use Schlick's approximation to adjust reflectance based on angle
		// Schlick’s approximation gives the probability of reflection at an angle `cosine`
		// It interpolates between `r0` and 1, with stronger reflection at glancing angles
		return r0 + (1.0f - r0) * (float)std::pow((1.0f - cosine), 5);
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

	inline bool Approximately(float value1, float value2)
	{
		return (std::fabs(value2 - value1) < FLT_EPSILON);
	}
}