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
#include "Shader.h"

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
    image.Load("textures/hill.jpg");

    std::vector<std::unique_ptr<Actor>> actors;

    Image imageAlpha;
    imageAlpha.Load("textures/imageAlpha.png");
    PostProcess::Alpha(imageAlpha.m_buffer, 64);



    // shader
    VertexShader::uniforms.view = camera.GetView();
    VertexShader::uniforms.projection = camera.GetProjection();
    VertexShader::uniforms.ambient = color3_t{ 0.1 };

    VertexShader::uniforms.light.position = glm::vec3{ 100, 100, -10 };
    VertexShader::uniforms.light.direction = glm::vec3{ 0, -1, 0 }; // light pointing down
    VertexShader::uniforms.light.color = color3_t{ 1 }; // white light

    Shader::framebuffer = &framebuffer;
    
    // models
    std::shared_ptr<Model> model = std::make_shared<Model>();
    model->Load("models/dragon.obj");
    model->SetColor({ 0, 0.5, 0, 0 });

    Transform transform{ glm::vec3{ 0 }, glm::vec3{ 0 }, glm::vec3{ 5 } };
    std::unique_ptr<Actor> actor = std::make_unique<Actor>(transform, model);
    actors.push_back(std::move(actor));

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

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        //framebuffer.DrawImage(0, -100, 1.0f, image);

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
        VertexShader::uniforms.view = camera.GetView();

        //transform.rotation.x += 45 * time.GetDeltaTime();
        //transform.rotation.y += 90 * time.GetDeltaTime();
        //transform.rotation.z += 180 * time.GetDeltaTime();
        
        //model.Draw(framebuffer, transform.GetMatrix(), camera);

        for (auto& actor : actors)
        {
            actor->GetTransform().rotation.y += time.GetDeltaTime() * 90;
            actor->Draw();
        }

        framebuffer.Update();
        
        renderer = framebuffer;
        //renderer.CopyFramebuffer(framebuffer);

        // show screen
        SDL_RenderPresent(renderer.GetRenderer());
    }
    return 0;
}