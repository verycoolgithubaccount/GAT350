#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "MathUtils.h"
#include <glm/gtx/color_space.hpp>
#include <SDL.h>
#include <algorithm>
#include <glm/glm.hpp>

using color_t = SDL_Color;
using color3_t = glm::vec3;
using color4_t = glm::vec4;

namespace Color
{
	inline color3_t HSVtoRGB(const glm::vec3& hsv)
	{
		return glm::rgbColor(hsv);
	}

	inline color3_t HSVtoRGB(float hue, float saturation, float value)
	{
		return glm::rgbColor(glm::vec3{ hue, saturation, value });
	}

	inline float LinearToGamma(float linear)
	{
		if (linear > 0) return std::sqrt(linear);
		return 0;
	}

	inline color_t Convert(const color4_t& color4)
	{
		color_t color;
		color.r = static_cast<uint8_t>(Math::Clamp(LinearToGamma(color4.r), 0.0f, 1.0f) * 255);
		color.g = static_cast<uint8_t>(Math::Clamp(LinearToGamma(color4.g), 0.0f, 1.0f) * 255);
		color.b = static_cast<uint8_t>(Math::Clamp(LinearToGamma(color4.b), 0.0f, 1.0f) * 255);
		color.a = static_cast<uint8_t>(Math::Clamp(color4.a, 0.0f, 1.0f) * 255);

		return color;
	}

	inline color_t Convert(const color3_t& color3)
	{
		color_t color;
		color.r = static_cast<uint8_t>(Math::Clamp(LinearToGamma(color3.r), 0.0f, 1.0f) * 255);
		color.g = static_cast<uint8_t>(Math::Clamp(LinearToGamma(color3.g), 0.0f, 1.0f) * 255);
		color.b = static_cast<uint8_t>(Math::Clamp(LinearToGamma(color3.b), 0.0f, 1.0f) * 255);
		color.a = 255;

		return color;
	}

	inline color4_t Convert(const color_t& color)
	{
		color4_t color4;
		color4.r = color.r / 255.0f;
		color4.g = color.g / 255.0f;
		color4.b = color.b / 255.0f;
		color4.a = color.a / 255.0f;

		return color4;
	}

	// color blending

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