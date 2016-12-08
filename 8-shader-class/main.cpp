#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <cmath>
#include "shader.hpp"

GLfloat vertices[] = {
  // Vertices         // Colors
  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
   0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
   0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

GLfloat rgbGreen()
{
  GLfloat time = glfwGetTime();
  return (sin(time) / 2) + 0.5;
}

/*
 * Setup Vertex Array Object to reuse vertex attributes and
 * vertices
 */
void createVertexArray(GLuint *VAO)
{
GLuint VBO;

  glGenVertexArrays(1, VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

int main()
{
  /*
   * Should set an OpenGL version and set window hints here
   */
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  /*
   * A window initialization
   */
  GLFWwindow *window = glfwCreateWindow(800, 600, "Learn OpenGL", nullptr, nullptr);
  if(window == nullptr)
  {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);

  /*
   * A GLEW initialization
   */
  glewExperimental = GL_TRUE;
  if(glewInit() != GLEW_OK)
  {
    printf("Failed to initialize GLEW\n");
    return 1;
  }

  /*
   * Should set an OpenGL viewport size here
   */
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  /*
   * Set the keyboard callbacks here
   */
  glfwSetKeyCallback(window, key_callback);

  GLuint VAO;
  createVertexArray(&VAO);

  Shader shader( "./vertexShader.glsl",
                 "./fragmentShader.glsl");

  /*
   * Main loop
   */
  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
