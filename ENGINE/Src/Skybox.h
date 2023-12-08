#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>

#include <glm/mat4x4.hpp>

#include "GameObject.h"
#include "ResourceManager.h"
#include "Texture.h"

struct Frustum;
class Shader;

class Skybox : public GameObject {
public:
	std::string textureRight;
	std::string textureLeft;
	std::string textureTop;
	std::string textureBottom;
	std::string textureFront;
	std::string textureBack;

	Skybox();
	~Skybox();

    void DrawSelfAndChild(const Frustum& frustum, Shader shader, unsigned int shadowMapTexture, unsigned int& display, unsigned int& total) override;

	void UpdateTexture() {
		ResourceManager::LoadCubemap({
			textureRight,
			textureLeft,
			textureTop,
			textureBottom,
			textureFront,
			textureBack
			}, "skybox");
	}

    const std::string& GetTextureRight() const { return textureRight; }
    const std::string& GetTextureLeft() const { return textureLeft; }
    const std::string& GetTextureTop() const { return textureTop; }
    const std::string& GetTextureBottom() const { return textureBottom; }
    const std::string& GetTextureFront() const { return textureFront; }
    const std::string& GetTextureBack() const { return textureBack; }

    void SetTextureRight(const std::string& path) {
        if (textureRight != path) {
            textureRight = path;
            texturesUpdated = true;
        }
    }
    void SetTextureLeft(const std::string& path) {
        if (textureLeft != path) {
            textureLeft = path;
            texturesUpdated = true;
        }
    }
    void SetTextureTop(const std::string& path) {
        if (textureTop != path) {
            textureTop = path;
            texturesUpdated = true;
        }
    }
    void SetTextureBottom(const std::string& path) {
        if (textureBottom != path) {
            textureBottom = path;
            texturesUpdated = true;
        }
    }
    void SetTextureFront(const std::string& path) {
        if (textureFront != path) {
            textureFront = path;
            texturesUpdated = true;
        }
    }
    void SetTextureBack(const std::string& path) {
        if (textureBack != path) {
            textureBack = path;
            texturesUpdated = true;
        }
    }
private:
	unsigned int vao;
	unsigned int vbo;
	bool texturesUpdated = false;

	std::vector<float> vertices;
};

#include <Meta.h>

namespace meta {
	template <>
	inline auto registerMembers<Skybox>()
	{
		return members(
			member("name", &Skybox::name),
			member("Skybox Right", &Skybox::GetTextureRight, &Skybox::SetTextureRight),
			member("Skybox Left", &Skybox::GetTextureLeft, &Skybox::SetTextureLeft),
			member("Skybox Top", &Skybox::GetTextureTop, &Skybox::SetTextureTop),
			member("Skybox Bottom", &Skybox::GetTextureBottom, &Skybox::SetTextureBottom),
			member("Skybox Front", &Skybox::GetTextureFront, &Skybox::SetTextureFront),
			member("Skybox Back", &Skybox::GetTextureBack, &Skybox::SetTextureBack)
		);
	}
}

#endif 