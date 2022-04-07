#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;

uniform float offset_x;
uniform float offset_y;

uniform float gamma = 2.2F;

vec2 offsets[9] = vec2[]
(
    vec2(-offset_x,  offset_y), vec2(0.0f,    offset_y), vec2(offset_x,  offset_y),
    vec2(-offset_x,  0.0f),     vec2(0.0f,    0.0f),     vec2(offset_x,  0.0f),
    vec2(-offset_x, -offset_y), vec2(0.0f,   -offset_y), vec2(offset_x, -offset_y) 
);

float kernel[9] = float[]
(
    1,  1, 1,
    1, -8, 1,
    1,  1, 1
);

void main() {
    //vec3 color = vec3(0.0f);
    
    //for(int i = 0; i < 9; i++)
    //    color += vec3(texture(screenTexture, texCoords.st + offsets[i])) * kernel[i];
    
    //FragColor = vec4(color, 1.0F);

    vec4 fragment = texture(screenTexture, texCoords);
    FragColor.rgb = pow(fragment.rgb, vec3(1.0f / gamma));
}
