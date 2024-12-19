#version 460

in vec2 frag_uv;

uniform sampler2D unif_screen_sampler;
uniform int unif_screen_width;
uniform int unif_screen_height;

out vec4 FragColor;

vec3 blur()
{
    float du = 1.0 / float(unif_screen_width);
    float dv = 1.0 / float(unif_screen_width);

    vec2 uvs[9] = vec2[](
        vec2(-du, +dv), vec2(0.0, +dv), vec2(+du, +dv),
        vec2(-du, 0.0), vec2(0.0, 0.0), vec2(+du, 0.0),
        vec2(-du, -dv), vec2(0.0, -dv), vec2(+du, -dv)
    );
    
    float kernal[9] = float[](
        1.0, 2.0, 1.0,
        2.0, 4.0, 2.0,
        1.0, 2.0, 1.0
    );

    vec3 final_color = vec3(0.0);
    for (int i = 0; i < 9; i++)
    {
        final_color += texture(unif_screen_sampler, frag_uv + uvs[i]).rgb * kernal[i];
    }
    
    return final_color / 16.0;
}

void main()
{
    vec4 origin_color = texture(unif_screen_sampler, frag_uv);
    
    // FragColor = vec4(blur(), origin_color.a);
    FragColor = origin_color;
}