#pragma once

#include <json.h>

using json = nlohmann::json;

#include <vector>
#include <string>

#include "../../Util/Util.h"

namespace v {
namespace engine {
            // Order of cubemaps should be:
            // right left top bottom front back
            struct Skybox_container {
                std::string arr[6];
            };
            struct Shader_container {
                std::string arr[3];
            };
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

                std::string vertexShaderPath = v::util::default_vertex_shader_path;
                std::string fragmentShaderPath = v::util::default_fragment_shader_path; 
                std::string geometryShaderPath = v::util::default_geom_shader_path;
        
                std::vector<std::string> model_paths;
       
                std::vector<Skybox_container> skybox_paths;

                std::vector<Shader_container> shader_paths;

                std::string appName = "V";

                double tickrate = 1.0 / 64.0;
            
                bool VSYNC = true;
                bool fullscreen = false;
            };
        
            void read_engineSettings(EngineSettings & setts, std::string path);

            EngineSettings read_engineSettings(std::string path);
    }
}
