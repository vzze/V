#version 330 core

out vec4 FragColor;

uniform vec4 outline_color = vec4(0.0F, 0.0F, 0.0F, 1.0F);

void main() {
    FragColor = outline_color;
}
