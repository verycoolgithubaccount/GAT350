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
#include "Actor.h"

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
    renderer.CreateWindow("RayTracer", 960, 600);

    //SDL_SetWindowMouseGrab(renderer.GetWindow(), SDL_TRUE);

    Framebuffer framebuffer(renderer, renderer.GetWidth(), renderer.GetHeight());

    Camera camera(renderer.GetWidth(), renderer.GetHeight());
    camera.SetView(glm::vec3{ 0, 0, -50 }, glm::vec3{ 0 });
    camera.SetProjection(60.0f, renderer.GetWidth() / renderer.GetHeight(), 0.1f, 200.0f);
    Transform cameraTransform{ { 0, 0, -20 } };

    // Load images
    Image image;
    image.Load("scenic.jpg");

    std::shared_ptr<Model> teapot = std::make_shared<Model>();
    teapot->Load("torus.obj");
    teapot->SetColor({ 0, 255, 0, 255 });

    std::vector<std::unique_ptr<Actor>> actors;

    for (int i = 0; i < 20; i++)
    {
        Transform transform({ randomf(-10.0f, 10.0f), randomf(-10.0f, 10.0f), randomf(-10.0f, 10.0f) }, glm::vec3{ 0, 0, 45 }, glm::vec3{ 2 });
        std::unique_ptr<Actor> actor = std::make_unique<Actor>(transform, teapot);
        actor->SetColor({ (uint8_t)random(256), (uint8_t)random(256), (uint8_t)random(256), 255 });
        actors.push_back(std::move(actor));
    }

    Image imageAlpha;
    imageAlpha.Load("imageAlpha.png");
    PostProcess::Alpha(imageAlpha.m_buffer, 64);


    // Create model
    vertices_t vertices = { { -5, 5, 0 }, { 5, 5, 0 }, { -5, -5, 0 } };
    Model model(vertices, { 0, 255, 0, 255 });

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

        if (input.GetKeyDown(SDL_SCANCODE_ESCAPE)) quit = true;

        // clear screen
        SDL_SetRenderDrawColor(renderer.GetRenderer(), 0, 0, 0, 0);
        SDL_RenderClear(renderer.GetRenderer());

        framebuffer.Clear(color_t{ 0, 0, 0, 255 });

#pragma region primitive drawing loop
        /*
        for (int i = 0; i < 5; i++)
        {
            int x = rand() % framebuffer.GetWidth();
            int y = rand() % framebuffer.GetHeight();
            int x2 = rand() % framebuffer.GetWidth();
            int y2 = rand() % framebuffer.GetHeight();
            int x3 = rand() % framebuffer.GetWidth();
            int y3 = rand() % framebuffer.GetHeight();

            framebuffer.DrawTriangle(x, y, x2, y2, x3, y3, { (uint8_t) (rand() % 255), (uint8_t)(rand() % 255), (uint8_t)(rand() % 255), 255});
        
            x = rand() % framebuffer.GetWidth();
            y = rand() % framebuffer.GetHeight();
            x2 = rand() % framebuffer.GetWidth();
            y2 = rand() % framebuffer.GetHeight();

            framebuffer.DrawLine(x, y, x2, y2, { (uint8_t)(rand() % 255), (uint8_t)(rand() % 255), (uint8_t)(rand() % 255), 255 });


            x = rand() % framebuffer.GetWidth();
            y = rand() % framebuffer.GetHeight();
            int w = rand() % framebuffer.GetWidth() / 30;
            int h = rand() % framebuffer.GetHeight() / 30;
            framebuffer.DrawRect(x, y, w, h, { (uint8_t)(rand() % 255), (uint8_t)(rand() % 255), (uint8_t)(rand() % 255), 255 });

            
            //x = rand() % framebuffer.GetWidth();
            //y = rand() % framebuffer.GetHeight();
            //framebuffer.DrawImage(x, y, (2.0f / (rand() % 10)), image);
        }
        */
#pragma endregion

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        //int x, y;
        //Math::CubicPoint(300, 400, 300, 300, mx, my, 600, 400, t, x, y);
        //framebuffer.DrawRect(x - 20, y - 20, 40, 40, { 0, 255, 0, 255 });

#pragma region alpha stuff
        //Color::SetBlendMode(Color::BlendMode::NORMAL);
        //framebuffer.DrawImage(200, 200, 1.5f, image);
        //PostProcess::BoxBlur(framebuffer.GetBuffer(), framebuffer.GetWidth(), framebuffer.GetHeight());
        //PostProcess::Posterize(framebuffer.GetBuffer(), 6);

        //Color::SetBlendMode(Color::BlendMode::ALPHA);
        //framebuffer.DrawImage(mx, my, 1.0f, imageAlpha);
#pragma endregion

#pragma region postprocess
        //
        //PostProcess::Monochrome(framebuffer.GetBuffer());
#pragma endregion

        glm::vec3 direction{ 0 };
        if (input.GetKeyDown(SDL_SCANCODE_D)) direction.x = 1;
        if (input.GetKeyDown(SDL_SCANCODE_A)) direction.x = -1;
        if (input.GetKeyDown(SDL_SCANCODE_Q)) direction.y = 1;
        if (input.GetKeyDown(SDL_SCANCODE_E)) direction.y = -1;
        if (input.GetKeyDown(SDL_SCANCODE_W)) direction.z = 1;
        if (input.GetKeyDown(SDL_SCANCODE_S)) direction.z = -1;

        cameraTransform.rotation.y += input.GetMousePositionDelta().x * 0.1f;
        cameraTransform.rotation.x -= input.GetMousePositionDelta().y * 0.1f;

        glm::vec3 offset = cameraTransform.GetMatrix() * glm::vec4{ direction, 0 };

        cameraTransform.position += offset * 100.0f * time.GetDeltaTime();
        camera.SetView(cameraTransform.position, cameraTransform.position + cameraTransform.GetForward());

        //transform.rotation.x += 45 * time.GetDeltaTime();
        //transform.rotation.y += 90 * time.GetDeltaTime();
        //transform.rotation.z += 180 * time.GetDeltaTime();
        
        //model.Draw(framebuffer, transform.GetMatrix(), camera);

        for (auto& actor : actors)
        {
            actor->Draw(framebuffer, camera);
        }

        framebuffer.Update();
        
        renderer = framebuffer;
        //renderer.CopyFramebuffer(framebuffer);

        // show screen
        SDL_RenderPresent(renderer.GetRenderer());
    }

    return 0;
}