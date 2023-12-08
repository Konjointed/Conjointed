#include "GameObject.h""

GameObject::GameObject() : type(STATICMESH), name("Unnamed GameObject") {
	std::cout << "Constructing GameObject\n";
}

GameObject::GameObject(std::shared_ptr<Model> model)
	: model(model), type(SKINNEDMESH), name("Unnamed GameObject") {
	std::cout << "Constructing GameObject\n";
	boundingVolume = std::make_shared<AABB>(generateAABB(*model));
	//boundingVolume = std::make_shared<AABB>(generateSphereBV(*model));
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

void GameObject::DrawSelfAndChild(const Frustum& frustum, Shader shader, unsigned int shadowMapTexture, unsigned int& display, unsigned int& total) {
	if (model != nullptr) {
		if (boundingVolume->IsOnFrustum(frustum, transform)) {
			shader.SetMatrix4("model", transform.GetModelMatrix());
			model->Draw(shader, shadowMapTexture);
			display++;
		}
	}
	total++;

	for (auto&& child : children) {
		child->DrawSelfAndChild(frustum, shader, 0, display, total);
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

