#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace GLCore::Utils {

	class Shader
	{
	public:
		~Shader();

		GLuint GetRendererID() { return m_RendererID; }

        virtual void SetMat4(const std::string& name, const glm::mat4& value) ;
        virtual void SetVec4(const std::string& name, const glm::vec4& value) ;
        virtual void SetVec3(const std::string& name, const glm::vec3& value) ;
        virtual void SetInt(const std::string& name, int value) ;
        virtual void SetIntArray(const std::string& name, int* value, uint32_t count) ;
        virtual void SetFloat(const std::string& name, float value) ;

        void UploadMat4(const std::string& name, const glm::mat4& value) const;
        void UploadVec4(const std::string& name, const glm::vec4& value) const;
        void UploadVec3(const std::string& name, const glm::vec3& value) const;
        void UploadInt(const std::string& name, int value) const;
        void UploadIntArray(const std::string& name, int* value, uint32_t count) const;
        void UploadFloat(const std::string& name, float value) const;

		static Shader* FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	private:
		Shader() = default;

		void LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		GLuint CompileShader(GLenum type, const std::string& source);
	private:
		GLuint m_RendererID;
	};

}