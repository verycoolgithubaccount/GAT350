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
#include "Actor.h"
#include "Ray.h"
#include "Scene.h"

#include <SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>
#include <ctime>

int main(int argc, char* argv[])
{
    // Initialize
    ETime eTime;
    Input input;
    input.Initialize();

    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("Ray Tracer", 960, 600);

    srand(time(NULL)); // looked up how to do this to get more random stuff

    Framebuffer framebuffer(renderer, renderer.GetWidth(), renderer.GetHeight());

    Camera camera{ 70.0f, (renderer.GetWidth() / (float) renderer.GetHeight())};
    camera.SetView({ 0, 0, -20 }, { 0, 0, 0 });

    Scene scene;
    std::vector<std::shared_ptr<Material>> materials;

    materials.push_back(std::make_shared<Material>(color3_t{ 0.6f, 0, 0 })); // red
    materials.push_back(std::make_shared<Material>(color3_t{ 0.9f, 0.55f, 0.4f })); // orange
    materials.push_back(std::make_shared<Material>(color3_t{ 0.96f, 0.8f, 0.08f })); // yellow
    materials.push_back(std::make_shared<Material>(color3_t{ 0.41f, 0.6f, 0.36f })); // green
    materials.push_back(std::make_shared<Material>(color3_t{ 0, 0.5f, 0.69f })); // blue
    materials.push_back(std::make_shared<Material>(color3_t{ 0.51f, 0, 0.39f })); // violet
    materials.push_back(std::make_shared<Material>(color3_t{ 0.41f, 0.4f, 0.49f })); // gray
    materials.push_back(std::make_shared<Material>(color3_t{ 0.996f, 0.99f, 1 })); // white
    materials.push_back(std::make_shared<Material>(color3_t{ 0.04f, 0.04f, 0.04f })); // black

    for (int i = 0; i < 20; i++)
    {
        auto sphere = std::make_unique<Sphere>(random(glm::vec3{ -10 }, glm::vec3{ 10 }), randomf(0, 3.0f), materials.at(random(materials.size())));
        scene.AddObject(std::move(sphere));
    }

    auto plane = std::make_unique<Plane>(glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, materials.at(6));
    scene.AddObject(std::move(plane));

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

        // render
        framebuffer.Clear(Color::Convert(color4_t{ 0.125, 0.125, 0.25, 1 }));

        scene.Render(framebuffer, camera);

        framebuffer.Update();
        
        // show screen
        renderer = framebuffer;
        SDL_RenderPresent(renderer.GetRenderer());
    }

    return 0;
}