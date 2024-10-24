#pragma once
#include "Ray.h"
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>

class Camera
{
private:
	float m_fov{ 60 }; // fov in degrees
	float m_aspectRatio{ 1 }; // screen width / screen height

	glm::vec3 m_eye{ 0 };

	// camera axis
	glm::vec3 m_forward{ 0 };
	glm::vec3 m_right{ 0 };
	glm::vec3 m_up{ 0 };

	// view plane origin and horizontal and vertical direction vectors
	glm::vec3 m_lowerLeft{ 0 };
	glm::vec3 m_horizontal{ 0 };
	glm::vec3 m_vertical{ 0 };

	void CalculateViewPlane();

public:
	Camera(float fov, float aspectRatio) : m_fov{ fov }, m_aspectRatio{ aspectRatio } {}

	void SetView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up = { 0, 1, 0 }); // up defaults to unit vector pointing up in y
	ray_t GetRay(const glm::vec2& point) const;
};