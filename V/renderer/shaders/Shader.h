#pragma once

#include <glad/glad.h>
#include <cstring>

#include "../../Util/Util.h"

namespace v {
    namespace renderer {

        class Shader {
            private:
            
                void compileErr(unsigned int shader, const char * type);

                GLint loc(const char * value);

            public:

                GLuint ID;

                Shader(const char * vertexFile, const char * fragmentFile);
                
                void Activate();
                
                void Delete();

                void Uniform1f(const char * v, GLfloat v0);
 
                void Uniform2f(const char * v, GLfloat v0, GLfloat v1);
 
                void Uniform3f(const char * v, GLfloat v0, GLfloat v1, GLfloat v2);
 
                void Uniform4f(const char * v, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
 
                void Uniform1i(const char * v, GLint v0);
 
                void Uniform2i(const char * v, GLint v0, GLint v1);
 
                void Uniform3i(const char * v, GLint v0, GLint v1, GLint v2);
 
                void Uniform4i(const char * v, GLint v0, GLint v1, GLint v2, GLint v3);
 
                void Uniform1ui(const char * v, GLuint v0);
 
                void Uniform2ui(const char * v, GLuint v0, GLuint v1);
 
                void Uniform3ui(const char * v, GLuint v0, GLuint v1, GLuint v2);
 
                void Uniform4ui(const char * v, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
 
                void Uniform1fv(const char * v, GLsizei count, const GLfloat *value);
 
                void Uniform2fv(const char * v, GLsizei count, const GLfloat *value);
 
                void Uniform3fv(const char * v, GLsizei count, const GLfloat *value);
 
                void Uniform4fv(const char * v, GLsizei count, const GLfloat *value);
 
                void Uniform1iv(const char * v, GLsizei count, const GLint *value);
 
                void Uniform2iv(const char * v, GLsizei count, const GLint *value);
 
                void Uniform3iv(const char * v, GLsizei count, const GLint *value);
 
                void Uniform4iv(const char * v, GLsizei count, const GLint *value);
 
                void Uniform1uiv(const char * v, GLsizei count, const GLuint *value);
 
                void Uniform2uiv(const char * v, GLsizei count, const GLuint *value);

                void Uniform3uiv(const char * v, GLsizei count, const GLuint *value);

                void Uniform4uiv(const char * v, GLsizei count, const GLuint *value);

                void UniformMatrix2fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value);

                void UniformMatrix3fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value);

                void UniformMatrix4fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value);
 
                void UniformMatrix2x3fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value);
 
                void UniformMatrix3x2fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value);
 
                void UniformMatrix2x4fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value);
 
                void UniformMatrix4x2fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value);
 
                void UniformMatrix3x4fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value);
 
                void UniformMatrix4x3fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value);
        };
    }
    
}
