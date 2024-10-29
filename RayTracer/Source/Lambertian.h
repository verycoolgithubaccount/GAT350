#pragma once
#include "Material.h"

class Lambertian : public Material
{
public:
	Lambertian(const color3_t& albedo) : Material{ albedo } {}

	bool Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scatter) override;
};