#version 330 core

in vec3 inColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D theTexture1;
uniform sampler2D theTexture2;

void main()
{
  color = mix(
    texture(theTexture1, TexCoord),
    texture(theTexture2, vec2(TexCoord.x, (1 - TexCoord.y))),
    0.5
  );
}

// vi: syntax=c
