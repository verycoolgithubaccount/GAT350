#include "FrameBuffer.h"
#include "Renderer.h"

Framebuffer::Framebuffer(const Renderer& renderer, int width, int height)
{
	m_width = width;
	m_height = height;
	m_pitch = width * sizeof(color_t);

	m_texture = SDL_CreateTexture(renderer.GetRenderer(), SDL_PIXELFORMAT_RGBA32, //SDL_PIXELFORMAT_RGBA32 is SDL_PIXELFORMAT_ABGR8888
		SDL_TEXTUREACCESS_STREAMING, width, height); 

	if (!m_texture) std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;

	m_buffer.resize(m_width * m_height);
}

Framebuffer::~Framebuffer()
{
	SDL_DestroyTexture(m_texture);
}

void Framebuffer::Update()
{
	SDL_UpdateTexture(m_texture, NULL, m_buffer.data(), m_pitch);
}

void Framebuffer::Clear(const color_t& color)
{
	std::fill(m_buffer.begin(), m_buffer.end(), color);
}

void Framebuffer::DrawPoint(int x, int y, const color_t& color)
{
	if (x > m_width || x < 0 || y < 0 || y >= m_height) return;

	m_buffer[x + y * m_width] = color;
}

void Framebuffer::DrawRect(int x, int y, int w, int h, const color_t& color)
{
	for (int i = y; i < y + w; i++)
	{
		for (int j = x; j < x + h; j++)
		{
			DrawPoint(i, j, color);
		}
	}
}
