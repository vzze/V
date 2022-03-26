#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <mutex>
#include <future>

#include "../../renderer/renderer.h"
#include "../../Util/Util.h"
#include "../EngineUtil/engineUtil.h"

namespace v {
    namespace engine {
        class Core {
            public:
                // changing settings while app is running might cause problems
                static v::engine::EngineSettings settings;
                // Stores all loaded Models that were given in the settings
                std::vector<v::engine::Object*> objects;
                // extra shaders in case you want different Models rendered differently
                std::vector<v::renderer::Shader*> shaders;

                v::renderer::Shader * default_shaderProgram;

                static v::renderer::Camera * camera;

                GLFWwindow * window = NULL;

                Core(v::engine::EngineSettings & _settings);
                // default settings are applied
                // no models are loaded
                Core();

                virtual ~Core();
                // Inner settings can still be changed in Init
                // It is recommended to create settings before class creation
                // and give as @param to v::Engine::Engine(v::EngineSettings)
                virtual bool Init() = 0;
                // settings default is 1.0 / 30.0
                // any opengl function should be executed on the main thread
                virtual bool Tickrate(double diffTime) = 0;
                // any opengl function should be executed on the main thread
                virtual bool Draw() = 0;

                void Run();
                // path is normalized inside function
                // \\Path\\to\\Model
                void loadModels(std::string & path);
                // path is normalized inside function
                // \\Path\\to\\Model
                void loadModels(std::vector<std::string> & paths);
            private:
                void main_thread();
                static void engine_callback(GLFWwindow * window, int width, int heigh);
        };
    }
}
