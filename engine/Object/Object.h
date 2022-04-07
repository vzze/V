#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include <vector>
#include <string>

#include "../../Util/Util.h"
#include "../../renderer/Model/Model.h"

namespace v {
    namespace engine {
        class Object : public v::renderer::Model {
            public:
                glm::vec3 translation = glm::vec3(0.0F, 0.0F, 0.0F);
                glm::quat rotation = glm::quat(1.0F, 0.0F, 0.0F, 0.0F);
                glm::vec3 scale = glm::vec3(1.0F, 1.0F, 1.0F);
                
                Object(const char * model_path, unsigned int instances = 1, std::vector<glm::mat4> instanceMatrix = {}) : v::renderer::Model(model_path, instances, instanceMatrix) {}
                
                virtual ~Object();

                void Draw(v::renderer::Shader & shader, v::renderer::Camera & cam);

                void DrawWithOutline(v::renderer::Shader & shader, v::renderer::Shader & stencil, v::renderer::Camera & cam, float thickness = 0.08F);
                // degrees are automatically converted into radians
                // X, Y and Z axis can be found in v::util
                void Rotate(float degrees, const glm::vec3 & axis);

                void SetRotation(const glm::quat & quat);

                void SetRotation(float degrees, const glm::vec3 & axis);

                void SetRotation(const glm::vec3 & eulerAngles);

                void SetRotation(const glm::vec3 & v1, const glm::vec3 & v2);

                void SetRotation(const glm::mat3 & matrix);

                void SetRotation(const glm::mat4 & matrix);

                void SetRotation(float w, float x, float y, float z); 
                // default coords is 0.0F, 0.0F, 0.0F
                void MoveTo(float x, float y, float z);

                void Move(float x, float y, float z);
                // default scale is 1.0F, 1.0F, 1.0F
                void Scale(float x, float y, float z);
        };
    }
}
