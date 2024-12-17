#version 460

in vec2 frag_uv;

uniform sampler2D unif_screen_sampler;

out vec4 FragColor;

void main()
{
    FragColor = texture(unif_screen_sampler, frag_uv);
}