#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Color.h"
#include "Tracer.h"
#include "Random.h"
#include <iostream>

void Scene::Render(Framebuffer& framebuffer, const Camera& camera, int numSamples, int depth)
{
	for (int y = 0; y < framebuffer.GetHeight(); y++)
	{
		for (int x = 0; x < framebuffer.GetWidth(); x++)
		{
			color3_t color{ 0 };
			for (int i = 0; i < numSamples; i++)
			{
				glm::vec2 pixel{ x, y };
				pixel += glm::vec2{ randomf(0, 1), randomf(0, 1)};
				glm::vec2 point = pixel / glm::vec2{ framebuffer.GetWidth(), framebuffer.GetHeight() };
				point.y = 1 - point.y;

				ray_t ray = camera.GetRay(point);
				color += Tracer::Trace(*this, ray, 0.001f, 100.0f, depth);
			}
			
			color /= (float) numSamples;

			framebuffer.DrawPoint(x, y, Color::Convert(color));
		}
		std::cout << "y: " << y << std::endl;
	}
}
