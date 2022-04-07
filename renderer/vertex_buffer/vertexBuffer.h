#pragma once

#include <glad/glad.h>

#include <glm.hpp>

#include <vector>

namespace v {
    namespace renderer {

        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec3 color;
            glm::vec2 texUV;
        };

        class VBO {
            public:

                GLuint ID;

                VBO(std::vector<Vertex> & vertices);
                
                VBO(std::vector<glm::mat4> & mat4s);

                void Bind();
                void Unbind();
                void Delete();
        };
    }
}
