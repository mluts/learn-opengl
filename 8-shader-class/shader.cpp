#include "shader.hpp"

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
{
  createProgram(vertexPath, fragmentPath);
}

void Shader::use()
{
  glUseProgram(program);
}

void Shader::readVertex(const GLchar *path)
{
std::ifstream file;
std::stringstream strstream;
const GLchar *source;

  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try
  {
    file.open(path);
    strstream << file.rdbuf();
    file.close();
  }
  catch(std::ifstream::failure e)
  {
    std::cout << "Can't read vertex shader file" << std::endl;
    return;
  }

  source = strstream.str().c_str();

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &source, NULL);
  glCompileShader(vertexShader);
  checkShaderCompilationStatus(vertexShader);
}

void Shader::readFragment(const GLchar *path)
{
std::ifstream file;
std::stringstream strstream;
const GLchar *source;

  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try
  {
    file.open(path);
    strstream << file.rdbuf();
    file.close();
  }
  catch(std::ifstream::failure e)
  {
    std::cout << "Can't read fragment shader file" << std::endl;
    return;
  }

  source = strstream.str().c_str();
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &source, NULL);
  glCompileShader(fragmentShader);
  checkShaderCompilationStatus(fragmentShader);
}

void Shader::checkShaderCompilationStatus(GLuint shader)
{
GLint success;
GLchar log[512];

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(vertexShader, sizeof(log), NULL, log);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << log
              << std::endl;
  }
}

void Shader::createProgram(const GLchar *vertexPath, const GLchar *fragmentPath)
{
GLint success;
GLchar log[512];

  readVertex(vertexPath);
  readFragment(fragmentPath);

  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetProgramInfoLog(program, sizeof(log), NULL, log);
    std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n"
               << log
               << std::endl;
    return;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}
