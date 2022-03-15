#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../Util/Util.h"

namespace v {
    namespace renderer {

        class Shader {
            private:
            
                void compileErr(unsigned int shader, const char * type);

            public:

                GLuint ID;

                Shader(const char * vertexFile, const char * fragmentFile);

                void Activate();
                
                void Delete();
        };
    }
    
}