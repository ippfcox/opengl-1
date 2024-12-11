#version 460 core

in vec3 frag_world_position;
in vec2 frag_uv;
in vec3 frag_normal;

out vec4 FragColor;

uniform sampler2D unif_sampler;
uniform vec3 unif_light_direction;
uniform vec3 unif_light_color;
uniform float unif_specular_intensity;
uniform float unif_specular_shiness;
uniform vec3 unif_ambient_color;
uniform vec3 unif_camera_position;

void main()
{
    // light common
    vec3 object_color = texture(unif_sampler, frag_uv).rgb;
    vec3 frag_normal_n = normalize(frag_normal);
    vec3 view_direction_n = normalize(frag_world_position - unif_camera_position);

    // diffuse
    vec3 light_direction_n = normalize(unif_light_direction);
    float diffuse = clamp(dot(-light_direction_n, frag_normal_n), 0.0, 1.0);
    vec3 diffuse_color = diffuse * unif_light_color * object_color;
    
    // specular
    vec3 light_reflect_direction_n = normalize(reflect(light_direction_n, frag_normal_n));
    float specular = pow(clamp(dot(-light_reflect_direction_n, view_direction_n), 0.0, 1.0), unif_specular_shiness);
    float disable_back_light = step(0.0, dot(-light_direction_n, frag_normal_n));
    vec3 specular_color = disable_back_light * unif_specular_intensity * specular * unif_light_color;

    // ambient
    vec3 ambient_color = unif_ambient_color * object_color;

    // final
    vec3 final_color = diffuse_color + specular_color + ambient_color;

    FragColor = vec4(final_color, 1.0);
}