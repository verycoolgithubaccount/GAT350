#pragma once
#include "VertexShader.h"
#include "FragmentShader.h"

class Shader
{
public:
	struct uniforms_t
	{
		// transformations
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		// lights
		light_t light;
		color3_t ambient;

		material_t material;
	};

	enum eFrontFace
	{
		CW,
		CCW
	};

	enum eCullMode
	{
		FRONT,
		BACK,
		NONE
	};

	static void Draw(const vertexbuffer_t& vb);
	static bool ToScreen(const vertex_output_t& vertex, glm::vec2& screen);

public:
	static class Framebuffer* framebuffer;
	static eFrontFace frontFace;
	static eCullMode cullMode;
	static uniforms_t uniforms;
};