#include "Renderer.h"
#include "Framebuffer.h"
#include "MathUtils.h"
#include "Image.h"
#include "PostProcess.h"
#include "Color.h"
#include "Model.h"
#include "Random.h"
#include "Input.h"
#include "ETime.h"
#include "Transform.h"
#include "Camera.h"
#include "Tracer.h"
#include "Ray.h"
#include "Scene.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Emissive.h"
#include "Dielectric.h"

#include <SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

void InitScene(Scene& scene);

int main(int argc, char* argv[])
{
    srand((unsigned int) time(NULL));

    // Initialize
    ETime eTime;
    Input input;
    input.Initialize();

    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("Ray Tracer", 960, 600);

    Framebuffer framebuffer(renderer, renderer.GetWidth(), renderer.GetHeight());

    Camera camera{ 70.0f, (renderer.GetWidth() / (float) renderer.GetHeight())};
    camera.SetView({ 0, 0, -20 }, { 0, 0, 0 });

    Scene scene;
    InitScene(scene);

    // render
    scene.Update();
    scene.Render(framebuffer, camera, 50, 5);
    framebuffer.Update();

    bool quit = false;
    while (!quit)
    {
        eTime.Tick();
        input.Update();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
            }
        }

        // show screen
        renderer = framebuffer;
        SDL_RenderPresent(renderer.GetRenderer());
    }

    return 0;
}


