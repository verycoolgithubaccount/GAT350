#pragma once
#include "Framebuffer.h"

namespace PostProcess
{
	void Invert(std::vector<color_t>& buffer);
	void Monochrome(std::vector<color_t>& buffer);
	void Brightness(std::vector<color_t>& buffer, int brightness);
	void ColorBalance(std::vector<color_t>& buffer, int ro, int go, int bo);
	void Noise(std::vector<color_t>& buffer, uint8_t noise);
	void Threshold(std::vector<color_t>& buffer, uint8_t threshold);
	void Posterize(std::vector<color_t>& buffer, uint8_t levels);

	void Edge(std::vector<color_t>& buffer, int width, int height, int threshold);

	void BoxBlur(std::vector<color_t>& buffer, int w, int h);
	void GaussianBlur(std::vector<color_t>& buffer, int w, int h);
	void Sharpen(std::vector<color_t>& buffer, int w, int h);
	void ApplyKernel(std::vector<color_t>& buffer, int w, int h, int kernel[3][3]);
}