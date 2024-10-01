#pragma once
#include <SDL.h>
#include <iostream>

class Renderer
{
private:
	SDL_Window* m_window;
public:
	int Initialize();
	int CreateWindow(std::string name, int height, int width);

	SDL_Window* GetWindow() { return m_window; }
};