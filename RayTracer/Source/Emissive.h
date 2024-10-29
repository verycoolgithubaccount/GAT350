#pragma once
#include "Material.h"

class Emissive : public Material
{
private:
	float m_intensity{ 1 };
public:
	Emissive(const color3_t& albedo, float intensity = 1);
};