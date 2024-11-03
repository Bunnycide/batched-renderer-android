#ifndef SHADER_
#define SHADER_

#include <GLES3/gl3.h>
#include <string>
#include <vector>

class Shader {
private:
    GLuint shaderProgram;
    std::vector<unsigned int> shaderIds;
public:
    explicit Shader() = default;
    void addShader(const std::string& shaderSource, GLenum shaderType);
    void buildProgram();
    void useProgram();
};

#endif // SHADER_