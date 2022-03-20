#include "engineUtil.h"

void v::engine::Object::Draw(v::renderer::Shader &shader, v::renderer::Camera &cam) {
    draw(shader, cam, translation, rotation, scale);    
}

void v::engine::Object::Rotate(float degrees, glm::vec3 axis) { 
    v::engine::Object::rotation = glm::angleAxis(glm::radians(degrees), axis);
}

void v::engine::Object::MoveTo(float x, float y, float z) {
    translation = glm::vec3(x, y, z);
}

void v::engine::Object::Scale(float x, float y, float z) {
    scale = glm::vec3(x, y, z);
}
