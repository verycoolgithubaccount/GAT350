#pragma once
#include "Framebuffer.h"

namespace PostProcess
{
	void Invert(std::vector<color_t>& buffer);
	void Monochrome(std::vector<color_t>& buffer);
	void Brightness(std::vector<color_t>& buffer, int brightness);

	void Edge(std::vector<color_t>& buffer, int width, int height, int threshold);

	void BoxBlur(std::vector<color_t>& buffer, int w, int h);
	void GaussianBlur(std::vector<color_t>& buffer, int w, int h);
	void Sharpen(std::vector<color_t>& buffer, int w, int h);
	void ApplyKernel(std::vector<color_t>& buffer, int w, int h, int kernel[3][3]);
}