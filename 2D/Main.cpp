#include "Source/Renderer.h"
#include "Source/Framebuffer.h"
#include "Source/MathUtils.h"
#include "Source/Image.h"
#include "Source/PostProcess.h"
#include "Source/Color.h"
#include "Source/Model.h"

#include <SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char* argv[])
{
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("2D", 960, 600);

    Image image;
    image.Load("scenic.jpg");

    Image imageAlpha;
    imageAlpha.Load("imageAlpha.png");
    PostProcess::Alpha(imageAlpha.m_buffer, 64);

    vertices_t vertices = { { -5, 5, 0 }, { 5, 5, 0 }, { -5, -5, 0 } };
    Model model(vertices, { 0, 255, 0, 255 });

    Framebuffer framebuffer(renderer, 960, 600);

    bool quit = false;
    while (!quit)
    {
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

        int ticks = SDL_GetTicks();
        float time = ticks * 0.001;
        float t = std::abs(std::sin(time));
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

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 translate = glm::translate(modelMatrix, glm::vec3(240.0f, 240.0f, 0.0f));
        glm::mat4 scale = glm::scale(modelMatrix, glm::vec3(10.0f));
        glm::mat4 rotate = glm::rotate(modelMatrix, glm::radians(time * 90), glm::vec3( 1, 1, 1)); // x, y, z, 0 or 1 for each

        modelMatrix = translate * scale * rotate;

        model.Draw(framebuffer, modelMatrix);

        framebuffer.Update();
        
        renderer = framebuffer;
        //renderer.CopyFramebuffer(framebuffer);

        // show screen
        SDL_RenderPresent(renderer.GetRenderer());
    }

    return 0;
}