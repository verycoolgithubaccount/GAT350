#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/glm.hpp>

struct Transform
{
	glm::vec3 position{ 0 };
	glm::vec3 rotation{ 0 };
	glm::vec3 scale{ 1 };

	Transform() = default;

	Transform(const glm::vec3& position,
		const glm::vec3& rotation = glm::vec3{ 0 },
		const glm::vec3& scale = glm::vec3{ 1 }) :
		position{ position }, rotation{ rotation }, scale{ scale }
	{}

	glm::mat4 GetMatrix()
	{
		glm::mat4 mxs = glm::scale(scale);
		glm::mat4 mxt = glm::translate(position);
		glm::mat4 mxr = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z)); // yaw, pitch, roll in that order

		glm::mat4 mx = mxt * mxs * mxr; // rotation * scale * transform in that order, it goes backwards for some reason

		return mx;
	}

	glm::vec3 GetForward() const
	{
		glm::mat4 mxr = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));

		return mxr * glm::vec4{ 0, 0, 1, 0 };
	}

	glm::vec3 GetUp() const
	{
		glm::mat4 mxr = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));

		return mxr * glm::vec4{ 0, 1, 0, 0 };
	}

	glm::vec3 GetRight() const
	{
		glm::mat4 mxr = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));

		return mxr * glm::vec4{ 1, 0, 0, 0 };
	}

	glm::vec4 operator * (const glm::vec4& v)
	{
		return GetMatrix() * v;
	}
};