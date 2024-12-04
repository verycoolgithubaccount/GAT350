#include "VertexShader.h"
#include "Shader.h"

//#define GOURAUD

void VertexShader::Process(const vertex_t& inputVertex, vertex_output_t& outputVertex)
{
	bool directional = true;

	glm::mat4 mvp = Shader::uniforms.projection * Shader::uniforms.view * Shader::uniforms.model;
	outputVertex.position = mvp * glm::vec4{ inputVertex.position, 1 };

	glm::mat4 mv = Shader::uniforms.view * Shader::uniforms.model;
	outputVertex.normal = glm::normalize(glm::mat3{ mv } * inputVertex.normal);

	outputVertex.vertexPosition = mv * glm::vec4{ inputVertex.position, 1 };

#ifdef GOURAUD
	glm::vec3 lightDir;
	glm::vec3 vertexPosition = mv * glm::vec4{ inputVertex.position, 1 };

	if (!directional)
	{
		glm::vec3 lightPosition = Shader::uniforms.view * glm::vec4{ Shader::uniforms.light.position, 1 };
		lightDir = glm::normalize(lightPosition - vertexPosition);
	}
	else
	{
		glm::vec3 lightDirection = Shader::uniforms.view * glm::vec4{ -Shader::uniforms.light.direction, 0 };
		lightDir = glm::normalize(lightDirection);
	}

	float intensity = std::max(0.0f, Math::Dot(lightDir, outputVertex.normal));

	color3_t diffuse = Shader::uniforms.light.color * intensity;

	color3_t specular{ 0 };

	if (intensity > 0)
	{
		glm::vec3 reflection = glm::reflect(-lightDir, outputVertex.normal); // Compute reflection vector
		glm::vec3 viewDirection = glm::normalize(-vertexPosition); // Calculate view direction
		float specIntensity = std::max(Math::Dot(reflection, viewDirection), 0.0f); // Compute intensity of reflection
		specIntensity = std::pow(specIntensity, Shader::uniforms.material.shininess); // Apply shininess factor
		specular = Shader::uniforms.material.specular * specIntensity;
	}

	outputVertex.color = ((Shader::uniforms.ambient + diffuse) * Shader::uniforms.material.albedo) + specular;
#endif // 0
}
