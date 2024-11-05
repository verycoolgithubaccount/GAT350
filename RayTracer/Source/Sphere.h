#pragma once
#include "SceneObject.h"
#include "glm/glm.hpp"

class Sphere : public SceneObject
{
private:
	glm::vec3 m_center{ 0 };
	float m_radius{ 0 };

public:
	Sphere() = default;
	Sphere(const Transform& transform, float radius, std::shared_ptr<Material> material) :
		SceneObject{ transform, material },
		m_radius{ radius }
	{}

	Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> material) : 
		SceneObject { material }, 
		m_center{ center }, 
		m_radius{ radius } 
	{}

	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance) override;
	static bool Raycast(const ray_t& ray, const glm::vec3& center, float radius, float minDistance, float maxDistance, float& t);
};