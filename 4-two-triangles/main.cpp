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

static const GLfloat triangle1[] = {
  -0.25f, 0.25f,  0.0f,
   0.0f,  0.0f,   0.0f,
  -0.5f,  0.0f,   0.0f,
};

static const GLfloat triangle2[] = {
  0.25f,  0.25f,  0.0f,
  0.5f,   0.0f,   0.0f,
  0.0f,   0.0f,   0.0f
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
void createVertexArray(const GLfloat *vertices, GLsizeiptr size, GLuint *VAO)
{
GLuint VBO;

  glGenVertexArrays(1, VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
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
  GLuint VAOtriangle1, VAOtriangle2, shaderProgram;

  success = createShaderProgram(&shaderProgram);
  if(!success) {
    std::cout << "Failed to create shader program\n" << std::endl;
    return 1;
  }

  createVertexArray(triangle1, 9 * sizeof(GLfloat), &VAOtriangle1);
  createVertexArray(triangle2, 9 * sizeof(GLfloat), &VAOtriangle2);

  /*
   * Main loop
   */
  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Draw triangles as line

    glBindVertexArray(VAOtriangle1);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(VAOtriangle2);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}
