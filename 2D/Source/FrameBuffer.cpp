#include "FrameBuffer.h"
#include "Renderer.h"
#include "MathUtils.h"
#include "Image.h"
#include "Color.h"

Framebuffer::Framebuffer(const Renderer& renderer, int width, int height)
{
	m_width = width;
	m_height = height;
	m_pitch = width * sizeof(color_t);

	m_texture = SDL_CreateTexture(renderer.GetRenderer(), SDL_PIXELFORMAT_RGBA32, //SDL_PIXELFORMAT_RGBA32 is SDL_PIXELFORMAT_ABGR8888
		SDL_TEXTUREACCESS_STREAMING, width, height); 

	if (!m_texture) std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;

	m_buffer.resize(m_width * m_height);
	m_depth.resize(m_width * m_height);
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
	std::fill(m_depth.begin(), m_depth.end(), std::numeric_limits<float>().max());
}

void Framebuffer::DrawPoint(int x, int y, const color_t& color)
{
	color_t& dest = m_buffer[x + y * m_width];
	dest = Color::ColorBlend(color, dest);
}

void Framebuffer::DrawPointClipped(int x, int y, const color_t& color)
{
	if (x >= m_width || x <= 0 || y <= 0 || y >= m_height) return;

	color_t& dest = m_buffer[x + y * m_width];
	dest = Color::ColorBlend(color, dest);
}

void Framebuffer::DrawRect(int x, int y, int w, int h, const color_t& color)
{
	if (x + w < 0 || y + h < 0 || x > m_width || y > m_height) return;
	int x1 = std::max(x, 0);
	int x2 = std::min(x + w, m_width);
	int y1 = std::max(y, 0);
	int y2 = std::min(y + h, m_height);

	for (int i = y1; i < y2; i++)
	{
		int index = x1 + i * m_width;
		std::fill(m_buffer.begin() + index, m_buffer.begin() + (index + x2 - x1), color);
	}
}

// Slope - intercept form
void Framebuffer::DrawLineSlope(int x1, int y1, int x2, int y2, const color_t& color)
{
	if (y1 > y2) std::swap(y1, y2);

	int dx = x2 - x1;
	int dy = y2 - y1;


	if (dx == 0)
	{
		for (int y = y1; y < y2; y++)
		{
			color_t& dest = m_buffer[x1 + y * m_width];
			dest = Color::ColorBlend(color, dest);
		}
		return;
	}

	float m = dy / (float) dx;
	float b = y1 - (m * x1);

	if (std::abs(dx) > std::abs(dy))
	{
		for (int x = x1; x < x2; x++)
		{
			int y = (int)round((m * x) + b);

			color_t& dest = m_buffer[x + y * m_width];
			dest = Color::ColorBlend(color, dest);
		}
	}
	else
	{
		for (int y = y1; y < y2; y++)
		{
			int x = (int)round((y - b) / m);
			color_t& dest = m_buffer[x + y * m_width];
			dest = Color::ColorBlend(color, dest);
		}
	}
}

// Bresenham's line algorithm
void Framebuffer::DrawLine(int x1, int y1, int x2, int y2, const color_t& color)
{
	// calculate deltas
	int dx = x2 - x1;
	int dy = y2 - y1;

	bool steep = (std::abs(dy) > std::abs(dx)); // if y increases faster than x
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	// Make sure it's drawing left to right
	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	// recalculate deltas
	dx = x2 - x1;
	dy = std::abs(y2 - y1);

	//error term and step direction
	int error = dx / 2;
	int ystep = (y1 < y2) ? 1 : -1;

	// draw
	for (int x = x1, y = y1; x <= x2; x++)
	{
		(steep) ? DrawPointClipped(y, x, color) : DrawPointClipped(x, y, color);
		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}

void Framebuffer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color)
{
	//if ((x1 < 0 && y1 < 0 && x2 < 0 && y2 < 0 && x3 < 0 && y3 < 0) || (x1 >= m_width && y1 >= m_height && x2 >= m_width && y2 >= m_height && x3 >= m_width && y3 >= m_height)) return;
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x3, y3, color);
	DrawLine(x3, y3, x1, y1, color);
}

