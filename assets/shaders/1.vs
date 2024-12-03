#version 460 core

layout (location = 0) in vec3 attr_position;
layout (location = 1) in vec3 attr_color;
layout (location = 2) in vec2 attr_uv;

uniform float time;
uniform mat4 transform;

out vec4 frag_color;
out vec2 frag_uv;

void main()
{
    gl_Position = transform * vec4(attr_position, 1.0);
    frag_color = vec4(attr_color, 1.0) * (sin(time) + 1) / 2;
    frag_uv = attr_uv;
}