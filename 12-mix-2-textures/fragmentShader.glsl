#version 330 core

in vec3 inColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D theTexture;

void main()
{
  //color = vec4(1.0f, 0.2f, 0.5f, 1.0f);
  color = texture(theTexture, TexCoord) * vec4(inColor, 1.0f);
}

// vi: syntax=c