void Framebuffer::DrawOctants(int xc, int yc, int x, int y, const color_t& color) {
	DrawPoint(xc + x, yc + y, color);
	DrawPoint(xc - x, yc + y, color);
	DrawPoint(xc + x, yc - y, color);
	DrawPoint(xc - x, yc - y, color);
	DrawPoint(xc + y, yc + x, color);
	DrawPoint(xc - y, yc + x, color);
	DrawPoint(xc + y, yc - x, color);
	DrawPoint(xc - y, yc - x, color);
}

// Function for circle-generation
// using Bresenham's algorithm
void Framebuffer::DrawCircle(int xc, int yc, int r, const color_t& color) {
	int x = 0, y = r;
	int d = 3 - 2 * r;
	DrawOctants(xc, yc, x, y, color);
	while (y >= x) {

		// check for decision parameter
		// and correspondingly 
		// update d, y
		if (d > 0) {
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;

		// Increment x after updating decision parameter
		x++;

		// Draw the circle using the new coordinates
		DrawOctants(xc, yc, x, y, color);
	}
}

void Framebuffer::DrawLinearCurve(int x1, int y1, int x2, int y2, const color_t& color)
{
	int steps = 10;
	float dt = (float) 1 / steps;
	float t1 = 0;
	for (int i = 0; i < steps; i++)
	{
		int sx1 = Math::Lerp(x1, x2, t1);
		int sy1 = Math::Lerp(y1, y2, t1);

		float t2 = t1 + dt;

		int sx2 = Math::Lerp(x1, x2, t2);
		int sy2 = Math::Lerp(y1, y2, t2);

		t1 += dt;
		DrawLine(sx1, sy1, sx2, sy2, color);
	}
}

void Framebuffer::DrawQuadraticCurve(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color)
{
	int steps = 50;
	float dt = (float) 1 / steps;
	float t1 = 0;
	for (int i = 0; i < steps; i++)
	{
		int sx1, sy1, sx2, sy2;
		Math::QuadraticPoint(x1, y1, x2, y2, x3, y3, t1, sx1, sy1);
		float t2 = t1 + dt;
		Math::QuadraticPoint(x1, y1, x2, y2, x3, y3, t2, sx2, sy2);

		t1 += dt;
		DrawLine(sx1, sy1, sx2, sy2, color);
	}
}

void Framebuffer::DrawCubicCurve(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, const color_t& color)
{
	int steps = 100;
	float dt = (float)1 / steps;
	float t1 = 0;
	for (int i = 0; i < steps; i++)
	{
		int sx1, sy1, sx2, sy2;
		Math::CubicPoint(x1, y1, x2, y2, x3, y3, x4, y4, t1, sx1, sy1);
		float t2 = t1 + dt;
		Math::CubicPoint(x1, y1, x2, y2, x3, y3, x4, y4, t2, sx2, sy2);

		t1 += dt;
		DrawLine(sx1, sy1, sx2, sy2, color);
	}
}

void Framebuffer::DrawImage(int x, int y, float scale, const class Image& image)
{
	DrawImage(x, y, scale, scale, image);
}

void Framebuffer::DrawImage(int x, int y, float scale_x, float scale_y, const class Image& image)
{
	// check if off-screen
	if (x + (image.GetWidth() * scale_x) < 0 || y + (image.GetHeight() * scale_y) < 0 || x > m_width || y > m_height) return;

	// iterate through image y
	for (int iy = 0; iy < image.GetHeight(); iy++)
	{
		// set screen y 
		int sy = y + (iy * scale_y);
		// check if off-screen, don't draw if off-screen
		if (sy < 0 || sy >= m_height) continue;

		// iterate through image x
		for (int ix = 0; ix < image.GetWidth(); ix++)
		{
			// set screen x
			int sx = x + (ix * scale_x);
			// check if off-screen, don't draw if off-screen
			if (sx < 0 || sx >= m_width) continue;

			// get image pixel color
			color_t color = image.GetBuffer()[ix + iy * image.GetWidth()];
			// check alpha, if 0 don't draw
			if (color.a == 0) continue;
			// set buffer to color
			DrawPoint(sx, sy, color);
		}
	}
}