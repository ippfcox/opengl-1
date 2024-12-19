#version 460 core

const float PI = 3.14159265358979323846;

in vec3 frag_uvw;

out vec4 FragColor;

// uniform samplerCube unif_sampler;
uniform sampler2D unif_sampler;

vec2 unw_to_uv(vec3 uvw_n)
{
    float phi = asin(uvw_n.y);
    float theta = atan(uvw_n.z, uvw_n.x);
    return vec2(theta / (2.0 * PI) + 0.5, phi / PI + 0.5);
}

void main()
{
    // FragColor = texture(unif_sampler, frag_uvw);
    FragColor = texture(unif_sampler, unw_to_uv(normalize(frag_uvw)));
}