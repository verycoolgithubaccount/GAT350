#pragma once
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include <vector> // for a game, list is better bc adding/deleting objects, for a showcase, vector is better bc faster
#include <memory>

class Scene
{
private:
	std::vector<std::unique_ptr<SceneObject>> m_objects;
public:
	Scene() = default;

	void Render(class Framebuffer& framebuffer, const class Camera& camera, int numSamples = 10, int depth = 5);
	void AddObject(std::unique_ptr<SceneObject> object) { m_objects.push_back(std::move(object)); }

	const std::vector<std::unique_ptr<SceneObject>>& const GetObjects() { return m_objects; }
};