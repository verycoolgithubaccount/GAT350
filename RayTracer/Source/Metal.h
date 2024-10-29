#pragma once
#include "Material.h"

class Metal : public Material
{
protected:
	float m_fuzz = 0;
public:
	Metal(const glm::vec3& albedo, float fuzz) : Material{ albedo }, m_fuzz{ fuzz } {}

	bool Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scatter) override;

};