#include "Shader.h"
#include "Framebuffer.h"
#include "Rasterizer.h"

Framebuffer* Shader::framebuffer{ nullptr };

void Shader::Draw(const vertexbuffer_t& vb)
{
	// vertex shader
	std::vector<vertex_output_t> outputVertices;
	outputVertices.reserve(vb.size()); // set the capacity to the size of the vertex buffer
	for (auto& vertex : vb)
	{
		vertex_output_t outputVertex;
		VertexShader::Process(vertex, outputVertex);
		outputVertices.push_back(outputVertex);
	}

	// primitive assembly (triangle)
	for (int i = 0; i < outputVertices.size(); i += 3)
	{
		vertex_output_t& v0 = outputVertices[i];
		vertex_output_t& v1 = outputVertices[i + 1];
		vertex_output_t& v2 = outputVertices[i + 2];

		// screen points
		glm::vec2 s0, s1, s2;
		if (!ToScreen(v0, s0)) continue;
		if (!ToScreen(v1, s1)) continue;
		if (!ToScreen(v2, s2)) continue;

		// rasterization
		Rasterizer::Triangle(*framebuffer, s0, s1, s2, v0, v1, v2);
		// framebuffer->DrawTriangle(s0.x, s0.y, s1.x, s1.y, s2.x, s2.y, { 255, 255, 255, 255 });
	}
}

bool Shader::ToScreen(const vertex_output_t& vertex, glm::vec2& screen)
{
	// prevent / 0
	if (vertex.position.w == 0) return false;

	// convert projection space to ndc (-1 <-> 1)
	glm::vec3 ndc = vertex.position / vertex.position.w;

	// don't draw if outside near and far clip plane
	if (ndc.z < -1 || ndc.z > 1) return false;

	screen.x = (ndc.x + 1) * (framebuffer->GetWidth() * 0.5f);
	screen.y = (1 - ndc.y) * (framebuffer->GetHeight() * 0.5f);

	return true;
}