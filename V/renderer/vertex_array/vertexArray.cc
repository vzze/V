#include "vertexArray.h"

v::renderer::VAO::VAO() {
    glGenVertexArrays(1, &ID);
}

void v::renderer::VAO::LinkAttrib(VBO & vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void * offset) {
    vbo.Bind();

    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);

    vbo.Unbind();
}

void v::renderer::VAO::Bind() {
    glBindVertexArray(ID);
}

void v::renderer::VAO::Unbind() {
    glBindVertexArray(0);
}

void v::renderer::VAO::Delete() {
    glDeleteVertexArrays(1, &ID);
}
