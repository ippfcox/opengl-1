#version 460 core

in vec2 frag_uv;
in vec3 frag_normal;

out vec4 FragColor;

uniform sampler2D unif_texture_earth;
uniform sampler2D unif_texture_cloud;
uniform sampler2D unif_texture_sky;
uniform sampler2D unif_texture_noise;

void main()
{
    // vec4 cloud_color = texture(unif_texture_cloud, frag_uv);
    // vec4 sky_color = texture(unif_texture_sky, frag_uv);
    // vec4 noise_color = texture(unif_texture_noise, frag_uv);

    // vec4 final_color = mix(cloud_color, sky_color, noise_color.r);

    // FragColor = texture(unif_texture_earth, frag_uv);
    
    vec3 normal = clamp(normalize(frag_normal), 0.0, 1.0);
    
    FragColor = vec4(normal, 1.0);
}