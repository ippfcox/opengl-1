#version 460

layout (location = 0) in vec3 attr_position;
layout (location = 1) in vec2 attr_uv;

out vec2 frag_uv;

void main()
{
    gl_Position = vec4(attr_position.xy, 0.0, 1.0);
    frag_uv = attr_uv;
}