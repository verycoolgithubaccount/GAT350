#pragma once
#include "Material.h"

class Dielectric : public Material
{
protected:
	float m_refractiveIndex = 0;
public:
	Dielectric(const glm::vec3& albedo, float refractiveIndex) : Material{ albedo }, m_refractiveIndex{ refractiveIndex } {}

	bool Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scatter) override;

};