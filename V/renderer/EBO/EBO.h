#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace v {
    namespace renderer {
        class EBO {
            public:

                GLuint ID;

                EBO(std::vector<GLuint> & indices);

                void Bind();
                void Unbind();
                void Delete();
        };
    }
}