#include "Shader.h"

v::renderer::Shader::Shader(const char * vertexFile, const char * fragmentFile, const char * geometryFile) {
    GLuint vertexShader = compileShader(vertexFile, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentFile, GL_FRAGMENT_SHADER);
    GLuint geometryShader; if(geometryFile) geometryShader = compileShader(geometryFile, GL_GEOMETRY_SHADER);

    ID = glCreateProgram();

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    if(geometryFile) glAttachShader(ID, geometryShader);

    glLinkProgram(ID);
    
    compileErr(ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if(geometryFile) glDeleteShader(geometryShader);
}

GLuint v::renderer::Shader::compileShader(const char * file, GLenum type) {
    std::string source = v::util::get_file(file);

    const char * c_source = source.c_str();

    GLuint Shader = glCreateShader(type);
    glShaderSource(Shader, 1, &c_source, NULL);
    glCompileShader(Shader);

    switch(type) {
        case GL_VERTEX_SHADER:
            v::util::log((std::string("Shader: Loading Vertex Shader: ") + file).c_str());
            compileErr(Shader, "VERTEX");
        break;

        case GL_FRAGMENT_SHADER:
            v::util::log((std::string("Shader: Loading Fragment Shader: ") + file).c_str());
            compileErr(Shader, "FRAGMENT");
        break;

        case GL_GEOMETRY_SHADER:
            v::util::log((std::string("Shader: Loading Geometry Shader: ") + file).c_str());
            compileErr(Shader, "GEOMETRY");
        break;
    }

    return Shader;
}

void v::renderer::Shader::Activate() {
    glUseProgram(ID);
}

void v::renderer::Shader::Delete() {
    glDeleteProgram(ID);
}

void v::renderer::Shader::compileErr(unsigned int shader, const char * type) {
    
    GLint hasCompiled;
    char info[1024];
    if(strcmp(type, "PROGRAM")) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if(hasCompiled == GL_FALSE) {
            glGetShaderInfoLog(shader, 1024, NULL, info);

            std::string log = "Shader Compile Error: ";

            log += type; log += '\n'; log += info;

            v::util::log(log.c_str());
        }
    } else {
        glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if(hasCompiled == GL_FALSE) {
            glGetProgramInfoLog(shader, 1024, NULL, info);

            std::string log = "Shader Compile Error: ";

            log += type; log += '\n'; log += info; 

            v::util::log(log.c_str());
        }
    } 
}

GLint v::renderer::Shader::loc(const char * value) {
    return glGetUniformLocation(ID, value);
}

void v::renderer::Shader::Uniform1f(const char * v, GLfloat v0) {
    glUniform1f(loc(v), v0);
}
 
void v::renderer::Shader::Uniform2f(const char * v, GLfloat v0, GLfloat v1) {
    glUniform2f(loc(v), v0, v1);
}
 
void v::renderer::Shader::Uniform3f(const char * v, GLfloat v0, GLfloat v1, GLfloat v2) {
    glUniform3f(loc(v), v0, v1, v2);
}
 
void v::renderer::Shader::Uniform4f(const char * v, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    glUniform4f(loc(v), v0, v1, v2, v3);
}
 
void v::renderer::Shader::Uniform1i(const char * v, GLint v0) {
    glUniform1i(loc(v), v0);
}
 
void v::renderer::Shader::Uniform2i(const char * v, GLint v0, GLint v1) {
    glUniform2i(loc(v), v0, v1);
}
 
void v::renderer::Shader::Uniform3i(const char * v, GLint v0, GLint v1, GLint v2) {
    glUniform3i(loc(v), v0, v1, v2);
}
 
void v::renderer::Shader::Uniform4i(const char * v, GLint v0, GLint v1, GLint v2, GLint v3) {
    glUniform4i(loc(v), v0, v1, v2, v3);
}
 
void v::renderer::Shader::Uniform1ui(const char * v, GLuint v0) {
    glUniform1ui(loc(v), v0);
}
 
