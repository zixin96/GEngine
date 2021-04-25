#include "glpch.h"
#include "OpenGLShader.h"
#include <fstream>
#include <glad/glad.h>

namespace GLCore
{
    static GLenum ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex")
        {
            return GL_VERTEX_SHADER;
        }
        if (type == "fragment" || type == "pixel")
        {
            return GL_FRAGMENT_SHADER;
        }

        GLCORE_ASSERT(false, "Unknown shader type!");
        return 0;
    }

    OpenGLShader::OpenGLShader(const std::string& filePath)
        : m_RendererID(0), m_Name(ExtractNameFromFilePath(filePath))
    {
        std::string source = ReadFile(filePath);
        auto shaderSources = Preprocess(source);
        Compile(shaderSources);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
        : m_RendererID(0), m_Name(name)
    {
        std::unordered_map<GLenum, std::string> shaderSrcMaps;
        shaderSrcMaps[GL_VERTEX_SHADER] = vertexSource;
        shaderSrcMaps[GL_FRAGMENT_SHADER] = fragmentSource;
        Compile(shaderSrcMaps);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    std::string OpenGLShader::ExtractNameFromFilePath(const std::string& filePath)
    {
        auto lastSlash = filePath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filePath.rfind('.');
        auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
        return filePath.substr(lastSlash, count);
    }

    std::string OpenGLShader::ReadFile(const std::string& filePath)
    {
        std::string result;
        std::ifstream in(filePath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else {
            LOG_ERROR("Could not open file '{0}'", filePath);
        }
        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::Preprocess(const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shaderSrcMaps;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            GLCORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            GLCORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

            size_t nextLinePos = source.find_first_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            shaderSrcMaps[ShaderTypeFromString(type)] = source.substr(
                nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos)
            );
        }
        return shaderSrcMaps;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSrcMaps)
    {
        GLuint program = glCreateProgram();
        GLCORE_ASSERT(shaderSrcMaps.size() <= 2, "We only support 2 shaders for now");
        std::array<GLenum, 2> glShaderIDs;
        int glShaderIDIndex = 0;
        for (auto& kv : shaderSrcMaps)
        {
            GLenum shaderType = kv.first;
            const std::string& shaderSource = kv.second;

            GLuint shader = glCreateShader(shaderType);
            const GLchar* sourceCStr = (const GLchar*)shaderSource.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);
            glCompileShader(shader);
            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
                glDeleteShader(shader);
                LOG_ERROR("{0}", infoLog.data());
                GLCORE_ASSERT(false, "Shader Compilation Failture!");
                break;
            }
            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }
        glLinkProgram(program);
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
            glDeleteProgram(program);
            for (auto id : glShaderIDs)
            {
                glDeleteShader(id);
            }
            LOG_ERROR("{0}", infoLog.data());
            GLCORE_ASSERT(false, "Shader Link Failture!");
            return;
        }

        for (auto id : glShaderIDs)
        {
            glDetachShader(program, id);
        }
        m_RendererID = program;
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        UploadMat4(name, value);
    }

    void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& value)
    {
        UploadVec4(name, value);
    }

    void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& value)
    {
        UploadVec3(name, value);
    }

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
        UploadInt(name, value);
    }

    void OpenGLShader::SetIntArray(const std::string& name, int* value, uint32_t count)
    {
        UploadIntArray(name, value, count);
    }

    void OpenGLShader::SetFloatArray(const std::string& name, float* value, uint32_t count)
    {
        UploadFloatArray(name, value, count);
    }

    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
        UploadFloat(name, value);
    }

    void OpenGLShader::UploadMat4(const std::string& name, const glm::mat4& value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    void OpenGLShader::UploadVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
    }

    void OpenGLShader::UploadVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, &value[0]);
    }

    void OpenGLShader::UploadInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
    }

    void OpenGLShader::UploadIntArray(const std::string& name, int* value, uint32_t count) const
    {
        glUniform1iv(glGetUniformLocation(m_RendererID, name.c_str()), count, value);
    }

    void OpenGLShader::UploadFloatArray(const std::string& name, float* value, uint32_t count) const
    {
        glUniform1fv(glGetUniformLocation(m_RendererID, name.c_str()), count, value);
    }

    void OpenGLShader::UploadFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
    }
}
