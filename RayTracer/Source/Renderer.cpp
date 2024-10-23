#include "Renderer.h"
#include "FrameBuffer.h"

int Renderer::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }
}

int Renderer::CreateWindow(std::string name, int width, int height)
{
    m_width = width;
    m_height = height;

    m_window = SDL_CreateWindow(name.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN);
    if (m_window == nullptr)
    {
        std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
}

void Renderer::CopyFramebuffer(Framebuffer framebuffer)
{
    SDL_RenderCopy(m_renderer, framebuffer.GetTexture(), NULL, NULL);
}

void Renderer::operator=(const Framebuffer& framebuffer)
{
    SDL_RenderCopy(m_renderer, framebuffer.GetTexture(), NULL, NULL);
}