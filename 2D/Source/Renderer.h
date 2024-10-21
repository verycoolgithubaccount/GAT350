#pragma once
#include <SDL.h>
#include <iostream>

class Renderer
{
private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	int m_width{ 0 };
	int m_height{ 0 };
public:
	int Initialize();
	int CreateWindow(std::string name, int height, int width);
	void CopyFramebuffer(class Framebuffer framebuffer);

	void operator = (const class Framebuffer& framebuffer);

	SDL_Window* GetWindow() const { return m_window; }
	SDL_Renderer* GetRenderer() const { return m_renderer; }
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
};