void InitScene(Scene& scene)
{
    std::vector<std::shared_ptr<Material>> materials;

#pragma region material declaration

    materials.push_back(std::make_shared<Lambertian>(color3_t{ 0.6f, 0, 0 })); // red
    materials.push_back(std::make_shared<Lambertian>(color3_t{ 0.9f, 0.55f, 0.4f })); // orange
    materials.push_back(std::make_shared<Lambertian>(color3_t{ 0.96f, 0.8f, 0.08f })); // yellow
    materials.push_back(std::make_shared<Lambertian>(color3_t{ 0.41f, 0.6f, 0.36f })); // green
    materials.push_back(std::make_shared<Lambertian>(color3_t{ 0, 0.5f, 0.69f })); // blue
    materials.push_back(std::make_shared<Lambertian>(color3_t{ 0.51f, 0, 0.39f })); // violet
    materials.push_back(std::make_shared<Lambertian>(color3_t{ 0.41f, 0.4f, 0.49f })); // gray
    materials.push_back(std::make_shared<Lambertian>(color3_t{ 0.996f, 0.99f, 1 })); // white
    materials.push_back(std::make_shared<Lambertian>(color3_t{ 0.04f, 0.04f, 0.04f })); // black

    materials.push_back(std::make_shared<Metal>(color3_t{ 0.6f, 0, 0 }, randomf(0, 0.5f))); // red
    materials.push_back(std::make_shared<Metal>(color3_t{ 0.9f, 0.55f, 0.4f }, randomf(0, 0.5f))); // orange
    materials.push_back(std::make_shared<Metal>(color3_t{ 0.96f, 0.8f, 0.08f }, randomf(0, 0.5f))); // yellow
    materials.push_back(std::make_shared<Metal>(color3_t{ 0.41f, 0.6f, 0.36f }, randomf(0, 0.5f))); // green
    materials.push_back(std::make_shared<Metal>(color3_t{ 0, 0.5f, 0.69f }, randomf(0, 0.5f))); // blue
    materials.push_back(std::make_shared<Metal>(color3_t{ 0.51f, 0, 0.39f }, randomf(0, 0.5f))); // violet
    materials.push_back(std::make_shared<Metal>(color3_t{ 0.41f, 0.4f, 0.49f }, randomf(0, 0.5f))); // gray
    materials.push_back(std::make_shared<Metal>(color3_t{ 0.996f, 0.99f, 1 }, randomf(0, 0.5f))); // white
    materials.push_back(std::make_shared<Metal>(color3_t{ 0.04f, 0.04f, 0.04f }, randomf(0, 0.5f))); // black

    materials.push_back(std::make_shared<Emissive>(color3_t{ 0.6f, 0, 0 }, 10.0f)); // red
    materials.push_back(std::make_shared<Emissive>(color3_t{ 0.9f, 0.55f, 0.4f }, 10.0f)); // orange
    materials.push_back(std::make_shared<Emissive>(color3_t{ 0.96f, 0.8f, 0.08f }, 10.0f)); // yellow
    materials.push_back(std::make_shared<Emissive>(color3_t{ 0.41f, 0.6f, 0.36f }, 10.0f)); // green
    materials.push_back(std::make_shared<Emissive>(color3_t{ 0, 0.5f, 0.69f }, 10.0f)); // blue
    materials.push_back(std::make_shared<Emissive>(color3_t{ 0.51f, 0, 0.39f }, 10.0f)); // violet
    materials.push_back(std::make_shared<Emissive>(color3_t{ 0.41f, 0.4f, 0.49f }, 10.0f)); // gray
    materials.push_back(std::make_shared<Emissive>(color3_t{ 0.996f, 0.99f, 1 }, 10.0f)); // white
    materials.push_back(std::make_shared<Emissive>(color3_t{ 0.04f, 0.04f, 0.04f }, 10.0f)); // black

    materials.push_back(std::make_shared<Dielectric>(color3_t{ 0.6f, 0, 0 }, randomf(1.33f, 2.42f))); // red
    materials.push_back(std::make_shared<Dielectric>(color3_t{ 0.9f, 0.55f, 0.4f }, randomf(1.33f, 2.42f))); // orange
    materials.push_back(std::make_shared<Dielectric>(color3_t{ 0.96f, 0.8f, 0.08f }, randomf(1.33f, 2.42f))); // yellow
    materials.push_back(std::make_shared<Dielectric>(color3_t{ 0.41f, 0.6f, 0.36f }, randomf(1.33f, 2.42f))); // green
    materials.push_back(std::make_shared<Dielectric>(color3_t{ 0, 0.5f, 0.69f }, randomf(1.33f, 2.42f))); // blue
    materials.push_back(std::make_shared<Dielectric>(color3_t{ 0.51f, 0, 0.39f }, randomf(1.33f, 2.42f))); // violet
    materials.push_back(std::make_shared<Dielectric>(color3_t{ 0.41f, 0.4f, 0.49f }, randomf(1.33f, 2.42f))); // gray
    materials.push_back(std::make_shared<Dielectric>(color3_t{ 0.996f, 0.99f, 1 }, randomf(1.33f, 2.42f))); // white
    materials.push_back(std::make_shared<Dielectric>(color3_t{ 0.04f, 0.04f, 0.04f }, randomf(1.33f, 2.42f))); // black
#pragma endregion

    /*
    for (int i = 0; i < 20; i++)
    {
        auto sphere = std::make_unique<Sphere>(Transform{ random(glm::vec3{ -10 }, glm::vec3{ 10 }) }, randomf(0, 3.0f), materials.at(random(materials.size())));
        scene.AddObject(std::move(sphere));
    }
    */

    auto control = std::make_unique<Sphere>(Transform{ glm::vec3{0} }, 3.0f, materials.at(7));
    //scene.AddObject(std::move(control));

    auto control2 = std::make_unique<Sphere>(Transform{ glm::vec3{ 2, 2, 10 } }, 3.0f, materials.at(10));
    //scene.AddObject(std::move(control2));

    auto triangle = std::make_unique<Triangle>(glm::vec3{ 0, 0, 0 }, glm::vec3{ -10, 5, 0 }, glm::vec3{ 10, 0, 0 }, materials.at(10));
    //scene.AddObject(std::move(triangle));

    auto plane = std::make_unique<Plane>(Transform{ glm::vec3{0, -2, 0} }, materials.at(6));
    scene.AddObject(std::move(plane));

    auto model = std::make_unique<Model>(Transform{ glm::vec3{0, 2, 2}, glm::vec3{0, 40, 0}, glm::vec3{4} }, materials.at(3));
    model->Load("models/cube.obj");
    scene.AddObject(std::move(model));
}