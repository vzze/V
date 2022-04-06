#version 330 core

out vec4 FragColor;

uniform vec3 Color = vec3(1.0F, 1.0F, 1.0F);

void main() {
    FragColor = vec4(Color, 1.0F);
}
