#pragma once
#include "Material.h"

class Emissive : public Material
{
private:
	float m_intensity{ 1 };

public:
	Emissive(const color3_t& albedo, float intensity = 1) : Material(albedo), m_intensity{ intensity } {}

	bool Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scatter) override;
	color3_t GetEmissive() const override { return m_albedo * m_intensity; }

};