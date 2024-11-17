#version 460 core

in vec4 frag_color;
in vec2 frag_uv;

out vec4 FragColor;

uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, frag_uv);
}