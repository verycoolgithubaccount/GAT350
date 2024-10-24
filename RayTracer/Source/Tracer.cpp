#include "Tracer.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Color.h"
#include "Ray.h"
#include "MathUtils.h"
#include <glm/glm.hpp>

color3_t Tracer::Trace(Scene& scene, const ray_t& ray)
{
	color3_t color{ 0 };

	// sky
	glm::vec3 direction = glm::normalize(ray.direction);
	float t = (direction.y + 1) * 0.5f;
	color = Math::Lerp(color3_t{ 1, 1, 1 }, color3_t{ 0.5f, 0.7f, 1.0f }, t);

	return color;
}
