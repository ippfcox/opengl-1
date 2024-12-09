#version 460 core

layout (location = 0) in vec3 attr_position;
layout (location = 1) in vec2 attr_uv;
layout (location = 2) in vec3 attr_normal;

uniform mat4 unif_model;
uniform mat4 unif_view;
uniform mat4 unif_projection;

out vec3 frag_world_position;
out vec4 frag_color;
out vec2 frag_uv;
out vec3 frag_normal;

void main()
{
    frag_world_position = (unif_model * vec4(attr_position, 1.0)).xyz;
    gl_Position = unif_projection * unif_view * unif_model * vec4(attr_position, 1.0);
    frag_uv = attr_uv;
    // note: inverse() cost too much gpu
    frag_normal = transpose(inverse(mat3(unif_model))) * attr_normal;
}