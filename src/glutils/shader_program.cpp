#include "shader_program.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

std::string ShaderProgram::readFile(const char* filepath)
{
    std::ifstream file{ filepath };
    if (!file)
    {
        std::cerr << "Failed to open file " << filepath << '\n';
        return std::string{};
    }
    std::stringstream content{};
    content << file.rdbuf();
    return content.str();
}

GLuint ShaderProgram::compileShader(GLenum shaderType, const char* shaderPath)
{
    std::cout << "Compiling shader " << shaderPath << "...\n";
    GLuint shader{ glCreateShader(shaderType) };
    std::string shaderString{ readFile(shaderPath) };
    const char* shaderSource{ shaderString.c_str() };
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);
    if (!compileSuccess(shader, shaderPath))
    {
        glDeleteShader(shader);
        shader = 0;
    }
    else
    {
        std::cout << shaderPath << " compiled successfully\n";
    }
    return shader;
}

GLuint ShaderProgram::linkProgram(const char* computePath)
{
    std::cout << '\n';
    std::cout << "Linking " << computePath << "...\n";
    GLuint program{ glCreateProgram() };
    GLuint computeShader{ compileShader(GL_COMPUTE_SHADER, computePath) };

    glAttachShader(program, computeShader);
    glLinkProgram(program);
    if (!linkSuccess(program))
    {
        glDeleteProgram(program);
        program = 0;
    }
    else
    {
        std::cout << "Program linked successfully\n";
    }

    std::cout << '\n';
    glDeleteShader(computeShader);
    return program;
}

GLuint ShaderProgram::linkProgram(const char* vertexPath, const char* fragmentPath)
{
    std::cout << '\n';
    std::cout << "Linking " << vertexPath << " and " << fragmentPath << "...\n";
    GLuint program{ glCreateProgram() };
    GLuint vertexShader{ compileShader(GL_VERTEX_SHADER, vertexPath) };
    GLuint fragmentShader{ compileShader(GL_FRAGMENT_SHADER, fragmentPath) };

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    if (!linkSuccess(program))
    {
        glDeleteProgram(program);
        program = 0;
    }
    else
    {
        std::cout << "Program linked successfully\n";
    }

    std::cout << '\n';
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

GLint ShaderProgram::compileSuccess(GLuint shader, const char* shaderPath)
{
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << "Failed to compile " << shaderPath << '\n' << infoLog << '\n';
    }
    return success;
}

GLint ShaderProgram::linkSuccess(GLuint program)
{
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        std::cerr << "Failed to link program\n" << infoLog << '\n';
    }
    return success;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteShader(m_id);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : m_id{ other.m_id }
{
    other.m_id = 0;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    std::swap(m_id, other.m_id);
    return *this;
}

ShaderProgram::ShaderProgram(const char* computePath)
    : m_id{ linkProgram(computePath) }
{
}

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath)
    : m_id{ linkProgram(vertexPath, fragmentPath) }
{
}

void ShaderProgram::setUniform(const char *name, int value)
{
    glUseProgram(m_id);
    glUniform1i(
        glGetUniformLocation(m_id, name),
        value
    );
}

void ShaderProgram::setUniform(const char* name, unsigned int value)
{
    glUseProgram(m_id);
    glUniform1ui(
        glGetUniformLocation(m_id, name),
        value
    );
}

void ShaderProgram::setUniform(const char *name, float value)
{
    glUseProgram(m_id);
    glUniform1f(
        glGetUniformLocation(m_id, name),
        value
    );
}

void ShaderProgram::setUniform(const char* name, const glm::vec2& value)
{
    glUseProgram(m_id);
    glUniform2fv(
        glGetUniformLocation(m_id, name),
        1,
        glm::value_ptr(value)
    );
}

void ShaderProgram::setUniform(const char *name, const glm::vec3 &value)
{
    glUseProgram(m_id);
    glUniform3fv(
        glGetUniformLocation(m_id, name),
        1,
        glm::value_ptr(value)
    );
}

void ShaderProgram::setUniform(const char* name, const glm::uvec3& value)
{
    glUseProgram(m_id);
    glUniform3uiv(
        glGetUniformLocation(m_id, name),
        1,
        glm::value_ptr(value)
    );
}

void ShaderProgram::setUniform(const char* name, const glm::vec4& value)
{
    glUseProgram(m_id);
    glUniform4fv(
        glGetUniformLocation(m_id, name),
        1,
        glm::value_ptr(value)
    );
}

void ShaderProgram::setUniform(const char *name, const glm::mat3 &value)
{
    glUseProgram(m_id);
    glUniformMatrix3fv(
        glGetUniformLocation(m_id, name),
        1,
        GL_FALSE,
        glm::value_ptr(value)
    );
}

void ShaderProgram::setUniform(const char *name, const glm::mat4 &value)
{
    glUseProgram(m_id);
    glUniformMatrix4fv(
        glGetUniformLocation(m_id, name),
        1,
        GL_FALSE,
        glm::value_ptr(value)
    );
}
