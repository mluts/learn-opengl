#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <string>
#include <iostream>

static const char *vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec4 position;\n"
"void main()\n"
"{ gl_Position = vec4(position.x, position.y, position.z, 1.0); }\n"
;

static const char *fragmentShaderSource =
"#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{ color = vec4(1.0f, 0.5f, 0.2f, 1.0f); }\n"
;

GLfloat vertices[] = {
   0.5f,  0.5f, 0.0f, // Top right
   0.5f, -0.5f, 0.0f, // Bottom right
  -0.5f, -0.5f, 0.0f, // Bottom left
  -0.5f,  0.5f, 0.0f  // Top left
};

GLuint indices[] = {
  0, 1, 2,
  3, 0, 2
};

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

bool createShaderProgram(GLuint *shaderProgram) {
GLuint vertexShader, fragmentShader;
GLint success;
GLchar infoLog[512];

  /*
   * Initializing vertex shader
   */
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog
              << std::endl;
    return false;
  }

  /*
   * Initializing fragment shader
   */
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog
              << std::endl;
    return false;
  }

  /*
   * Initializing shader program
   */
  *shaderProgram = glCreateProgram();

  glAttachShader(*shaderProgram, vertexShader);
  glAttachShader(*shaderProgram, fragmentShader);
  glLinkProgram(*shaderProgram);

  glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n"
               << infoLog
               << std::endl;
    return false;
  }

  /* Cleanup */
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return true;
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

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

  bool success;
  GLuint VAO, shaderProgram;

  success = createShaderProgram(&shaderProgram);
  if(!success) {
    std::cout << "Failed to create shader program\n" << std::endl;
    return 1;
  }

  createVertexArray(&VAO);

  /*
   * Main loop
   */
  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Draw triangles as line
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
