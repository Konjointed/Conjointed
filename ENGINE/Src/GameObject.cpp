#include "GameObject.h"

GameObject::GameObject() : type(STATICMESH), name("Unnamed GameObject") {
	std::cout << "Constructing GameObject\n";
}

GameObject::GameObject(std::shared_ptr<Model> model)
	: model(model), type(SKINNEDMESH), name("Unnamed GameObject") {
	std::cout << "Constructing GameObject\n";
}

void GameObject::AddChild(std::unique_ptr<GameObject> child) {
	child->parent = this;
	children.push_back(std::move(child));
}

void GameObject::UpdateSelfAndChild() {
	if (transform.IsDirty()) {
		ForceUpdateSelfAndChild();
		return;
	}

	for (auto&& child : children)
	{
		child->UpdateSelfAndChild();
	}
}

void GameObject::DrawSelfAndChild(Shader shader, unsigned int shadowMapTexture) {
	// If this GameObject has a model, draw it
	if (model != nullptr) {
		// Set the transformation matrix for this object
		shader.SetMatrix4("model", transform.GetModelMatrix());

		// Draw the model
		model->Draw(shader, shadowMapTexture);
	}

	// Draw all children
	for (auto&& child : children) {
		child->DrawSelfAndChild(shader);
	}
}

void GameObject::ForceUpdateSelfAndChild() {
	if (parent) {
		transform.ComputeModelMatrix(parent->transform.GetModelMatrix());
	}
	else {
		transform.ComputeModelMatrix();
	}

	for (auto&& child : children) {
		child->ForceUpdateSelfAndChild();
	}
}

void GameObject::Destroy() {
	pendingDestruction = true;
}

bool GameObject::IsPendingDesturction() const {
	return pendingDestruction;
}

