#version 460 core

in vec4 frag_color;
in vec2 frag_uv;

out vec4 FragColor;

uniform sampler2D texture_cloud;
uniform sampler2D texture_sky;
uniform sampler2D texture_noise;

void main()
{
    vec4 cloud_color = texture(texture_cloud, frag_uv);
    vec4 sky_color = texture(texture_sky, frag_uv);
    vec4 noise_color = texture(texture_noise, frag_uv);

    vec4 final_color = mix(cloud_color, sky_color, noise_color.r);

    FragColor = final_color;
}