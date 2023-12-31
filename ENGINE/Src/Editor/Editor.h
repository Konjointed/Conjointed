#ifndef GUI_H
#define GUI_H 

#include <vector>
#include <string>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>

#include "UI/EditorUI.h"
#include "../GameObject.h"

class Window;
struct Scene;

class Editor {
public:
	EditorUI* editorUI = nullptr;
	Window& window;

	Editor(Window& window);
	~Editor();

	void Draw(unsigned int textureColorBuffer, Scene& scene);
private:
};

#endif 
