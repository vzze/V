#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
                void loadModels(std::vector<std::string> & paths);
                
                // path is normalized inside function
                // \\Path\\to\\Model
                void loadShaders(v::engine::Shader_container & cont);
                void loadShaders(std::vector<v::engine::Shader_container> & conts);

                // path is normalized inside function
                // \\Path\\to\\Model
                void loadSkyboxes(Skybox_container & cont);
                void loadSkyboxes(std::vector<Skybox_container> & conts);
                
                // sets the skybox from the skyboxes vector
                void setCurrentSkybox(unsigned int index);

                void SetBackgroundColor(long long hex);
                void SetBackgroundColor(float r, float g, float b);
                void SetBackgroundColor(std::tuple<float, float, float> rgb);
                void SetBackgroundColor(short int r, short int g, short int b);
                void SetBackgroundColor(std::tuple<short int, short int, short int> rgb);

                void SetMode(MODE mode);

                void DebugDraw();
                void DebugTickrate();

            private:
                v::renderer::Skybox * current_skybox = nullptr;

                std::tuple<float, float, float> backgroundColor = { 0.0F, 0.0F, 0.0F };

                MODE mode = v::MODE::VRELEASE;

                void release_thread();
                // ------------- debug mode 
                void debug_thread();

                ImGuiIO & Init(GLFWwindow * window, const char * version);

                void Prep();

                void MainWindow();
                void GammaWindow();

                bool skybox_draw = false;
                int skybox_slider;

                void SkyboxWindow();

                float color[3] = { 0.0F, 0.0F, 0.0F };

                void BackgroundColorWindow();

                struct ObjectProps {
                    bool draw = true;
                    bool DrawOutline = false;
                    bool DrawNormals = false;

                    float colors[3] = { 1.0F, 1.0F, 1.0F };
                    float thickness = 0.08;
                    float alpha = 1.0F;
                    
                    float X_rot = 0.0F;
                    float Y_rot = 0.0F;
                    float Z_rot = 0.0F;

                    float X_scale = 1.0F;
                    float Y_scale = 1.0F;
                    float Z_scale = 1.0F;

                    float X_translate = 0.0F;
                    float Y_translate = 0.0F;
                    float Z_translate = 0.0F;
                };

                ObjectProps * props;

                int object_slider = 1;

                void ObjectWindow();

                void Render();

                void Terminate();
                // ----------------
                static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
                static void window_callback(GLFWwindow * window, int width, int heigh);
                static void focus_callback(GLFWwindow * window, int focused);
        };
    }
}
