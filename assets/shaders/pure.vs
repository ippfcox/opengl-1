#version 460 core

layout (location = 0) in vec3 attr_position;

uniform mat4 unif_model;
uniform mat4 unif_view;
uniform mat4 unif_projection;

void main()
{
    gl_Position = unif_projection * unif_view * unif_model * vec4(attr_position, 1.0);
}