#include "Color.h"
#include <iostream>
#include <cassert>

namespace Color
{
	color_t(*blend_func)(const color_t& src, const color_t& dest);

	void SetBlendMode(BlendMode blendMode)
	{
		switch (blendMode)
		{
		case BlendMode::NORMAL:
			blend_func = NormalBlend;
			break;
		case BlendMode::ALPHA:
			blend_func = AlphaBlend;
			break;
		case BlendMode::ADDITIVE:
			blend_func = AdditiveBlend;
			break;
		case BlendMode::MULTIPLY:
			blend_func = MultiplyBlend;
			break;
		default:
			break;
		}
	}


	color_t ColorBlend(const color_t& src, const color_t& dest)
	{
		if (!blend_func) SetBlendMode(BlendMode::NORMAL);
		return blend_func(src, dest);
	}

	color_t NormalBlend(const color_t& src, const color_t& dest)
	{
		return src;
	}

	color_t AlphaBlend(const color_t& src, const color_t& dest)
	{
		uint8_t alpha = src.a;
		uint8_t inv_alpha = 255 - src.a; // 1(255) - alpha

		color_t color;
		color.r = (src.r * alpha + dest.r * inv_alpha) >> 8; // bitshift 8, = divide by 256
		color.g = (src.g * alpha + dest.g * inv_alpha) >> 8;
		color.b = (src.b * alpha + dest.b * inv_alpha) >> 8;
		color.a = src.a;

		return color;
	}

	color_t AdditiveBlend(const color_t& src, const color_t& dest)
	{
		color_t color;
		color.r = std::min(src.r + dest.r, 255); // std::min returns whichever is smaller
		color.g = std::min(src.g + dest.g, 255);
		color.b = std::min(src.b + dest.b, 255);
		color.a = src.a;

		return color;
	}

	color_t MultiplyBlend(const color_t& src, const color_t& dest)
	{
		color_t color;
		color.r = (src.r * dest.r) >> 8;
		color.g = (src.g * dest.g) >> 8;
		color.b = (src.b * dest.b) >> 8;
		color.a = src.a;

		return color;
	}
}