#include <iostream>

#include "Scenegraph.h"
#include "../Editor.h"
#include "../../GameObject.h"
#include "../../Scene.h"

SceneGraph::SceneGraph() {}

SceneGraph::~SceneGraph() {}

void SceneGraph::Draw(Scene& scene) {
	if (ImGui::Begin("Scene Graph")) {
		ImGui::InputText("Search", searchQuery, IM_ARRAYSIZE(searchQuery));

		for (auto& object : scene.sceneObjects) {
			DisplaySceneGraph(*object);
		}
	}
	ImGui::End();
}

void SceneGraph::DisplaySceneGraph(GameObject& object) {
	// Start a tree node with a selectable label
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (selectedObject == &object) {
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool node_open = ImGui::TreeNodeEx(object.name.c_str(), node_flags);
	if (ImGui::IsItemClicked()) {
		selectedObject = &object;
	}

	// Right-click on the node to open context menu for removal
	if (ImGui::BeginPopupContextItem()) {
		if (ImGui::MenuItem("Remove")) {
			object.Destroy();
			selectedObject = nullptr;
		}
		ImGui::EndPopup();
	}

	if (node_open) {
		// Recursively display children
		for (auto&& child : object.children) {
			DisplaySceneGraph(*child);
		}
		ImGui::TreePop();
	}
}

GameObject* SceneGraph::GetSelectedObject() {
	return selectedObject;
}