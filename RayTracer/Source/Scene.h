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
	color3_t m_sky{ 0.5f, 0.7f, 1.0f };
	color3_t m_skyFog{ 1 };
public:
	Scene() = default;

	void Update();
	void Render(class Framebuffer& framebuffer, const class Camera& camera, int numSamples = 10, int depth = 5);
	void AddObject(std::unique_ptr<SceneObject> object) { m_objects.push_back(std::move(object)); }

	const std::vector<std::unique_ptr<SceneObject>>& const GetObjects() { return m_objects; }
	const color3_t& GetSkyColor() const { return m_sky; }
	const color3_t& GetSkyFogColor() const { return m_skyFog; }

	void SetSky(const color3_t& skyFog, const color3_t& skyColor)
	{
		m_sky = skyColor;
		m_skyFog = skyFog;
	}

};