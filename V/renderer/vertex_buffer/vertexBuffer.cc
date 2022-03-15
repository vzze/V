#include "vertexBuffer.h"

v::renderer::VBO::VBO(std::vector<Vertex> & vertices) {
    glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
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