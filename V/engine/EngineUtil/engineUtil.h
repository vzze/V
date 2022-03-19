#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include <vector>
#include <string>
#include <future>

#include "../../Util/Util.h"

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

            GLFWmonitor * monitor = NULL;
            GLFWwindow * share = NULL;

            char * vertexShaderPath = const_cast<char*>(v::util::default_vertex_shader_path.c_str());
            char * fragmentShaderPath = const_cast<char*>(v::util::default_fragment_shader_path.c_str()); 
        
            std::vector<std::string> model_paths;
        
            char * appName = const_cast<char*>("V");

            double tickrate = 1.0 / 30.0;

            bool VSYNC = true;
        };
        // any thread created should not edit opengl variables
        // or call opengl functions
        // by default opengl is linked to the main thread
        template<typename return_value, typename function, typename... Args>
        class Thread {
            private:
                std::future<return_value> f;
                bool promise_resolved = false;
            public:
                Thread(function ff, Args... args) { f = std::async(ff, args...); }

                inline return_value get() { promise_resolved = true; return f.get(); }

                ~Thread() { if(!promise_resolved) f.get(); }
        };
    }
}