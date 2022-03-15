#pragma once

#include <string>
#include <vector>

#include "../vertex_array/vertexArray.h"
#include "../EBO/EBO.h"
#include "../Camera/Camera.h"
#include "../Texture/Texture.h"

namespace v {
    namespace renderer {
        class Mesh {
            public:
                std::vector<Vertex> vertices;
                std::vector<GLuint> indices;
                std::vector<Texture> textures;

                VAO vao;

                Mesh(std::vector<Vertex> & vertices, std::vector<GLuint> & indices, std::vector<Texture> & textures);

                void Draw(
                    Shader & shader, Camera & cam, 
                    glm::mat4 matrix = glm::mat4(1.0F), 
                    glm::vec3 translation = glm::vec3(0.0F, 0.0F, 0.0F), 
                    glm::quat rotation = glm::quat(1.0F, 0.0F, 0.0F, 0.0F),
                    glm::vec3 scale = glm::vec3(1.0F, 1.0F, 1.0F)
                );
        };
    }
}