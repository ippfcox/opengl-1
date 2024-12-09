#version 460 core

in vec2 frag_uv;
in vec3 frag_normal;

out vec4 FragColor;

uniform sampler2D unif_texture_earth;
uniform sampler2D unif_texture_cloud;
uniform sampler2D unif_texture_sky;
uniform sampler2D unif_texture_noise;
uniform vec3 unif_diffuse_light_direction;
uniform vec3 unif_diffuse_light_color;

void main()
{
    vec3 object_color = texture(unif_texture_earth, frag_uv).rgb;
    vec3 frag_normal_n = normalize(frag_normal);
    vec3 diffuse_light_direction_n = normalize(unif_diffuse_light_direction);

    float diffuse = clamp(dot(-diffuse_light_direction_n, frag_normal_n), 0.0, 1.0);

    vec3 final_color = object_color * diffuse * unif_diffuse_light_color;

    // vec4 cloud_color = texture(unif_texture_cloud, frag_uv);
    // vec4 sky_color = texture(unif_texture_sky, frag_uv);
    // vec4 noise_color = texture(unif_texture_noise, frag_uv);

    // vec4 final_color = mix(cloud_color, sky_color, noise_color.r);

    // FragColor = texture(unif_texture_earth, frag_uv);

    FragColor = vec4(final_color, 1.0);
}