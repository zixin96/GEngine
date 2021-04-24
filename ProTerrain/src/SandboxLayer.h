#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include "PerlinNoise.h"

class SandboxLayer : public GLCore::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	GLCore::Utils::PerspectiveCamera m_PerspectiveCamera;
	PerlinNoise m_PerlinNoise;
	float* m_NoiseMap;
    uint32_t m_NoiseMapWidth = 512;
    uint32_t m_NoiseMapHeight = 512;
};