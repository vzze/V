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
        // fragment and vertex shader paths should be normalized with
        // v::util::normalized_path
        // @width - window width
        // @height - window height
        // @camera_position - camera starting position
        // cameraFOVdegrees - field of view of the camera
        // cameraNearPlane - limit to how close an object can be to be rendered
        // cameraFarPlane - limit to how far an object can be to be rendered
        struct EngineSettings {
            unsigned int width = 800;
            unsigned int height = 800;
        
            glm::vec3 camera_position = glm::vec3(0.0F, 0.0F, 2.0F);

            float cameraFOVdegrees = 45.0F;
            float cameraNearPlane = 0.1F;
            float cameraFarPlane = 100.0F;

            std::string vertexShaderPath = v::util::default_vertex_shader_path;
            std::string fragmentShaderPath = v::util::default_fragment_shader_path.c_str(); 
        
            std::vector<std::string> model_paths;
        
            std::string appName = "V";

            double tickrate = 1.0 / 64.0;
            
            bool VSYNC = true;
            bool fullscreen = false;
        };

        void read_engineSettings(EngineSettings & setts, std::string path);

        EngineSettings read_engineSettings(std::string path);

        class Object : public v::renderer::Model {
            public:
                glm::vec3 translation = glm::vec3(0.0F, 0.0F, 0.0F);
                glm::quat rotation = glm::quat(1.0F, 0.0F, 0.0F, 0.0F);
                glm::vec3 scale = glm::vec3(1.0F, 1.0F, 1.0F);
                
                Object(const char * model_path) : v::renderer::Model(model_path) {}
                
                virtual ~Object();

                void Draw(v::renderer::Shader & shader, v::renderer::Camera & cam);
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
