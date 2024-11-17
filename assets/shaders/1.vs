#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 uv;

uniform float time;

out vec4 frag_color;
out vec2 frag_uv;

void main()
{
    gl_Position = vec4(position, 1.0);
    frag_color = vec4(color, 1.0) * (sin(time) + 1) / 2;
    frag_uv = uv;
}