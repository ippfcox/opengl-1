#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform float time;

out vec4 frag_color;

void main()
{
    gl_Position = vec4(position, 1.0);
    frag_color = vec4(color, 1.0) * (sin(time) + 1) / 2;
}