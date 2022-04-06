#pragma once

#include "../../engine/EngineSettings/engineSettings.h"
#include "../Framebuffer/Framebuffer.h"
#include "../shaders/Shader.h"
#include "../../engine/Window/Window.h"
#include "GLFW/glfw3.h"
#include "../Camera/Camera.h"

namespace v {
    namespace renderer {

        struct Callback_Functions {
            GLFWframebuffersizefun size_callback; 
            GLFWkeyfun key_callback;
            GLFWwindowfocusfun focus_callback;  
        };

        class Core final {
            public:
                v::renderer::Shader * shaderProgram;

                v::renderer::Shader * stencilProgram;

                v::renderer::Shader * skyboxProgram;
                
                v::renderer::Shader * normalsProgram;

                v::renderer::Shader * framebufferProgram;
 
                v::renderer::Framebuffer * framebuffer;

                v::renderer::Camera * camera;

                v::engine::Window * Window;

                Core(v::engine::EngineSettings & settings, Callback_Functions funcs);

                ~Core(); 

                void SetNormalLength(float length);

                void SetNormalcolor(float r, float g, float b);
                void SetNormalcolor(std::tuple<float, float, float> rgb);
                void SetNormalColor(short int r, short int g, short int b);
                void SetNormalColor(std::tuple<short int, short int, short int> rgb);
        };
    }
}
