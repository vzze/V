#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>

namespace v {
    namespace util {
        extern int width;
        extern int height;

        void log(const char * output);

        std::tuple<short int, short int, short int> hex_to_rgb(long long hex);

        std::tuple<float, float, float> normalized_rgb(std::tuple<short int, short int, short int> rgb);
        // color - 0 - 255
        float normalized_color(short int color);

        std::string get_file(const char * file);
        // Format: \\Path\\to\\file
        std::string normalized_path(const char * path);

        void framebuffer_size_callback(GLFWwindow * window, int width, int height);
    }
}