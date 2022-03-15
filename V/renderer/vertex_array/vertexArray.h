#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../vertex_buffer/vertexBuffer.h"

namespace v {
    namespace renderer {
        class VAO {
            public:

                GLuint ID;

                VAO();

                void LinkAttrib(VBO & vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void * offset);
                void Bind();
                void Unbind();
                void Delete();
        };
    }
}