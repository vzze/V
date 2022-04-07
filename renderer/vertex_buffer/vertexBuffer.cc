#include "vertexBuffer.h"

v::renderer::VBO::VBO(std::vector<Vertex> & vertices) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

v::renderer::VBO::VBO(std::vector<glm::mat4> & mat4s) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, mat4s.size() * sizeof(glm::mat4), mat4s.data(), GL_STATIC_DRAW);
}

void v::renderer::VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void v::renderer::VBO::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void v::renderer::VBO::Delete() {
    glDeleteBuffers(1, &ID);
}
