#pragma once
#include "Model.h"
#include "Transform.h"
#include "Material.h"
#include <memory>

class Actor
{
public:
	Actor(Transform transform, std::shared_ptr<Model> model, std::shared_ptr<material_t> material) : 
		m_transform{ transform }, m_model{ model }, m_material{ material } {}

	Transform& GetTransform() { return m_transform; }
	const Transform& GetTransform() const { return m_transform; }

	void SetTransform(const Transform& transform) { m_transform = transform; }

	void Draw();

private:
	Transform m_transform;
	std::shared_ptr<Model> m_model;
	std::shared_ptr<material_t> m_material;
};