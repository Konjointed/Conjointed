#include "Frustum.h"
#include "Camera.h"

Frustum createFrustumFromCamera(const Camera& cam, float aspect, float fovY, float zNear, float zFar) {
	Frustum     frustum;
	const float halfVSide = zFar * tanf(fovY * .5f);
	const float halfHSide = halfVSide * aspect;
	const glm::vec3 frontMultFar = zFar * cam.front;

	glm::vec3 cameraPosition = cam.GetPosition();
	frustum.nearFace = { cameraPosition + zNear * cam.front, cam.front };
	frustum.farFace = { cameraPosition + frontMultFar, -cam.front };
	frustum.rightFace = { cameraPosition, glm::cross(frontMultFar - cam.right * halfHSide, cam.up) };
	frustum.leftFace = { cameraPosition, glm::cross(cam.up, frontMultFar + cam.right * halfHSide) };
	frustum.topFace = { cameraPosition, glm::cross(cam.right, frontMultFar - cam.up * halfVSide) };
	frustum.bottomFace = { cameraPosition, glm::cross(frontMultFar + cam.up * halfVSide, cam.right) };
	return frustum;
}