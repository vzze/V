#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../Util/Util.h"
#include "../EngineUtil/engineUtil.h"

namespace v {
    namespace engine {
        class Window {
            public:
                GLFWwindow * window = nullptr;
                GLFWwindow * share = nullptr;
                GLFWmonitor * monitor = nullptr;

                bool failed = false;

                Window(v::engine::EngineSettings & settings);
                Window();

                bool isFullscreen();
                void SetFullscreen(bool fullscreen);

                void SetFramebufferSizeCallback(GLFWframebuffersizefun fn);
                void SetKeyCallback(GLFWkeyfun fn);

                bool ShouldClose();

                void SetTitle(const char * title);

                void SwapBuffers();

                virtual ~Window();
        };
    }
}
