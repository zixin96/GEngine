#include "glpch.h"
#include "Shader.h"
#include "OpenGLShader.h"
#include <fstream>

namespace GLCore::Utils 
{
    Ref<Shader> Shader::Create(const std::string& filePath)
    {
        return CreateRef<OpenGLShader>(filePath);
    }


    Ref<Shader> Shader::Create(const std::string& name, std::string& vertexSource, const std::string& fragmentSource)
    {
        return CreateRef<OpenGLShader>(name, vertexSource, fragmentSource);
    }

    std::string Shader::ReadFileAsString(const std::string& filepath)
    {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize((size_t)in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        }
        else
        {
            LOG_ERROR("Could not open file '{0}'", filepath);
        }

        return result;
    }

    void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
    {
        GLCORE_ASSERT(!Exists(name), "Shader already exists!");
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        auto& name = shader->GetName();
        Add(name, shader);
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filePath)
    {
        auto shader = Shader::Create(filePath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
    {
        auto shader = Shader::Create(filePath);
        Add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        GLCORE_ASSERT(Exists(name), "Shader Not Found!");
        return m_Shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string& name) const
    {
        return m_Shaders.find(name) != m_Shaders.end();
    }
}