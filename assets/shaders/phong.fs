#version 460 core

struct DirectionalLight
{
    vec3 color;
    float specular_intensity;
    vec3 direction;
};

struct PointLight
{
    vec3 color;
    float specular_intensity;
    vec3 position;
    float k2;
    float k1;
    float kc;
};

struct SpotLight
{
    vec3 color;
    float specular_intensity;
    vec3 position;
    vec3 direction;
    float inner_cone;
    float outer_cone;
};

in vec3 frag_world_position;
in vec2 frag_uv;
in vec3 frag_normal;

out vec4 FragColor;

uniform sampler2D unif_diffuse_sampler;
uniform sampler2D unif_specular_mask_sampler;

uniform DirectionalLight unif_directional_light;
uniform SpotLight unif_spot_light;

uniform float unif_specular_shiness;
uniform vec3 unif_ambient_color;
uniform vec3 unif_camera_position;

vec3 calculate_diffuse_color(vec3 light_color, vec3 object_color, vec3 light_direction_n, vec3 normal_n)
{
    float diffuse = clamp(dot(-light_direction_n, normal_n), 0.0, 1.0);
    return diffuse * light_color * object_color;
}

// [fixme] may have problem
vec3 calculate_specular_color(vec3 light_color, vec3 light_direction_n, vec3 normal_n, vec3 view_direction_n, float intensity)
{
    vec3 light_reflect_direction_n = normalize(reflect(light_direction_n, normal_n));
    float specular = pow(clamp(dot(-light_reflect_direction_n, view_direction_n), 0.0, 1.0), unif_specular_shiness);
    float disable_back_light = step(0.0, dot(-light_direction_n, normal_n));
    // float specular_mask
    return /* specular_mask * */ disable_back_light * intensity * specular * light_color;
}

vec3 calculate_spot_light(SpotLight light, vec3 normal_n, vec3 view_direction_n)
{
    vec3 object_color = texture(unif_diffuse_sampler, frag_uv).rgb; // dup?
    vec3 light_direction_n = normalize(frag_world_position - light.position);
    vec3 target_direction_n = normalize(light.direction);

    float cos_gamma = dot(light_direction_n, target_direction_n);
    float intensity = clamp((cos_gamma - light.outer_cone) / (light.inner_cone - light.outer_cone), 0.0, 1.0);

    vec3 diffuse_color = calculate_diffuse_color(light.color, object_color, light_direction_n, normal_n);
    vec3 specular_color = calculate_specular_color(light.color, light_direction_n, normal_n, view_direction_n, light.specular_intensity);

    return (diffuse_color + specular_color) * intensity;
}

vec3 calcualte_directeional_light(DirectionalLight light, vec3 normal_n, vec3 view_direction_n)
{
    vec3 object_color = texture(unif_diffuse_sampler, frag_uv).rgb; // dup?
    vec3 light_direction_n = normalize(light.direction);
    
    vec3 diffuse_color = calculate_diffuse_color(light.color, object_color, light_direction_n, normal_n);
    vec3 specular_color = calculate_specular_color(light.color, light_direction_n, normal_n, view_direction_n, light.specular_intensity);

    return diffuse_color + specular_color;
}

void main()
{
    // light common
    vec3 object_color = texture(unif_diffuse_sampler, frag_uv).rgb;
    vec3 frag_normal_n = normalize(frag_normal);
    vec3 view_direction_n = normalize(frag_world_position - unif_camera_position);

    vec3 final_color = vec3(0.0);
    final_color += calculate_spot_light(unif_spot_light, frag_normal_n, view_direction_n);
    final_color += calcualte_directeional_light(unif_directional_light, frag_normal_n, view_direction_n);
    
    vec3 ambient_color = unif_ambient_color * object_color;
    
    final_color += ambient_color;

    // debug

    FragColor = vec4(final_color, 1.0);
}