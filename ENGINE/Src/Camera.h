#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <SDL2/SDL_events.h>
#include <glm/ext/matrix_transform.hpp>

#include "GameObject.h"
#include "Editor/UI/Viewport.h"

struct Frustum;
class Shader;

class Camera : public GameObject {
public:
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float movementSpeed = 0.005f;
	float mouseSensitivity = 0.1f;
	float nearPlane = 0.1f;
	float farPlane = 500.0f;
	float fov = 70.0f;
	float aspectRatio = (float)Viewport::VIEWPORT_WIDTH / Viewport::VIEWPORT_HEIGHT;

	Camera(glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 45.0f, float pitch = 0.0f);
	~Camera();

	void UpdateSelfAndChild() override;
	void DrawSelfAndChild(const Frustum& frustum, Shader shader, unsigned int shadowMapTexture, unsigned int& display, unsigned int& total) override;

	void ProcessKeyboard(float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset);
	void Update(float deltaTime);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::vec3 GetFront();

	const glm::vec3& GetPosition() const {
		return transform.GetLocalPosition();
	}

	void SetPosition(const glm::vec3& newPosition) {
		transform.SetLocalPosition(newPosition);
	}
private:
	bool firstMouse = true;
	float lastX, lastY;
};

#include <Meta.h>

namespace meta {
	template <>
	inline auto registerMembers<Camera>()
	{
		return members(
			member("name", &GameObject::name),
			member("position", &Camera::GetPosition, &Camera::SetPosition),
			member("speed", &Camera::movementSpeed),
			member("nearPlane", &Camera::nearPlane),
			member("farPlane", &Camera::farPlane),
			member("fov", &Camera::fov),
			member("ratio", &Camera::aspectRatio)
		);
	}
}

#endif