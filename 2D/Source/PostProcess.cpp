#include "PostProcess.h"
#include "MathUtils.h"
#include <algorithm>
#include <iostream>

namespace PostProcess
{
	void PostProcess::Invert(std::vector<color_t>& buffer)
	{
		/*
		for (auto& color : buffer)
		{
			color.r = 255 - color.r;
			color.g = 255 - color.g;
			color.b = 255 - color.b;
		}
		*/

		std::for_each(buffer.begin(), buffer.end(), [](color_t& color)
			{
				color.r = 255 - color.r;
				color.g = 255 - color.g;
				color.b = 255 - color.b;
			});
	}

	void Monochrome(std::vector<color_t>& buffer)
	{
		std::for_each(buffer.begin(), buffer.end(), [](color_t& color)
			{
				uint8_t average = static_cast<uint8_t>((color.r + color.g + color.b) / 3);

				color.r = average;
				color.g = average;
				color.b = average;
			});
	}

	void Brightness(std::vector<color_t>& buffer, int brightness)
	{
		std::for_each(buffer.begin(), buffer.end(), [brightness](color_t& color)
			{
				color.r = static_cast<uint8_t>(Math::Clamp(color.r + brightness, 0, 255));
				color.g = static_cast<uint8_t>(Math::Clamp(color.g + brightness, 0, 255));
				color.b = static_cast<uint8_t>(Math::Clamp(color.b + brightness, 0, 255));
			});
	}

	void ColorBalance(std::vector<color_t>& buffer, int ro, int go, int bo)
	{
		std::for_each(buffer.begin(), buffer.end(), [ro, go, bo](color_t& color)
			{
				color.r = static_cast<uint8_t>(Math::Clamp(color.r + ro, 0, 255));
				color.g = static_cast<uint8_t>(Math::Clamp(color.g + go, 0, 255));
				color.b = static_cast<uint8_t>(Math::Clamp(color.b + bo, 0, 255));
			});
	}

	void Threshold(std::vector<color_t>& buffer, uint8_t threshold)
	{
		std::for_each(buffer.begin(), buffer.end(), [threshold](color_t& color)
			{
				if ((color.r + color.g + color.b) / 3 > threshold)
				{
					color.r = 255;
					color.g = 255;
					color.b = 255;
				}
				else
				{
					color.r = 0;
					color.g = 0;
					color.b = 0;
				}
			});
	}

	void Noise(std::vector<color_t>& buffer, uint8_t noise)
	{
		std::for_each(buffer.begin(), buffer.end(), [noise](color_t& color)
			{
				int offset = (rand() % ((noise * 2) + 1)) - noise;
				color.r = static_cast<uint8_t>(Math::Clamp(color.r + offset, 0, 255));
				color.g = static_cast<uint8_t>(Math::Clamp(color.g + offset, 0, 255));
				color.b = static_cast<uint8_t>(Math::Clamp(color.b + offset, 0, 255));
			});
	}

	void Posterize(std::vector<color_t>& buffer, uint8_t levels)
	{
		uint8_t level = 255 / levels;
		std::for_each(buffer.begin(), buffer.end(), [level](color_t& color)
			{
				color.r = (color.r / level) * level;
				color.g = (color.g / level) * level;
				color.b = (color.b / level) * level;
			});
	}

	void Edge(std::vector<color_t>& buffer, int width, int height, int threshold)
	{
		std::vector<color_t> source = buffer; // make a copy of the source to keep the original pixels for operation

		int hk[3][3] =
		{
			{1, 0, -1},
			{2, 0, -2},
			{1, 0, -1}
		};

		int vk[3][3] =
		{
			{-1, -2, -1},
			{0, 0, 0},
			{1, 2, 1}
		};

		for (int i = 0; i < buffer.size(); i++)
		{
			int x = i % width;
			int y = i / width;

			// skip if along an edge bc doesn't have some of the pixels in the kernel
			if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

			int h = 0;
			int v = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
					
					h += pixel.r * hk[iy][ix];
					v += pixel.r * vk[iy][ix];
				}
			}

			int m = static_cast<int>(std::sqrt(h * h + v * v));
			m = (m >= threshold) ? m : 0;

			uint8_t c = static_cast<uint8_t>(Math::Clamp(m, 0, 255));

			color_t& color = buffer[i];
			color.r = c;
			color.g = c;
			color.b = c;
		}
	}

	void Alpha(std::vector<color_t>& buffer, uint8_t alpha)
	{
		std::for_each(buffer.begin(), buffer.end(), [alpha](color_t& c)
			{
				c.a = alpha;
			});
	}

	void BoxBlur(std::vector<color_t>& buffer, int w, int h)
	{
		int kernel[3][3] =
		{
			{1, 1, 1},
			{1, 1, 1},
			{1, 1, 1}
		};

		ApplyKernel(buffer, w, h, kernel);
	}

	void GaussianBlur(std::vector<color_t>& buffer, int w, int h)
	{
		int kernel[3][3] =
		{
			{1, 2, 1},
			{2, 4, 2},
			{1, 2, 1}
		};

		ApplyKernel(buffer, w, h, kernel);
	}

	void Sharpen(std::vector<color_t>& buffer, int w, int h)
	{
		int kernel[3][3] =
		{
			{0, -1, 0},
			{-1, 5, -1},
			{0, -1, 0}
		};

		ApplyKernel(buffer, w, h, kernel);
	}

	void Emboss(std::vector<color_t>& buffer, int w, int h)
	{
		int kernel[3][3] =
		{
			{-2, -1, 0},
			{-1, 1, 1},
			{0, 1, 2}
		};

		ApplyKernel(buffer, w, h, kernel);
	}

	
	void ApplyKernel(std::vector<color_t>& buffer, int w, int h, int kernel[3][3])
	{
		std::vector<color_t> source = buffer; // make a copy of the source to keep the original pixels for operation

		int kernelSum = 0;
		for (int i = 0; i < 3; i ++)
		{
			for (int j = 0; j < 3; j++)
			{
				kernelSum += kernel[i][j];
			}
		}

		for (int i = 0; i < buffer.size(); i++)
		{
			int x = i % w;
			int y = i / w;

			// skip if along an edge bc doesn't have some of the pixels in the kernel
			if (x < 1 || x + 1 >= w || y < 1 || y + 1 >= h) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + (y + iy - 1) * w];
					int weight = kernel[iy][ix];

					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;
				}
			}

			color_t& color = buffer[i];
			color.r = static_cast<uint8_t>(Math::Clamp((r / kernelSum), 0, 255));
			color.g = static_cast<uint8_t>(Math::Clamp((g / kernelSum), 0, 255));
			color.b = static_cast<uint8_t>(Math::Clamp((b / kernelSum), 0, 255));
		}
	}
}

