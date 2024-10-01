#include "Source/Renderer.h"
#include <SDL.h>
#include <iostream>
#include <memory>

int main(int argc, char* argv[])
{
    std::unique_ptr<Renderer> rendererObj = std::make_unique<Renderer>();
    rendererObj->Initialize();
    rendererObj->CreateWindow("Game Engine", 800, 600);

    // create renderer
    SDL_Renderer* SDLrenderer = SDL_CreateRenderer(rendererObj->GetWindow(), - 1, 0);

    while (true)
    {
        // clear screen
        SDL_SetRenderDrawColor(SDLrenderer, 0, 0, 0, 0);
        SDL_RenderClear(SDLrenderer);

        // show screen
        SDL_RenderPresent(SDLrenderer);
    }

    return 0;
}