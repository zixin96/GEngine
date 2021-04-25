#pragma once
#include "GLCore/Renderer/Shader.h"

typedef unsigned int GLenum;

namespace GLCore
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filePath);
        OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
        ~OpenGLShader();
        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual const std::string& GetName() const override { return m_Name; }

        virtual void SetMat4(const std::string& name, const glm::mat4& value) override;
        virtual void SetVec4(const std::string& name, const glm::vec4& value) override;
        virtual void SetVec3(const std::string& name, const glm::vec3& value) override;
        virtual void SetInt(const std::string& name, int value) override;
        virtual void SetIntArray(const std::string& name, int* value, uint32_t count) override;
        virtual void SetFloatArray(const std::string& name, float* value, uint32_t count) override;
        virtual void SetFloat(const std::string& name, float value) override;

        void UploadMat4(const std::string& name, const glm::mat4& value) const;
        void UploadVec4(const std::string& name, const glm::vec4& value) const;
        void UploadVec3(const std::string& name, const glm::vec3& value) const;
        void UploadInt(const std::string& name, int value) const;
        void UploadIntArray(const std::string& name, int* value, uint32_t count) const;
        void UploadFloatArray(const std::string& name, float* value, uint32_t count) const;
        void UploadFloat(const std::string& name, float value) const;
    private:
        std::string ReadFile(const std::string& filePath);
        std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSrcMaps);
        std::string ExtractNameFromFilePath(const std::string& filePath);
    private:
        uint32_t m_RendererID;
        std::string m_Name;
    };
}
