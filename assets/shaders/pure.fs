#version 460 core

out vec4 FragColor;

uniform vec3 unif_point_light_color;

void main()
{
    FragColor = vec4(unif_point_light_color, 1.0);
}