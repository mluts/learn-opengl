#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 color;
out vec3 inColor;

void main()
{
  gl_Position = vec4(position.x, position.y, position.z, 1.0);
  inColor = color;
}

// vi: syntax=c