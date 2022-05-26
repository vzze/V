#include "Object.h"

v::engine::Object::~Object() {
    v::util::log("Called Engine Object Destructor");
}

void v::engine::Object::Draw(v::renderer::Shader &shader, v::renderer::Camera &cam) {
    draw(shader, cam, translation, rotation, scale);    
}

void v::engine::Object::DrawWithOutline(
    v::renderer::Shader & shader, 
    v::renderer::Shader & stencil, 
    v::renderer::Camera & cam,
    std::tuple<float, float, float> rgb,
    float alpha,
    float thickness
) {
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    
    draw(shader, cam, translation, rotation, scale);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    
    stencil.Activate();

    stencil.Uniform1f("outlining", thickness);
    stencil.Uniform4f("outline_color", std::get<0>(rgb), std::get<1>(rgb), std::get<2>(rgb), alpha);

    draw(stencil, cam, translation, rotation, scale);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
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
