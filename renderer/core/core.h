#pragma once

#include "../../engine/EngineUtil/engineUtil.h"
#include "../Framebuffer/Framebuffer.h"
#include "../shaders/Shader.h"
#include "../../engine/Window/Window.h"
#include "GLFW/glfw3.h"

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

                v::renderer::Shader * skyboxProgram;
                
                v::renderer::Shader * normalsProgram;

                v::renderer::Shader * framebufferProgram;
 
                v::renderer::Framebuffer * framebuffer;

                v::renderer::Camera * camera;

                v::engine::Window * Window;

                Core(v::engine::EngineSettings & settings, Callback_Functions funcs);

                ~Core(); 
        };
    }
}
