#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <gtx/rotate_vector.hpp>
#include <gtx/vector_angle.hpp>

#include "../shaders/Shader.h"

namespace v {
    namespace renderer {
        class Camera {
            public:
                glm::vec3 Position;
                glm::vec3 Orientation = glm::vec3(0.0F, 0.0F, -1.0F);
                glm::vec3 Up = glm::vec3(0.0F, 1.0F, 0.0F);

                glm::mat4 cameraMatrix = glm::mat4(1.0F);

                int width, height;

                // Depending on how you want the camera to work you will have to edit void Inputs()
                bool firstClick = true;

                float speed = 0.1F;
                float sensitivity = 100.0F;

                Camera() = default;
                Camera(int width, int height, glm::vec3 position);
                
                virtual ~Camera();

                void updateMatrix(float FOVdeg, float nearPlane, float farPlane);

                void Matrix(Shader & shader, const char * uniform);

                void Inputs(GLFWwindow * window);
        };
    }
}
