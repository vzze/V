#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../renderer/renderer.h"
#include "../Util/Util.h"

namespace v {
    // fragment and vertex shader paths should be normalized with v::util::normalized_path
    struct EngineSettings {
        unsigned int width = 800;
        unsigned int height = 800;
        
        glm::vec3 camera_position = glm::vec3(0.0F, 0.0F, 2.0F);

        float cameraFOVdegrees = 45.0F;
        float cameraNearPlane = 0.1F;
        float cameraFarPlane = 100.0F;

        GLFWmonitor * monitor = NULL;
        GLFWwindow * share = NULL;

        char * vertexShaderPath = const_cast<char*>(v::util::default_vertex_shader_path.c_str());
        char * fragmentShaderPath = const_cast<char*>(v::util::default_fragment_shader_path.c_str()); 
        
        std::vector<std::string> model_paths;
        
        char * appName = const_cast<char*>("V");

        double tickrate = 1.0 / 30.0;

        bool VSYNC = true;
    };

    class Engine {
        public:
            // changing settings while app is running might cause problems
            static EngineSettings settings;
            // Stores all loaded Models that were given in the settings
            std::vector<v::renderer::Model*> models;
            // extra shaders in case you want different Models rendered differently
            std::vector<v::renderer::Shader> * shaders;

            v::renderer::Shader * default_shaderProgram;
            static v::renderer::Camera * camera;

            GLFWwindow * window = NULL;

            Engine(EngineSettings & _settings);
            // default settings are applied
            // no models are loaded
            Engine();

            ~Engine();
            // Inner settings can still be changed in Init
            // It is recommended to create settings before class creation
            // and give as @param to v::Engine::Engine(v::EngineSettings)
            virtual bool Init() = 0;
            // settings default is 1.0 / 3.0
            virtual bool Tickrate(double diffTime) = 0;

            virtual bool Draw() = 0;

            void Run();
            // path is normalized inside function
            // \\Path\\to\\Model
            void loadModels(std::string & path);
            // path is normalized inside function
            // \\Path\\to\\Model
            void loadModels(std::vector<std::string> & paths);
        private:
            static void engine_callback(GLFWwindow * window, int width, int heigh);
    };
}
