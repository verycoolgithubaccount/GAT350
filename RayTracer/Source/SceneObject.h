#pragma once
#include <memory>

class SceneObject
{
private:
	std::shared_ptr<class Material> m_material;

public:
	SceneObject() = default;
	SceneObject(std::shared_ptr<class Material> material) : m_material{ material } {}

	std::weak_ptr<class Material> GetMaterial() { return m_material; }
};