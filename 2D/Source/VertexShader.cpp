#include "VertexShader.h"

VertexShader::uniforms_t VertexShader::uniforms =
{
	glm::mat4{ 1 }, // model (1 constructor is identity matrix)
	glm::mat4{ 1 }, // view
	glm::mat4{ 1 }, // projection
	color3_t{ 1 }
};

void VertexShader::Process(const vertex_t& inputVertex, vertex_output_t& outputVertex)
{
	bool directional = true;

	glm::mat4 mvp = uniforms.projection * uniforms.view * uniforms.model;
	outputVertex.position = mvp * glm::vec4{ inputVertex.position, 1 };

	glm::mat4 mv = uniforms.view * uniforms.model;
	outputVertex.normal = glm::normalize(glm::mat3{ mv } * inputVertex.normal);

	glm::vec3 lightDir;

	if (!directional)
	{
		glm::vec3 lightPosition = uniforms.view * glm::vec4{ uniforms.light.position, 1 };
		glm::vec3 vertexPosition = mv * glm::vec4{ inputVertex.position, 1 };
		lightDir = glm::normalize(lightPosition - vertexPosition);
	}
	else
	{
		glm::vec3 lightDirection = uniforms.view * glm::vec4{ uniforms.light.direction, 1 };
		lightDir = glm::normalize(-lightDirection);
	}

	float intensity = std::max(0.0f, Math::Dot(lightDir, outputVertex.normal));

	color3_t diffuse = uniforms.light.color * intensity;
	outputVertex.color = uniforms.ambient + diffuse;
}
