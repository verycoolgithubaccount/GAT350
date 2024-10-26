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

    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("2D", 960, 600);

    Input input;
    input.Initialize();
    input.Update();

    //SDL_SetWindowMouseGrab(renderer.GetWindow(), SDL_TRUE);

    Framebuffer framebuffer(renderer, renderer.GetWidth(), renderer.GetHeight());

    Camera camera(renderer.GetWidth(), renderer.GetHeight());
    camera.SetView(glm::vec3{ 0, 0, -50 }, glm::vec3{ 0 });
    camera.SetProjection(60.0f, renderer.GetWidth() / (float) renderer.GetHeight(), 0.1f, 200.0f);
    Transform cameraTransform{ { 0, 0, -20 } };

    // Load images
    Image image;
    image.Load("hill.jpg");

    std::vector<std::unique_ptr<Actor>> actors;

    std::shared_ptr<Model> teapotModel = std::make_shared<Model>();
    teapotModel->Load("teapot.obj");
    Transform teapotTransform({ 0.0f, 30.0f, 0.0f }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 2 });
    std::unique_ptr<Actor> teapot = std::make_unique<Actor>(teapotTransform, teapotModel);
    teapot->SetColor({ 0, 55, 200 });
    actors.push_back(std::move(teapot));

    std::shared_ptr<Model> littleManModel = std::make_shared<Model>();
    littleManModel->Load("littleMan.obj");
    Transform littleManTransform({ 0.0f, 3.0f, 0.0f }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 4 });
    std::unique_ptr<Actor> littleMan = std::make_unique<Actor>(littleManTransform, littleManModel);
    littleMan->SetColor({ 255, 128, 0 });
    actors.push_back(std::move(littleMan));

    std::shared_ptr<Model> groundModel = std::make_shared<Model>();
    groundModel->Load("ground.obj");
    Transform groundTransform({ 0.0f, -5.0f, 0.0f }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 5 });
    std::unique_ptr<Actor> ground = std::make_unique<Actor>(groundTransform, groundModel);
    ground->SetColor({ 92, 56, 21 });
    actors.push_back(std::move(ground));


    std::shared_ptr<Model> treeModel = std::make_shared<Model>();
    treeModel->Load("tree.obj");
    

    
    for (int i = 0; i < 20; i++)
    {
        Transform transform({ randomf(-50.0f, 50.0f), 0, randomf(-50.0f, 50.0f) }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0.05 });
        std::unique_ptr<Actor> actor = std::make_unique<Actor>(transform, treeModel);
        actor->SetColor({ (uint8_t)random(0, 100), (uint8_t)random(150, 256), (uint8_t)random(0, 100), 255 });
        actors.push_back(std::move(actor));
    }
    

    Image imageAlpha;
    imageAlpha.Load("imageAlpha.png");
    PostProcess::Alpha(imageAlpha.m_buffer, 64);


    // Create model
    vertices_t vertices = { { -5, 5, 0 }, { 5, 5, 0 }, { -5, -5, 0 } };
    Model model(vertices, { 0, 255, 0, 255 });

    bool teapotShrinking = true;

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

        actors.at(0).get()->GetTransform().rotation.x += 10 * time.GetDeltaTime();
        actors.at(0).get()->GetTransform().rotation.y += 60 * time.GetDeltaTime();
        actors.at(0).get()->GetTransform().rotation.z += 30 * time.GetDeltaTime();

        if (actors.at(0).get()->GetTransform().scale.x > 2) teapotShrinking = true;
        else if (actors.at(0).get()->GetTransform().scale.x < 0.5) teapotShrinking = false;

        if (teapotShrinking) actors.at(0).get()->GetTransform().scale -= 0.3f * time.GetDeltaTime();
        else actors.at(0).get()->GetTransform().scale += 0.3f * time.GetDeltaTime();

        if (randomf() > 0.5) actors.at(1).get()->GetTransform().position.x -= 20 * time.GetDeltaTime();
        if (randomf() > 0.5) actors.at(1).get()->GetTransform().position.x += 20 * time.GetDeltaTime();
        if (randomf() > 0.5) actors.at(1).get()->GetTransform().position.z -= 20 * time.GetDeltaTime();
        if (randomf() > 0.5) actors.at(1).get()->GetTransform().position.z += 20 * time.GetDeltaTime();

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

        framebuffer.DrawImage(0, -100, 1.0f, image);

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

        if (input.GetMouseButtonDown(2))
        {
            input.SetRelativeMode(true);

            glm::vec3 direction{ 0 };
            if (input.GetKeyDown(SDL_SCANCODE_D)) direction.x = 1;
            if (input.GetKeyDown(SDL_SCANCODE_A)) direction.x = -1;
            if (input.GetKeyDown(SDL_SCANCODE_E)) direction.y = 1;
            if (input.GetKeyDown(SDL_SCANCODE_Q)) direction.y = -1;
            if (input.GetKeyDown(SDL_SCANCODE_W)) direction.z = 1;
            if (input.GetKeyDown(SDL_SCANCODE_S)) direction.z = -1;

            cameraTransform.rotation.y += input.GetMouseRelative().x * 0.25f;
            cameraTransform.rotation.x += input.GetMouseRelative().y * 0.25f;

            glm::vec3 offset = cameraTransform.GetMatrix() * glm::vec4{ direction, 0 };

            cameraTransform.position += offset * 70.0f * time.GetDeltaTime();
        }
        else
        {
            input.SetRelativeMode(false);
        }

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