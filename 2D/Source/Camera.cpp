#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 Camera::ModelToView(const glm::vec3& position) const
{
	// convert point from worldspace to viewspace, so models render relative to the camera
	return m_view * glm::vec4{ position, 1 };
}

glm::vec4 Camera::ViewToProjection(const glm::vec3& position) const
{
	// convert point from viewspace to projectionspace, so models render with perspective
	return m_projection * glm::vec4{ position, 1 };
}

glm::ivec2 Camera::ToScreen(const glm::vec3& position) const
{
	// Dividing by z moves points closer to the vanishing point depending on how far they are in z
	glm::vec4 clip = ViewToProjection(position);

	// prevent division by 0
	if (clip.w == 0) return glm::ivec2{ -1, -1 };
	
	// convert projection space to ndc (-1 <-> 1)
	glm::vec3 ndc = clip / clip.w;

	// don't draw if outside near and far
	if (ndc.z < -1 || ndc.z > 1) return glm::ivec2{ -1, -1 };

	float x = (ndc.x + 1) * (m_width * 0.5f);
	float y = (1 - ndc.y) * (m_height * 0.5f);

	return glm::ivec2(x, y);
}

void Camera::SetView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
{
	m_view = glm::lookAt(eye, target, up);
}

void Camera::SetProjection(float fov, float aspectRatio, float near, float far)
{
	m_projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
}
