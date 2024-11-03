#include "Shader/Shader.h"
#include "../../AndroidOut.h"

void Shader::addShader(const std::string& shaderSource, GLenum shaderType) {
    auto shaderId = glCreateShader(shaderType);

    const char *c_str = shaderSource.c_str();

    glShaderSource(shaderId, 1, &c_str , nullptr);
    glCompileShader(shaderId);

    int  success;
    char infoLog[512];
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
        aout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    shaderIds.push_back(shaderId);
}

void Shader::buildProgram() {
    shaderProgram = glCreateProgram();

    for(auto& id : shaderIds){
        glAttachShader(shaderProgram, id);
    }

    glLinkProgram(shaderProgram);

    int  success;
    char infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(! success){
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        aout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }
}

void Shader::useProgram() {
    glUseProgram(shaderProgram);
}
