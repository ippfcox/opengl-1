#version 460 core

out vec4 FragColor;

uniform vec4 unif_color;
uniform float unif_opacity;

void main()
{
    FragColor = vec4(unif_color.rgb, unif_color.a * unif_opacity);
}