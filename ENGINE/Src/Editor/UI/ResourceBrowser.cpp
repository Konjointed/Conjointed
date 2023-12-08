#include <fstream>
#include <functional>

#include <tinyfiledialogs/tinyfiledialogs.h>

#include "ResourceBrowser.h"
#include "../Editor.h"

/*
std::string ImportFileDialog() {
    char* filePath = tinyfd_openFileDialog(
        "Import File",
        "",
        0,
        NULL,
        NULL,
        0
    );

    if (filePath) {
        return std::string(filePath);
    }
    return "";
}
*/
std::string ImportFileDialog() {
    char* folderPath = tinyfd_selectFolderDialog("Select Folder", "");

    if (folderPath) {
        return std::string(folderPath);
    }

    return "";
}

ResourceBrowser::ResourceBrowser() {}

void ResourceBrowser::DisplayDirectoryContents(const std::filesystem::path& directoryPath, TextEdit& textEdit) {
    static bool deleteFilePopup = false;
    static std::filesystem::path fileToDelete;

    // Recursive function to handle directory traversal and display
    std::function<void(const std::filesystem::path&)> handleDirectory = [&](const std::filesystem::path& path) {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (std::filesystem::is_directory(entry)) {
                if (ImGui::TreeNode(entry.path().filename().string().c_str())) {
                    // Recursively display contents of this directory
                    handleDirectory(entry.path());
                    ImGui::TreePop();
                }
            }
            else {
                // Perform an action based on the file extension
                // Example: Open a new text editor for .lua files
                std::string fileName = entry.path().filename().string();
                std::string fileExtension = entry.path().extension().string();
                if (ImGui::Selectable(fileName.c_str(), false)) {
                    if (fileExtension == ".lua") {
                        textEdit.Open(entry.path().string());
                    }
                    else if (fileExtension == ".vert" || fileExtension == ".frag" || fileExtension == ".geom") {
                        textEdit.Open(entry.path().string());
                    }
                }

                // Drag and drop for files
                if (ImGui::BeginDragDropSource()) {
                    std::string filePathStr = entry.path().string();
                    ImGui::SetDragDropPayload("MODEL_FILE", filePathStr.c_str(), filePathStr.size() + 1);
                    ImGui::Text("%s", filePathStr.c_str());
                    ImGui::EndDragDropSource();
                }

                // File deletion context menu
                if (ImGui::BeginPopupContextItem()) {
                    if (ImGui::MenuItem("Delete File")) {
                        deleteFilePopup = true;
                        fileToDelete = entry.path();
                    }
                    ImGui::EndPopup();
                }
            }
        }
        };

    // Start handling the directory contents
    handleDirectory(directoryPath);

    // Popups for file creation and deletion
    if (deleteFilePopup) {
        ImGui::OpenPopup("Delete File?");
    }

    // Confirmation popup for deleting the file
    if (ImGui::BeginPopupModal("Delete File?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure you want to delete this file?");
        ImGui::Text("%s", fileToDelete.filename().string().c_str());
        if (ImGui::Button("Yes")) {
            std::filesystem::remove(fileToDelete); // Delete the file
            deleteFilePopup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("No")) {
            deleteFilePopup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void ResourceBrowser::Draw(TextEdit& textEdit) {
    ImGui::Begin("Resource Browser");

    static bool createFilePopup = false;
    static char fileNameBuffer[128] = ""; // Buffer for file name input
    static std::filesystem::path newFilePath; // Path for new file creation

    std::string path = "Resources";
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_directory(entry)) {
            if (ImGui::TreeNode(entry.path().filename().string().c_str())) {
                // Context menu for directories
                if (ImGui::BeginPopupContextItem()) {
                    if (ImGui::MenuItem("New File")) {
                        createFilePopup = true;
                        newFilePath = entry.path();
                        std::fill(std::begin(fileNameBuffer), std::end(fileNameBuffer), 0);
                    }
                    if (ImGui::MenuItem("Import File")) {
                        std::string importedFolderPath = ImportFileDialog();
                        if (!importedFolderPath.empty()) {
                            // Determine the destination path
                            std::filesystem::path destination = entry.path() / std::filesystem::path(importedFolderPath).filename();

                            // Copy the entire directory tree
                            std::filesystem::copy(importedFolderPath, destination, std::filesystem::copy_options::recursive);
                        }
                    }
                    ImGui::EndPopup();
                }

                DisplayDirectoryContents(entry.path(), textEdit);
                ImGui::TreePop();
            }
        }
    }

    // Popup for creating a new file
    if (createFilePopup) {
        ImGui::OpenPopup("Create File");
        createFilePopup = false;
    }

    // Popup window to enter the file name
    if (ImGui::BeginPopupModal("Create File", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputText("File Name", fileNameBuffer, sizeof(fileNameBuffer));
        if (ImGui::Button("Create")) {
            // Create the file with the given name
            std::ofstream outfile(newFilePath / fileNameBuffer);
            outfile.close();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}
