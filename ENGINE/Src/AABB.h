#pragma once

#include <array>

#include <glm/vec3.hpp>

struct Frustum;
struct Plane;
class Transform;
class Model;

struct BoundingVolume {
	virtual bool IsOnFrustum(const Frustum& camFrustum, const Transform& transform) const = 0;
	virtual bool IsOnOrForwardPlane(const Plane& plane) const = 0;
	bool IsOnFrustum(const Frustum& camFrustum) const;
};

struct Sphere : public BoundingVolume
{
	glm::vec3 center{ 0.f, 0.f, 0.f };
	float radius{ 0.f };

	Sphere(const glm::vec3& inCenter, float inRadius)
		: BoundingVolume{}, center{ inCenter }, radius{ inRadius }
	{}

	bool IsOnOrForwardPlane(const Plane& plane) const final;
	bool IsOnFrustum(const Frustum& camFrustum, const Transform& transform) const final;
};

struct AABB : public BoundingVolume {
	glm::vec3 center{ 0.f, 0.f, 0.f };
	glm::vec3 extents{ 0.f, 0.f, 0.f };

	AABB(const glm::vec3& min, const glm::vec3& max)
		: BoundingVolume{}, center{ (max + min) * 0.5f }, extents{ max.x - center.x, max.y - center.y, max.z - center.z }
	{}

	AABB(const glm::vec3& inCenter, float iI, float iJ, float iK)
		: BoundingVolume{}, center{ inCenter }, extents{ iI, iJ, iK }
	{}

	std::array<glm::vec3, 8> getVertice() const;
	//see https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
	bool IsOnOrForwardPlane(const Plane& plane) const final;
	bool IsOnFrustum(const Frustum& camFrustum, const Transform& transform) const final;
};

AABB generateAABB(const Model& model);
Sphere generateSphereBV(const Model& model);