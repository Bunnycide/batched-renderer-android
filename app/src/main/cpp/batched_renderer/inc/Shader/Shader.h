#ifndef SHADER_
#define SHADER_

#define GLM_ENABLE_EXPERIMENTAL

#include <GLES3/gl3.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"

class Shader {
private:
    GLuint shaderProgram;
    std::vector<unsigned int> shaderIds;
    std::unordered_map<std::string, GLenum> uniformTypes;
    void queryActiveUniforms();

public:
    explicit Shader() = default;
    void addShader(const std::string& shaderSource, GLenum shaderType);
    void buildProgram();
    void useProgram() const;

    GLint getUniformLocation(const std::string &name) const;

    void set(const std::string &name, float value) const;
    void set(const std::string &name, const glm::vec2 &value) const;
    void set(const std::string &name, const glm::vec3 &value) const;
    void set(const std::string &name, const glm::vec4 &value) const;
    void set(const std::string &name, const glm::mat2 &mat) const;
    void set(const std::string &name, const glm::mat3 &mat) const;
    void set(const std::string &name, const glm::mat4 &mat) const;
    void set(const std::string &name, const glm::mat2x3 &mat) const;
    void set(const std::string &name, const glm::mat3x2 &mat) const;
    void set(const std::string &name, const glm::mat2x4 &mat) const;
    void set(const std::string &name, const glm::mat4x2 &mat) const;
    void set(const std::string &name, const glm::mat3x4 &mat) const;
    void set(const std::string &name, const glm::mat4x3 &mat) const;

    void set(const std::string &name, int value) const;
    void set(const std::string &name, const glm::ivec2 &value) const;
    void set(const std::string &name, const glm::ivec3 &value) const;
    void set(const std::string &name, const glm::ivec4 &value) const;

    void set(const std::string &name, unsigned int value) const;
    void set(const std::string &name, const glm::uvec2 &value) const;
    void set(const std::string &name, const glm::uvec3 &value) const;
    void set(const std::string &name, const glm::uvec4 &value) const;
};

#endif // SHADER_