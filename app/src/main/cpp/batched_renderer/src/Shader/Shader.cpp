#include "Shader/Shader.h"
#include "../../AndroidOut.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

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

    queryActiveUniforms();
}

void Shader::queryActiveUniforms() {
    GLint i;
    GLint count;

    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc.)

    const GLsizei bufSize = 256; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length

    glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &count);

    aout << "Number of active uniforms is " << count << std::endl;

    for(i =0; i < count; i++){
        glGetActiveUniform(shaderProgram, i, sizeof(name), &length, &size, &type, name);
        uniformTypes[reinterpret_cast<GLchar*>(name)] = (GLenum)type;
    }
}

void Shader::useProgram() const {
    glUseProgram(shaderProgram);
}

GLint Shader::getUniformLocation(const std::string &name) const {
    auto it = uniformTypes.find(name);
    if (it != uniformTypes.end()) {
        return glGetUniformLocation(shaderProgram, name.c_str());
    } else {
        aout << "Uniform " << name << " not found!" << std::endl;
        return -1;
    }
}

// Set methods for float types
void Shader::set(const std::string &name, float value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void Shader::set(const std::string &name, const glm::vec2 &value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform2f(location, value.x, value.y);
    }
}

void Shader::set(const std::string &name, const glm::vec3 &value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform3f(location, value.x, value.y, value.z);
    }
}

void Shader::set(const std::string &name, const glm::vec4 &value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }
}

// Set methods for int types
void Shader::set(const std::string &name, int value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

void Shader::set(const std::string &name, const glm::ivec2 &value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform2i(location, value.x, value.y);
    }
}

void Shader::set(const std::string &name, const glm::ivec3 &value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform3i(location, value.x, value.y, value.z);
    }
}

void Shader::set(const std::string &name, const glm::ivec4 &value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform4i(location, value.x, value.y, value.z, value.w);
    }
}

// Set methods for unsigned int types
void Shader::set(const std::string &name, unsigned int value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform1ui(location, value);
    }
}

void Shader::set(const std::string &name, const glm::uvec2 &value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform2ui(location, value.x, value.y);
    }
}

void Shader::set(const std::string &name, const glm::uvec3 &value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform3ui(location, value.x, value.y, value.z);
    }
}

void Shader::set(const std::string &name, const glm::uvec4 &value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform4ui(location, value.x, value.y, value.z, value.w);
    }
}

// Set methods for matrix types
void Shader::set(const std::string &name, const glm::mat2 &mat) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void Shader::set(const std::string &name, const glm::mat3 &mat) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void Shader::set(const std::string &name, const glm::mat4 &mat) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void Shader::set(const std::string &name, const glm::mat2x3 &mat) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniformMatrix2x3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void Shader::set(const std::string &name, const glm::mat3x2 &mat) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniformMatrix3x2fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void Shader::set(const std::string &name, const glm::mat2x4 &mat) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniformMatrix2x4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void Shader::set(const std::string &name, const glm::mat4x2 &mat) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniformMatrix4x2fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void Shader::set(const std::string &name, const glm::mat3x4 &mat) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniformMatrix3x4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void Shader::set(const std::string &name, const glm::mat4x3 &mat) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniformMatrix4x3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
}
