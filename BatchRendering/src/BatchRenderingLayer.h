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
		
	GLCore::Utils::PerspectiveCamera m_PerspectiveCamera;

	GLCore::Utils::Ref<GLCore::Utils::Texture2D> m_SnowTexture;
	GLCore::Utils::Ref<GLCore::Utils::Texture2D> m_BrickTexture;

	//glm::vec2 m_QuadPositon = { -1.5f, -0.5f };
};