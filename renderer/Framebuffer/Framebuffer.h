#pragma once

#include <glad/glad.h>

#include "../../Util/Util.h"
#include "../shaders/Shader.h"

namespace v {
    namespace renderer {
        class Framebuffer {
            private:

                float rectangleVertices[24] = {
                   1.0F,  -1.0F,    1.0F, 0.0F,
                   -1.0F, -1.0F,    0.0F, 0.0F,
                   -1.0F,  1.0F,    0.0F, 1.0F,

                   1.0F,   1.0F,    1.0F, 1.0F,
                   1.0F,  -1.0F,    1.0F, 0.0F,
                   -1.0F,  1.0F,    0.0F, 1.0F
                };

            public:

                GLuint FBO;
                GLuint RBO; 
                
                GLuint framebufferTexture;

                GLuint rectVAO;
                GLuint rectVBO;

                Framebuffer();

                void Bind(GLuint width, GLuint height);
                void Delete();
                void Draw(v::renderer::Shader & framebufferProgram);
        };
    }
}
