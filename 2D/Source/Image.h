#pragma once
#include "Framebuffer.h"
#include <vector>
#include <string>

class Image
{
private:
	int m_width{ 0 };
	int m_height{ 0 };
	std::vector<color_t> m_buffer;
public:
	Image() = default;
	~Image() = default;

	bool Load(const std::string& filename);
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	const std::vector<color_t>& GetBuffer() const { return m_buffer; }
};