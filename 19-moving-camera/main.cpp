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
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f), 
  glm::vec3( 2.0f,  5.0f, -15.0f), 
  glm::vec3(-1.5f, -2.2f, -2.5f),  
  glm::vec3(-3.8f, -2.0f, -12.3f),  
  glm::vec3( 2.4f, -0.4f, -3.5f),  
  glm::vec3(-1.7f,  3.0f, -7.5f),  
  glm::vec3( 1.3f, -2.0f, -2.5f),  
  glm::vec3( 1.5f,  2.0f, -2.5f), 
  glm::vec3( 1.5f,  0.2f, -1.5f), 
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 cameraPos{.0f, .0f, 3.0f};
glm::vec3 cameraFront{.0f, .0f, -1.0f};
glm::vec3 cameraUp{.0f, 1.0f, .0f};
GLfloat cameraSpeed{.05f};

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  // if(action != GLFW_PRESS)
  //   return;

  switch(key) {
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, GL_TRUE);
      break;
    case GLFW_KEY_W:
      cameraPos += cameraSpeed * cameraFront;
      break;
    case GLFW_KEY_S:
      cameraPos -= cameraSpeed * cameraFront;
      break;
    case GLFW_KEY_A:
      cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
      break;
    case GLFW_KEY_D:
      cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
      break;
  }
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

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
  glEnable(GL_DEPTH_TEST);

  /*
   * Main loop
   */
  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(glGetUniformLocation(shader.program, "theTexture1"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniform1i(glGetUniformLocation(shader.program, "theTexture2"), 1);

    for(int i = 0; i < (sizeof(cubePositions)/sizeof(glm::vec3)); i++)
    {
      glm::mat4 model, view, projection;
      model = glm::translate(model, cubePositions[i]);
      // model = glm::rotate(model, glm::radians(20.0f*i + (GLfloat)glfwGetTime() * (i+1) * 30.0f), glm::vec3(.5f, 1.0f, .0f));
      model = glm::rotate(model, glm::radians(20.0f + i * 20.0f), glm::vec3(.5f, 1.0f, .0f));

      // GLfloat radius = 10.0f,
      //         camX = sin(glfwGetTime()) * radius,
      //         camZ = cos(glfwGetTime()) * radius;
      //
      // // view = glm::translate(view, glm::vec3(.0f, .0f, -3.0f));
      // view = glm::lookAt(
      //     glm::vec3(camX, .0f, camZ),
      //     glm::vec3(.0f, .0f, .0f),
      //     glm::vec3(.0f, 1.0f, .0f)
      //     );
      view = glm::lookAt(
          cameraPos,
          cameraPos + cameraFront,
          cameraUp
          );

      projection = glm::perspective(glm::radians(45.0f), width / (float)height, .1f, 100.0f);
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

      glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (sizeof(GLfloat)*5));
    }

    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
