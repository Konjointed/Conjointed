#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <SDL2/SDL_events.h>
#include <glm/ext/matrix_transform.hpp>

#include "GameObject.h"
#include "Editor/UI/Viewport.h"

class Camera : public GameObject {
public:
	float movementSpeed = 1.0f;
	float nearPlane = 0.1f;
	float farPlane = 5000.0f;
	float fov = 45.0f;
	float aspectRatio = (float)Viewport::VIEWPORT_WIDTH / Viewport::VIEWPORT_HEIGHT;

	Camera(glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 45.0f, float pitch = 0.0f);
	~Camera();

	void UpdateSelfAndChild() override;
	void DrawSelfAndChild(Shader shader, unsigned int shadowMapTexture) override;

	void ProcessKeyboard(float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset);
	void Update(float deltaTime);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::vec3 GetFront();
private:
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	//float movementSpeed = 30.0f;
	float mouseSensitivity = 0.1f;
	bool firstMouse = true;
	float lastX, lastY;
};

#include <Meta.h>

namespace meta {
	template <>
	inline auto registerMembers<Camera>()
	{
		return members(
			member("name", &Camera::name),
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