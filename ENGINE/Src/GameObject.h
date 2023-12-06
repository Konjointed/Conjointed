#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <list>
#include <memory>
#include <iostream>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Shader.h>

#include "Model.h"

//class Model;

enum ObjectType {
	NONE,
	ROOT,
	WORKSPACE,
	LIGHTING,
	STATICMESH,
	SKINNEDMESH,    
};

class Transform {
public:
	void ComputeModelMatrix() {
		modelMatrix = GetLocalModelMatrix();
		isDirty = false;
	}

	void ComputeModelMatrix(const glm::mat4& parentGlobalModelMatrix) {
		modelMatrix = parentGlobalModelMatrix * GetLocalModelMatrix();
		isDirty = false;
	}

	void SetLocalPosition(const glm::vec3& newPosition) {
		position = newPosition;
		isDirty = true;
	}

	void SetLocalRotation(const glm::vec3& newRotation) {
		eulerRotation = newRotation;
		isDirty = true;
	}

	void SetLocalScale(const glm::vec3& newScale) {
		scale = newScale;
		isDirty = true;
	}

	const glm::vec3& GetGlobalPosition() const { return modelMatrix[3]; }
	const glm::vec3& GetLocalPosition() const { return position; }
	const glm::vec3& GetLocalRotation() const { return eulerRotation; }
	const glm::vec3& GetLocalScale() const { return scale; }
	const glm::mat4& GetModelMatrix() const { return modelMatrix; }
	glm::vec3 GetRight() const { return modelMatrix[0]; }
	glm::vec3 GetUp() const { return modelMatrix[1]; }
	glm::vec3 GetBackward() const { return modelMatrix[2]; }
	glm::vec3 GetForward() const{ return -modelMatrix[2]; }
	glm::vec3 GetGlobalScale() const { return { glm::length(GetRight()), glm::length(GetUp()), glm::length(GetBackward()) }; }
	bool IsDirty() const { return isDirty; }

protected:
	// Local space information
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 eulerRotation = { 0.0f, 0.0f, 0.0f }; // Degrees
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

	// Global space information concatenate in matrix
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	bool isDirty = true;

	glm::mat4 GetLocalModelMatrix() {
		const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(eulerRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(eulerRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(eulerRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		// Y * X * Z
		const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

		// translation * rotation * scale (also know as TRS matrix)
		return glm::translate(glm::mat4(1.0f), position) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
	}
};

class  GameObject : public std::enable_shared_from_this<GameObject> {
public:
	// Scene graph
	std::list<std::unique_ptr<GameObject>> children;
	GameObject* parent = nullptr;

	// Space information
	Transform transform;

	ObjectType type;
	std::string name;
	std::shared_ptr<Model> model = nullptr;

	GameObject();
	GameObject(std::shared_ptr<Model> model);

	/*
	template<typename... TArgs>
	void AddChild(TArgs&... args) {
		children.emplace_back(std::make_unique<GameObject>(args...));
		children.back()->parent = this;
	}
	*/
	virtual void UpdateSelfAndChild();
	virtual void DrawSelfAndChild(Shader shader, unsigned int shadowMapTexture = 0);

	void AddChild(std::unique_ptr<GameObject> child);
	void Destroy();
	bool IsPendingDesturction() const;

	const glm::vec3& GetPosition() const {
		return transform.GetLocalPosition();
	}

	void SetPosition(const glm::vec3& newPos) {
		transform.SetLocalPosition(newPos);
	}

	const glm::vec3& GetRotation() const {
		return transform.GetLocalRotation();
	}

	void SetRotation(const glm::vec3& newRotation) {
		transform.SetLocalRotation(newRotation);
	}

	const glm::vec3& GetScale() const {
		return transform.GetLocalScale();
	}

	void SetScale(const glm::vec3& newScale) {
		transform.SetLocalScale(newScale);
	}

	const std::string& GetModel() const {
		return "path/to/model";
	}

	void SetModel(const std::string& path) {
		model = std::make_shared<Model>(path, false);
	}
private:
	bool pendingDestruction = false;

	void ForceUpdateSelfAndChild();
};

#include <Meta.h>

namespace meta {
	/*
	template <>
	inline auto registerMembers<Transform>()
	{
		return members(
			member("position", &Transform::GetLocalPosition, &Transform::SetLocalPosition),
			member("rotation", &Transform::GetLocalRotation, &Transform::SetLocalRotation),
			member("scale", &Transform::GetLocalScale, &Transform::SetLocalScale)
			);
	}

	template <>
	inline auto registerMembers<GameObject>()
	{
		return std::tuple_cat(
			meta::getMembers<Transform>(),
			members(member("name", &GameObject::name))
			//member("parent", &GameObject::parent),
			//member("model", &GameObject::model)
		);
	}
	*/

	template <>
	inline auto registerMembers<GameObject>()
	{
		return members(
			member("position", &GameObject::GetPosition, &GameObject::SetPosition),
			member("rotation", &GameObject::GetRotation, &GameObject::SetRotation),
			member("scale", &GameObject::GetScale, &GameObject::SetScale),
			member("name", &GameObject::name),
			member("model", &GameObject::GetModel, &GameObject::SetModel)
		);
	}
}

#endif 