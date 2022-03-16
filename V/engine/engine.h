#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../renderer/renderer.h"

namespace v {
    struct EngineSettings {
        unsigned int width = 800;
        unsigned int height = 800;
        
        glm::vec3 camera_position = glm::vec3(0.0F, 0.0F, 2.0F);

        float cameraFOVdegrees = 45.0F;
        float cameraNearPlane = 0.1F;
        float cameraFarPlane = 100.0F;

        GLFWmonitor * monitor = NULL;
        GLFWwindow * share = NULL;

        char * vertexShaderPath = const_cast<char*>("\\V\\renderer\\shaders\\default.vert");
        char * fragmentShaderPath = const_cast<char*>("\\V\\renderer\\shaders\\default.frag");
        
        std::vector<std::string> model_paths;
        
        char * appName = const_cast<char*>("V");

        double tickrate = 1.0 / 30.0;

        bool VSYNC = true;
    };

    class Engine {
        public:

            static EngineSettings settings;

            std::vector<v::renderer::Model*> models;

            v::renderer::Shader * shaderProgram;
            v::renderer::Camera * camera;

            GLFWwindow * window = NULL;

            Engine(EngineSettings & _settings);

            ~Engine();

            void Run();

            void loadModels(std::string & path);
            void loadModels(std::vector<std::string> & paths);

            static void engine_callback(GLFWwindow * window, int width, int heigh);
    };

    
}