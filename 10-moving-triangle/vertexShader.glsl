#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 color;
out vec3 inColor;

uniform float offset;

void main()
{
  gl_Position = vec4((offset + position.x), -position.y, position.z, 1.0);
  inColor = color;
}

// vi: syntax=c
