#include "AABB.h"

#include "Frustum.h"
#include "GameObject.h"
#include "Model.h"

bool BoundingVolume::IsOnFrustum(const Frustum& camFrustum) const {
    return (IsOnOrForwardPlane(camFrustum.leftFace) &&
        IsOnOrForwardPlane(camFrustum.rightFace) &&
        IsOnOrForwardPlane(camFrustum.topFace) &&
        IsOnOrForwardPlane(camFrustum.bottomFace) &&
        IsOnOrForwardPlane(camFrustum.nearFace) &&
        IsOnOrForwardPlane(camFrustum.farFace));
}

bool Sphere::IsOnOrForwardPlane(const Plane& plane) const {
	return plane.GetSignedDistanceToPlane(center) > -radius;
}

bool Sphere::IsOnFrustum(const Frustum& camFrustum, const Transform& transform) const
{
	//Get global scale thanks to our transform
	const glm::vec3 globalScale = transform.GetGlobalScale();

	//Get our global center with process it with the global model matrix of our transform
	const glm::vec3 globalCenter{ transform.GetModelMatrix() * glm::vec4(center, 1.f) };

	//To wrap correctly our shape, we need the maximum scale scalar.
	const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);

	//Max scale is assuming for the diameter. So, we need the half to apply it to our radius
	Sphere globalSphere(globalCenter, radius * (maxScale * 0.5f));

	//Check Firstly the result that have the most chance to failure to avoid to call all functions.
	return (globalSphere.IsOnOrForwardPlane(camFrustum.leftFace) &&
		globalSphere.IsOnOrForwardPlane(camFrustum.rightFace) &&
		globalSphere.IsOnOrForwardPlane(camFrustum.farFace) &&
		globalSphere.IsOnOrForwardPlane(camFrustum.nearFace) &&
		globalSphere.IsOnOrForwardPlane(camFrustum.topFace) &&
		globalSphere.IsOnOrForwardPlane(camFrustum.bottomFace));
};

std::array<glm::vec3, 8> AABB::getVertice() const {
	std::array<glm::vec3, 8> vertice;
	vertice[0] = { center.x - extents.x, center.y - extents.y, center.z - extents.z };
	vertice[1] = { center.x + extents.x, center.y - extents.y, center.z - extents.z };
	vertice[2] = { center.x - extents.x, center.y + extents.y, center.z - extents.z };
	vertice[3] = { center.x + extents.x, center.y + extents.y, center.z - extents.z };
	vertice[4] = { center.x - extents.x, center.y - extents.y, center.z + extents.z };
	vertice[5] = { center.x + extents.x, center.y - extents.y, center.z + extents.z };
	vertice[6] = { center.x - extents.x, center.y + extents.y, center.z + extents.z };
	vertice[7] = { center.x + extents.x, center.y + extents.y, center.z + extents.z };
	return vertice;
}

//see https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
bool AABB::IsOnOrForwardPlane(const Plane& plane) const {
	// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
	const float r = extents.x * std::abs(plane.normal.x) + extents.y * std::abs(plane.normal.y) +
		extents.z * std::abs(plane.normal.z);

	return -r <= plane.GetSignedDistanceToPlane(center);
}

bool AABB::IsOnFrustum(const Frustum& camFrustum, const Transform& transform) const {
	//Get global scale thanks to our transform
	const glm::vec3 globalCenter{ transform.GetModelMatrix() * glm::vec4(center, 1.f) };

	// Scaled orientation
	const glm::vec3 right = transform.GetRight() * extents.x;
	const glm::vec3 up = transform.GetUp() * extents.y;
	const glm::vec3 forward = transform.GetForward() * extents.z;

	const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
		std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
		std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

	const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

	const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
		std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

	const AABB globalAABB(globalCenter, newIi, newIj, newIk);

	return (globalAABB.IsOnOrForwardPlane(camFrustum.leftFace) &&
		globalAABB.IsOnOrForwardPlane(camFrustum.rightFace) &&
		globalAABB.IsOnOrForwardPlane(camFrustum.topFace) &&
		globalAABB.IsOnOrForwardPlane(camFrustum.bottomFace) &&
		globalAABB.IsOnOrForwardPlane(camFrustum.nearFace) &&
		globalAABB.IsOnOrForwardPlane(camFrustum.farFace));
};

AABB generateAABB(const Model& model) {
	glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::min());
	for (auto&& mesh : model.meshes)
	{
		for (auto&& vertex : mesh.vertices)
		{
			minAABB.x = std::min(minAABB.x, vertex.position.x);
			minAABB.y = std::min(minAABB.y, vertex.position.y);
			minAABB.z = std::min(minAABB.z, vertex.position.z);

			maxAABB.x = std::max(maxAABB.x, vertex.position.x);
			maxAABB.y = std::max(maxAABB.y, vertex.position.y);
			maxAABB.z = std::max(maxAABB.z, vertex.position.z);
		}
	}
	return AABB(minAABB, maxAABB);
}

Sphere generateSphereBV(const Model& model)
{
	glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::min());
	for (auto&& mesh : model.meshes)
	{
		for (auto&& vertex : mesh.vertices)
		{
			minAABB.x = std::min(minAABB.x, vertex.position.x);
			minAABB.y = std::min(minAABB.y, vertex.position.y);
			minAABB.z = std::min(minAABB.z, vertex.position.z);

			maxAABB.x = std::max(maxAABB.x, vertex.position.x);
			maxAABB.y = std::max(maxAABB.y, vertex.position.y);
			maxAABB.z = std::max(maxAABB.z, vertex.position.z);
		}
	}

	return Sphere((maxAABB + minAABB) * 0.5f, glm::length(minAABB - maxAABB));
}
