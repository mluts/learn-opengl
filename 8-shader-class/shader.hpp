#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class Shader
{
public:
    GLuint program;

    Shader(
        const GLchar *vertexPath,
        const GLchar *fragmentPath
    );

    void use();
private:
    GLuint vertexShader, fragmentShader;
    void readVertex(const GLchar *path);
    void readFragment(const GLchar *path);
    void createProgram();
    void checkShaderCompilationStatus(GLuint shader);
};

#endif
