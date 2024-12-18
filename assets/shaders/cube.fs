#version 460 core

in vec3 frag_uvw;

out vec4 FragColor;

uniform samplerCube unif_sampler;

void main()
{
    FragColor = texture(unif_sampler, frag_uvw);
}