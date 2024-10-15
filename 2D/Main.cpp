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

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1.0f, 0.0f, 0.0f));

    vertices_t vertices = { { -5, 5, 0 }, { 5, 5, 0 }, { -5, -5, 0 } };
    Model model;

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

        int mx, my;
        SDL_GetMouseState(&mx, &my);

        //framebuffer.DrawLinearCurve(100, 100, 200, 200, { 255, 255, 255, 255 });
        //framebuffer.DrawQuadraticCurve(100, 200, mx, my, 300, 200, { 255, 255, 255, 255 });
        //framebuffer.DrawCubicCurve(300, 500, 300, 200, mx, my, 600, 400, { 255, 255, 255, 255 });
        //framebuffer.DrawCircle(500, 500, 50, { 255, 255, 255, 255 });
        

        int ticks = SDL_GetTicks();
        float time = ticks * 0.001;
        float t = std::abs(std::sin(time));
        //int x, y;
        //Math::CubicPoint(300, 400, 300, 300, mx, my, 600, 400, t, x, y);
        //framebuffer.DrawRect(x - 20, y - 20, 40, 40, { 0, 255, 0, 255 });

        Color::SetBlendMode(Color::BlendMode::NORMAL);
        framebuffer.DrawImage(200, 200, 1.5f, image);
        PostProcess::BoxBlur(framebuffer.GetBuffer(), framebuffer.GetWidth(), framebuffer.GetHeight());
        //PostProcess::Posterize(framebuffer.GetBuffer(), 6);

        Color::SetBlendMode(Color::BlendMode::NORMAL);
        framebuffer.DrawCircle(mx, my, 20, { 100, 100, 0 });
        //framebuffer.DrawImage(mx, my, 1.0f, imageAlpha);

        //PostProcess::Monochrome(framebuffer.GetBuffer());
        framebuffer.Update();
        
        renderer = framebuffer;
        //renderer.CopyFramebuffer(framebuffer);

        // show screen
        SDL_RenderPresent(renderer.GetRenderer());
    }

    return 0;
}