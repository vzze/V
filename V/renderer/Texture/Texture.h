#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../shaders/shaderClass.h"

namespace v {
    namespace renderer {
        class Texture {
            public:

                GLuint ID;
                const char * type;
                GLuint unit;

                Texture(const char * image, const char * texType, GLuint slot);

                void texUnit(Shader & shader, const char * uniform, GLuint unit);

                void Bind();

                void Unbind();

                void Delete();
        };
    }
}