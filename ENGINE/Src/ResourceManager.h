#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <vector>

class Texture;
class Shader;
class Model;

class ResourceManager {
public:
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture> textures;
	static std::map<std::string, Model> models;
	static std::map<std::string, std::tuple<std::string, std::string, std::string>> shaderFilePaths;

	static Shader LoadShader(std::string vsPath, std::string fsPath, std::string gsPath, std::string name);
	static Shader GetShader(std::string name);
	static void ReloadShader(const std::string& name) {
		if (shaderFilePaths.find(name) != shaderFilePaths.end()) {
			auto paths = shaderFilePaths[name];
			shaders[name] = LoadShaderFromFile(std::get<0>(paths), std::get<1>(paths), std::get<2>(paths));
		}
		else {
			// Handle the case where the shader name does not exist in the map
		}
	}

	static Texture LoadTexture(std::string path, std::string name);
	static Texture LoadCubemap(std::vector<std::string> faces, std::string name);
	static Texture GetTexture(std::string name);

	static Model LoadModel(std::string path, std::string name);
	static Model GetModel(std::string name);
private:
	ResourceManager() {}
	static Shader LoadShaderFromFile(std::string vsPath, std::string fsPath, std::string gsPath);
	static Shader LoadModelFromFile();

};

#endif 