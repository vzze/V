#pragma once

#include <glad/glad.h>

#include <glm.hpp>

#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>
#include <chrono>
#include <iomanip>

namespace v {
    namespace util {
        void log(const char * output);
        
        extern const glm::vec3 Xaxis;
        extern const glm::vec3 Yaxis;
        extern const glm::vec3 Zaxis;

        extern const std::string default_vertex_shader_path;
        extern const std::string default_fragment_shader_path;
        extern const std::string default_geom_shader_path;

        extern const std::string default_stencil_vertex_path;
        extern const std::string default_stencil_fragment_path;

        extern const std::string default_framebuffer_vertex_path;
        extern const std::string default_framebuffer_fragment_path;

        extern const std::string default_skybox_vertex_path;
        extern const std::string default_skybox_fragment_path;

        extern const std::string default_normal_geom_path;
        extern const std::string default_normal_fragment_path;

        std::tuple<short int, short int, short int> hex_to_rgb(long long hex);

        std::tuple<float, float, float> normalized_rgb(std::tuple<short int, short int, short int> rgb);
        // color - 0 - 255
        float normalized_color(short int color);

        std::string get_file(const char * file);
        // Format: \\Path\\to\\file
        std::string normalized_path(const char * path);

    }
}
