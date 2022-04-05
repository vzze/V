#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../renderer/renderer.h"
#include "../../Util/Util.h"
#include "../Window/Window.h"
#include "../EngineSettings/engineSettings.h"
#include "../Object/Object.h"

namespace v {
    namespace engine {
        class Core {
            public:
                // changing settings while app is running might cause problems
                static v::engine::EngineSettings settings;
                // Stores all loaded Models that were given in the settings
                std::vector<v::engine::Object*> objects;
                // stores all loaded skyboxes that were given in the settings;
                std::vector<v::renderer::Skybox*> skyboxes;
                // extra shaders in case you want different Models rendered differently
                std::vector<v::renderer::Shader*> shaders;

                static v::renderer::Core * renderer; 

                Core(v::engine::EngineSettings & _settings);
                // default settings are applied
                // no models are loaded
                Core();

                virtual ~Core();
                // Inner settings can still be changed in Init
                // It is recommended to create settings before class creation
                // and give as @param to v::Engine::Engine(v::EngineSettings)
                // function is executed right before the main thread runs
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
                // path is normalized inside function
                // \\Path\\to\\Model
                void loadSkyboxes(Skybox_container & cont);
                // path is normalized inside function
                // \\Path\\to\\Model
                void loadSkyboxes(std::vector<Skybox_container> & conts);
                
                // sets the skybox from the skyboxes vector
                void setCurrentSkybox(unsigned int index);

            private:
                v::renderer::Skybox * current_skybox = nullptr;

                void main_thread();

                static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
                static void window_callback(GLFWwindow * window, int width, int heigh);
                static void focus_callback(GLFWwindow * window, int focused);
        };
    }
}
