#include "Object.h"

v::engine::Object::~Object() {}

void v::engine::Object::Draw(v::renderer::Shader &shader, v::renderer::Camera &cam) {
    draw(shader, cam, translation, rotation, scale);    
}

void v::engine::Object::Rotate(float degrees, const glm::vec3 & axis) { 
    rotation = glm::normalize(rotation * glm::angleAxis(glm::radians(degrees), axis));
}

void v::engine::Object::SetRotation(const glm::quat & quat) {
    rotation = quat; 
}

void v::engine::Object::SetRotation(float degrees, const glm::vec3 & axis) {
    rotation = glm::angleAxis(glm::radians(degrees), axis); 
}

void v::engine::Object::SetRotation(const glm::vec3 & eulerAngles) {
    rotation = glm::quat(eulerAngles);
}

void v::engine::Object::SetRotation(const glm::vec3 & v1, const glm::vec3 & v2) {
    rotation = glm::quat(v1, v2);
}

void v::engine::Object::SetRotation(const glm::mat3 & matrix) {
    rotation = glm::quat(matrix);
}

void v::engine::Object::SetRotation(const glm::mat4 & matrix) {
    rotation = glm::quat(matrix);
}

void v::engine::Object::SetRotation(float w, float x, float y, float z) {
    rotation = glm::quat(w, x, y, z);
}

void v::engine::Object::MoveTo(float x, float y, float z) {
    translation = glm::vec3(x, y, z);
}

void v::engine::Object::Move(float x, float y, float z) {
    translation.x += x;
    translation.y += y;
    translation.z += z;
}

void v::engine::Object::Scale(float x, float y, float z) {
    scale = glm::vec3(x, y, z);
}
