/*
#ifndef SHADER_RELOADER_H
#define SHADER_RELOADER_H

#include <cassert>

#include "IncludeGL.h"

class ReloadableShader {
public:
	unsigned int CreateShaderProgramFromStrings(const char* vertexShaderStr, const char* fragmentShaderStr, const char* geometryShaderStr) {
		assert(vertexShaderStr && fragmentShaderStr);

		unsigned int shaderProgram = glCreateProgram();
		unsigned int vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
		unsigned int geometryShaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
		{
			glShaderSource(vertexShaderHandle, 1, &vertexShaderStr, NULL);
			glCompileShader(vertexShaderHandle);
			int params = -1;
			glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &params);
		}
	}

	unsigned int CreateShaderProgramFromFiles() {}

	void ReloadShaderProgramFromFiles() {}
private:
};

#endif 
*/