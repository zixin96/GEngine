#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

class BatchRenderingLayer : public GLCore::Layer
{
public:
	BatchRenderingLayer();
	virtual ~BatchRenderingLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	GLCore::Utils::Shader* m_Shader;
	GLCore::Utils::OrthographicCameraController m_CameraController;
	
	GLCore::Utils::Ref<GLCore::Utils::VertexArray> m_QuadVA;
	GLCore::Utils::Ref<GLCore::Utils::VertexBuffer> m_QuadVB;
	GLCore::Utils::Ref<GLCore::Utils::IndexBuffer> m_QuadIB;

	GLCore::Utils::Ref<GLCore::Utils::Texture2D> m_SnowTexture;
	GLCore::Utils::Ref<GLCore::Utils::Texture2D> m_BrickTexture;

	glm::vec4 m_SquareBaseColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec4 m_SquareAlternateColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_SquareColor = m_SquareBaseColor;
};