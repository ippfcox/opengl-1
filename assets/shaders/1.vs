#version 460 core

layout (location = 0) in vec3 attr_position;
layout (location = 1) in vec2 attr_uv;

uniform mat4 unif_model;
uniform mat4 unif_view;
uniform mat4 unif_projection;

out vec4 frag_color;
out vec2 frag_uv;

void main()
{
    gl_Position = unif_projection * unif_view * unif_model * vec4(attr_position, 1.0);
    frag_uv = attr_uv;
}