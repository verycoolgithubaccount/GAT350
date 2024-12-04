#include "FragmentShader.h"
#include "Shader.h"

color4_t FragmentShader::Process(const fragment_input_t& fragment)
{
	bool directional = false;

	glm::vec3 lightDir;
	//glm::vec3 vertexPosition = mv * glm::vec4{ fragment.position, 1 };

	if (!directional)
	{
		glm::vec3 lightPosition = Shader::uniforms.view * glm::vec4{ Shader::uniforms.light.position, 1 };
		lightDir = glm::normalize(lightPosition - fragment.position);
	}
	else
	{
		glm::vec3 lightDirection = Shader::uniforms.view * glm::vec4{ -Shader::uniforms.light.direction, 0 };
		lightDir = glm::normalize(lightDirection);
	}

	float intensity = std::max(0.0f, Math::Dot(lightDir, fragment.normal));

	color3_t diffuse = Shader::uniforms.light.color * intensity;

	color3_t specular{ 0 };

	if (intensity > 0)
	{
		glm::vec3 reflection = glm::reflect(-lightDir, fragment.normal); // Compute reflection vector
		glm::vec3 viewDirection = glm::normalize(-fragment.position); // Calculate view direction
		float specIntensity = std::max(Math::Dot(reflection, viewDirection), 0.0f); // Compute intensity of reflection
		specIntensity = std::pow(specIntensity, Shader::uniforms.material.shininess); // Apply shininess factor
		specular = Shader::uniforms.material.specular * specIntensity;
	}

	color3_t color = ((Shader::uniforms.ambient + diffuse) * Shader::uniforms.material.albedo) + specular;

	return color4_t{ color, 1 };
}


