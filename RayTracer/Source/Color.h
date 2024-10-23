#pragma once
#include <SDL.h>
#include <algorithm>

using color_t = SDL_Color;

namespace Color
{
	enum class BlendMode
	{
		NORMAL,
		ALPHA,
		ADDITIVE,
		MULTIPLY
	};


	void SetBlendMode(BlendMode blendMode);
	color_t ColorBlend(const color_t& src, const color_t& dest);

	color_t NormalBlend(const color_t& src, const color_t& dest);
	color_t AlphaBlend(const color_t& src, const color_t& dest);
	color_t AdditiveBlend(const color_t& src, const color_t& dest);
	color_t MultiplyBlend(const color_t& src, const color_t& dest);
}