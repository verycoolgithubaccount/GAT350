#pragma once
#include "Color.h"
#include "Ray.h"

class Material
{
protected:
	color3_t m_albedo;

public:
	Material() = default;
	Material(const color3_t& albedo) : m_albedo{ albedo } {}

	virtual bool Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scatter) = 0;
	virtual color3_t GetEmissive() const { return { 0, 0, 0 }; }

	color3_t& GetColor() { return m_albedo; }
};