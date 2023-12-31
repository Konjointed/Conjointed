#include <iostream>

#include "Window.h"

Window::Window(int& success, WindowProperties properties) : properties(properties) {
	success = 1;

	// Initialzie SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL error on initialization: " << SDL_GetError() << "\n";
		return;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// FIND OUT WHAT THESE DO
	//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create sdl window
	const Uint32 windowFlags = (SDL_WINDOW_OPENGL | (properties.fullscreen ? SDL_WINDOW_RESIZABLE : 0));
	window = SDL_CreateWindow(properties.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, properties.width, properties.height, windowFlags);
	if (!window) {
		std::cout << "SDL error on create window: " << SDL_GetError() << "\n";
		success = 0;
		return;
	}

	// Create opengl context
	glContext = SDL_GL_CreateContext(window);
	if (!glContext) {
		std::cout << "SDL GL error on create context: " << SDL_GetError() << "\n";
		SDL_DestroyWindow(window);
		SDL_Quit();
		success = 0;
		return;
	}

	// Load opengl functions Aand pointers
	if (!gladLoadGL()) {
		std::cout << "GLAD Initialization Error\n";
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return;
	}
}

Window::~Window() {
	std::cout << "Window Destroyed\n";
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Window::SwapBuffers() {
	SDL_GL_SwapWindow(window);
}

/*
void Window::UpdateSize(int width, int height) {
	properties.height = height;
	properties.width = width;
}
*/

int Window::GetWidth() {
	return properties.width;
}

int Window::GetHeight() {
	return properties.height;
}

SDL_Window* Window::GetWindow() {
	return window;
}

SDL_GLContext Window::GetContext() {
	return glContext;
}