#include "Shader.h"

#include <iostream>

v::renderer::Shader::Shader(const char * vertexFile, const char * fragmentFile) {
    std::string vertF = v::util::get_file(vertexFile);
    std::string fragF = v::util::get_file(fragmentFile);

    const char * vf = vertF.c_str();
    const char * ff = fragF.c_str();
    try {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vf, NULL);
    glCompileShader(vertexShader);

    compileErr(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &ff, NULL);
    glCompileShader(fragmentShader);

    compileErr(fragmentShader, "FRAGMENT");

    ID = glCreateProgram();

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    glLinkProgram(ID);
    
    compileErr(ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 
    }catch(const std::exception& e)
    {
        std::cout << "Error:\n";
        std::cout << e.what() << '\n';
    }
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

            log += type; log += '\n'; log += info; log += '\n';

            v::util::log(log.c_str());
        }
    } else {
        glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if(hasCompiled == GL_FALSE) {
            glGetProgramInfoLog(shader, 1024, NULL, info);

            std::string log = "Shader Compile Error: ";

            log += type; log += '\n'; log += info; log += '\n';

            v::util::log(log.c_str());
        }
    } 
}