void v::renderer::Shader::Uniform2ui(const char * v, GLuint v0, GLuint v1) {
    glUniform2ui(loc(v), v0, v1);
}
 
void v::renderer::Shader::Uniform3ui(const char * v, GLuint v0, GLuint v1, GLuint v2) {
    glUniform3ui(loc(v), v0, v1, v2);
}
 
void v::renderer::Shader::Uniform4ui(const char * v, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
    glUniform4ui(loc(v), v0, v1, v2, v3);
}
 
void v::renderer::Shader::Uniform1fv(const char * v, GLsizei count, const GLfloat *value) {
    glUniform1fv(loc(v), count, value);
}
 
void v::renderer::Shader::Uniform2fv(const char * v, GLsizei count, const GLfloat *value) {
    glUniform2fv(loc(v), count, value);
}
 
void v::renderer::Shader::Uniform3fv(const char * v, GLsizei count, const GLfloat *value) {
    glUniform3fv(loc(v), count, value);
}
 
void v::renderer::Shader::Uniform4fv(const char * v, GLsizei count, const GLfloat *value) {
    glUniform4fv(loc(v), count, value);
}
 
void v::renderer::Shader::Uniform1iv(const char * v, GLsizei count, const GLint *value) {
    glUniform1iv(loc(v), count, value);
}
 
void v::renderer::Shader::Uniform2iv(const char * v, GLsizei count, const GLint *value) {
    glUniform2iv(loc(v), count, value);
}
 
void v::renderer::Shader::Uniform3iv(const char * v, GLsizei count, const GLint *value) {
    glUniform3iv(loc(v), count, value);
}
 
void v::renderer::Shader::Uniform4iv(const char * v, GLsizei count, const GLint *value) {
    glUniform4iv(loc(v), count, value);
}
 
void v::renderer::Shader::Uniform1uiv(const char * v, GLsizei count, const GLuint *value) {
    glUniform1uiv(loc(v), count, value);
}
 
void v::renderer::Shader::Uniform2uiv(const char * v, GLsizei count, const GLuint *value) {
    glUniform2uiv(loc(v), count, value);
}

void v::renderer::Shader::Uniform3uiv(const char * v, GLsizei count, const GLuint *value) {
    glUniform3uiv(loc(v), count, value);
}

void v::renderer::Shader::Uniform4uiv(const char * v, GLsizei count, const GLuint *value) {
    glUniform4uiv(loc(v), count, value);
}

void v::renderer::Shader::UniformMatrix2fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value) {
    glUniformMatrix2fv(loc(v), count, transpose, value);
}

void v::renderer::Shader::UniformMatrix3fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value) {
    glUniformMatrix3fv(loc(v), count, transpose, value);
}

void v::renderer::Shader::UniformMatrix4fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value) {
    glUniformMatrix4fv(loc(v), count, transpose, value);
}
 
void v::renderer::Shader::UniformMatrix2x3fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value) {
    glUniformMatrix2x3fv(loc(v), count, transpose, value);
}
 
void v::renderer::Shader::UniformMatrix3x2fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value) {
    glUniformMatrix3x2fv(loc(v), count, transpose, value);
}
 
void v::renderer::Shader::UniformMatrix2x4fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value) {
    glUniformMatrix2x4fv(loc(v), count, transpose, value);
}
 
void v::renderer::Shader::UniformMatrix4x2fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value) {
    glUniformMatrix4x2fv(loc(v), count, transpose, value);
}
 
void v::renderer::Shader::UniformMatrix3x4fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value) {
    glUniformMatrix3x4fv(loc(v), count, transpose, value);
}
 
void v::renderer::Shader::UniformMatrix4x3fv(const char * v, GLsizei count, GLboolean transpose, const GLfloat *value) {
    glUniformMatrix4x3fv(loc(v), count, transpose, value);
}
