#include <iostream>
#include <memory>

#include "Properties.h"
#include "../Editor.h"
#include "../../Scene.h"

Properties::Properties() {}
Properties::~Properties() {}

void Properties::CameraProperties(Camera* camera) {
    // Use MetaStuff to iterate over properties
    meta::doForAllMembers<Camera>([camera](const auto& member) {
        using MemberType = meta::get_member_type<decltype(member)>;
        MemberType value = member.get(*camera);

        // Create ImGui elements based on member type
        if constexpr (std::is_same<MemberType, float>::value) {
            if (ImGui::DragFloat(member.getName(), &value, 0.1f, -FLT_MAX, FLT_MAX, "%.3f")) {
                member.set(*camera, value);
            }
        }
        else if constexpr (std::is_same<MemberType, glm::vec3>::value) {
            // Handle glm::vec3 type
            if (ImGui::DragFloat3(member.getName(), &value[0])) {
                member.set(*camera, value);
            }
        }
    });
}

void Properties::SkyboxProperties(Skybox* skybox) {
    meta::doForAllMembers<Skybox>([skybox](const auto& member) {
        using MemberType = meta::get_member_type<decltype(member)>;
        MemberType value = member.get(*skybox);

        // Create ImGui elements based on member type
        if constexpr (std::is_same<MemberType, float>::value) {
            if (ImGui::DragFloat(member.getName(), &value, 0.1f, -FLT_MAX, FLT_MAX, "%.3f")) {
                member.set(*skybox, value);
            }
        }
        else if constexpr (std::is_same<MemberType, glm::vec3>::value) {
            // Handle glm::vec3 type
            if (ImGui::DragFloat3(member.getName(), &value[0])) {
                member.set(*skybox, value);
            }
        }
        // Handle std::string type type
        else if constexpr (std::is_same<MemberType, std::string>::value) {
            char buffer[1024];
            strncpy_s(buffer, value.c_str(), sizeof(buffer));
            buffer[sizeof(buffer) - 1] = '\0'; // Ensure null termination

            if (ImGui::InputText(member.getName(), buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                std::string newPath(buffer);

                // Check if the path is valid (e.g., file exists) before setting
                if (std::filesystem::exists(newPath)) {
                    member.set(*skybox, newPath);
                }
            }
        }
        });
}

void Properties::ObjectProperties(GameObject* object) {
    // Use MetaStuff to iterate over properties of GameObject
    meta::doForAllMembers<GameObject>([object](const auto& member) {
        using MemberType = meta::get_member_type<decltype(member)>;
        MemberType value = member.get(*object);

        // Handle float type
        if constexpr (std::is_same<MemberType, float>::value) {
            if (ImGui::DragFloat(member.getName(), &value, 0.1f, -FLT_MAX, FLT_MAX, "%.3f")) {
                member.set(*object, value);
            }
        }
        else if constexpr (std::is_same<MemberType, bool>::value) {
            if (ImGui::Checkbox(member.getName(), &value)) {
                member.set(*object, value);
            }
        }
        // Handle glm::vec3 type
        else if constexpr (std::is_same<MemberType, glm::vec3>::value) {
            if (ImGui::DragFloat3(member.getName(), &value[0])) {
                member.set(*object, value);
            }
        }
        // Handle std::string type type
        else if constexpr (std::is_same<MemberType, std::string>::value) {
            char buffer[1024];
            strncpy_s(buffer, value.c_str(), sizeof(buffer));
            buffer[sizeof(buffer) - 1] = '\0'; // Ensure null termination

            if (ImGui::InputText(member.getName(), buffer, sizeof(buffer))) {
                member.set(*object, std::string(buffer));
            }
        }
        });

    // Drag and Drop for model files
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MODEL_FILE")) {
            // Assuming payload data is the path to the model file
            const char* path = static_cast<const char*>(payload->Data);

            // Set the model for the GameObject
            object->SetModel(std::string(path));
        }
        ImGui::EndDragDropTarget();
    }
}

void Properties::Draw(GameObject* selectedObject, Scene& scene) {
	ImGui::Begin("Properties");
	ImGui::InputText("Search", searchQuery, IM_ARRAYSIZE(searchQuery));

	if (selectedObject) {
        Camera* camera = dynamic_cast<Camera*>(selectedObject);
        Skybox* skybox = dynamic_cast<Skybox*>(selectedObject);

        if (camera) {
            CameraProperties(camera);
        }
        else if (skybox) {
            SkyboxProperties(skybox);
        }
        else {
            // I'M NOT PROUD OF THIS SOLUTION, BUT OTHER METHODS CONFLICTED WITH RENDERING :(
            if (selectedObject->name == "Workspace") {
                ImGui::Checkbox("Wireframe Mode", &scene.wireframe);
                //ImGui::DragFloat3("Light Position", &scene.lightPosition[0]);
                //ImGui::DragFloat("Near", &scene.nearPlane);
                //ImGui::DragFloat("Far", &scene.farPlane);
                //ImGui::DragFloat("Ortho Size", &scene.orthoSize);
                ImGui::DragFloat3("Light Direction", &scene.lightDirection[0]);
            }

            ObjectProperties(selectedObject);
        }
	}

	ImGui::End();
}