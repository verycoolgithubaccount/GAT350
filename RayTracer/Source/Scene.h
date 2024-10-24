#pragma once
#include "SceneObject.h"
#include <vector> // for a game, list is better bc adding/deleting objects, for a showcase, vector is better bc faster
#include <memory>

class Scene
{
private:
	std::vector<std::unique_ptr<SceneObject>> m_objects;
public:
	Scene() = default;

	void Render(class Framebuffer& framebuffer, const class Camera& camera);
	void AddObject(std::unique_ptr<SceneObject> object) { m_objects.push_back(std::move(object)); }
};