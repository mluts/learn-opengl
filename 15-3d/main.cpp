#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "../soil/src/SOIL.h"

GLfloat vertices[] = {
   // Vertices        // Colors         // Tex Coords
   0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
   0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

GLuint indices[] = {
  0, 1, 2,
  3, 0, 2
};

GLfloat velocity = 50.0f,
        newVelocity = 50.0f,
        acceleration = 30.0f,
        angle = 0.0f;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  if(action != GLFW_PRESS)
    return;

  switch(key) {
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, GL_TRUE);
      break;
    case GLFW_KEY_UP:
      newVelocity += 30.0;
      break;
    case GLFW_KEY_DOWN:
      newVelocity -= 30.0;
      break;
    case GLFW_KEY_ENTER:
      newVelocity = 0;
      break;
  }
}

void syncVelocity(GLfloat t1, GLfloat t2)
{
  GLfloat diff = newVelocity - velocity,
          minus = diff < 0 ? -1 : 1;

  if(abs(diff) == 0)
    return;

  if(abs(diff) < 0.01) {
    velocity = newVelocity;
  } else if(abs(diff) < acceleration) {
    velocity += diff / 3.0;
  } else {
    velocity += (t2 - t1) * acceleration * minus;
  }
}

GLfloat nextangle()
{
  static GLfloat t1 = 0, t2 = 0;
  if(t2 == 0)
    t2 = glfwGetTime();
  t1 = t2;
  t2 = glfwGetTime();

  syncVelocity(t1, t2);
  angle += (t2 - t1) * velocity;
  return angle;
}

/*
 * Setup Vertex Array Object to reuse vertex attributes and
 * vertices
 */
void createVertexArray(GLuint *VAO)
{
GLuint VBO, EBO;

  glGenVertexArrays(1, VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void loadTexture(const char *path, GLenum texname, GLuint *texture)
{
  int width = 0, height = 0;

  glActiveTexture(texname);

  unsigned char *image = SOIL_load_image(
      path, &width, &height,
      0, SOIL_LOAD_RGB
      );

  glGenTextures(1, texture);
  glBindTexture(GL_TEXTURE_2D, *texture);
  glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGB,
      width, height, 0, GL_RGB,
      GL_UNSIGNED_BYTE, image
      );
  glGenerateMipmap(GL_TEXTURE_2D);

  SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0);
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

  Shader shader( "./vertexShader.glsl",
                 "./fragmentShader.glsl");
  GLuint VAO;
  createVertexArray(&VAO);

  GLuint texture1, texture2;
  loadTexture("./container.jpg", GL_TEXTURE0, &texture1);
  loadTexture("./awesomeface.png", GL_TEXTURE1, &texture2);

  glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);

  /*
   * Main loop
   */
  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    glm::mat4 model, view, projection;
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, .0f, .0f));
    view = glm::translate(view, glm::vec3(.0f, .5f, -2.0f));
    projection = glm::perspective(45.0f, width / (float)height, .1f, 100.0f);
    glUniformMatrix4fv(
        glGetUniformLocation(shader.program, "model"),
        1, GL_FALSE, glm::value_ptr(model)
        );
    glUniformMatrix4fv(
        glGetUniformLocation(shader.program, "view"),
        1, GL_FALSE, glm::value_ptr(view)
        );
    glUniformMatrix4fv(
        glGetUniformLocation(shader.program, "projection"),
        1, GL_FALSE, glm::value_ptr(projection)
        );

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(glGetUniformLocation(shader.program, "theTexture1"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniform1i(glGetUniformLocation(shader.program, "theTexture2"), 1);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
