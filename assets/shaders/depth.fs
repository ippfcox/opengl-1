#version 460 core

out vec4 FragColor;

uniform float unif_near;
uniform float unif_far;

void main()
{
    float z_ndc = gl_FragCoord.z * 2.0 - 1.0;
    float linear_depth = 2.0 * unif_near / (unif_far + unif_near - z_ndc * (unif_far - unif_near));
    FragColor = vec4(linear_depth, linear_depth, linear_depth, 1.0);
    // FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}