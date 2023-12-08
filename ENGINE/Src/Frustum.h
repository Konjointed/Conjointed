#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

class Camera;

struct Plane {
	glm::vec3 normal = { 0.f, 1.f, 0.f }; // unit vector
	float distance = 0.f; // Distance with origin

	Plane() = default;

	Plane(const glm::vec3& p1, const glm::vec3& norm)
		: normal(glm::normalize(norm)),
		distance(glm::dot(normal, p1))
	{}

	float GetSignedDistanceToPlane(const glm::vec3& point) const {
		return glm::dot(normal, point) - distance;
	}
};

struct Frustum {
	Plane topFace;
	Plane bottomFace;

	Plane rightFace;
	Plane leftFace;

	Plane farFace;
	Plane nearFace;
};

Frustum createFrustumFromCamera(const Camera& cam, float aspect, float fovY, float zNear, float zFar);