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

int main(int argc, char* argv[])
{
    Time time;
    Input input;
    input.Initialize();

    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("Ray Tracer", 960, 600);

    Framebuffer framebuffer(renderer, renderer.GetWidth(), renderer.GetHeight());

    Camera camera{ 70.0f, (float) (renderer.GetWidth() / renderer.GetHeight())};
    camera.SetView({ 0, 0, -20 }, { 0, 0, 0 });

    Scene scene;

    bool quit = false;
    while (!quit)
    {
        time.Tick();
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