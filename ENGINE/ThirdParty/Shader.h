#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Src/IncludeGL.h"

class Shader
{
public:
    // state
    unsigned int ID;
    std::string vertexPath, fragmentPath, geometryPath;
    // constructor
    Shader() {}
    // sets the current shader as active
    Shader& Use();
    // compiles the shader from given source code
    void    Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr); // note: geometry source code is optional 
    void Recompile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr)
    {
        // Clean up the existing shader program if necessary
        // ...

        // Compile the new shader
        Compile(vertexSource, fragmentSource, geometrySource);
    }
    void SetFilePaths(const std::string& vsPath, const std::string& fsPath, const std::string& gsPath = "")
    {
        vertexPath = vsPath;
        fragmentPath = fsPath;
        geometryPath = gsPath;
    }
    // utility functions
    void    SetFloat(const char* name, float value, bool useShader = false);
    void    SetInteger(const char* name, int value, bool useShader = false);
    void    SetVector2f(const char* name, float x, float y, bool useShader = false);
    void    SetVector2f(const char* name, const glm::vec2& value, bool useShader = false);
    void    SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
    void    SetVector3f(const char* name, const glm::vec3& value, bool useShader = false);
    void    SetVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
    void    SetVector4f(const char* name, const glm::vec4& value, bool useShader = false);
    void    SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);
private:
    // checks if compilation or linking failed and if so, print the error logs
    void    checkCompileErrors(unsigned int object, std::string type);
};

#endif