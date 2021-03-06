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

        class Core {
            public:
                v::renderer::Shader shaderProgram;

                v::renderer::Shader stencilProgram;

                v::renderer::Shader skyboxProgram;
                
                v::renderer::Shader normalsProgram;

                static v::renderer::Shader framebufferProgram;
 
                static v::renderer::Framebuffer * framebuffer;

                static v::renderer::Camera camera;

                static v::engine::Window * Window;

                Core(v::engine::EngineSettings & settings, Callback_Functions funcs);

                virtual ~Core(); 

                static unsigned int MSAAsamples;
                float gamma = 2.2F;

                void MSAA(MSAA msaa);

                void SetNormalLength(float length);

                void SetNormalColor(long long hex);
                void SetNormalcolor(float r, float g, float b);
                void SetNormalcolor(std::tuple<float, float, float> rgb);
                void SetNormalColor(short int r, short int g, short int b);
                void SetNormalColor(std::tuple<short int, short int, short int> rgb);
                
                // domain of 0.45F to 2.5F
                void SetGammaCorrection(float value = 2.2F);
        };
    }
}
