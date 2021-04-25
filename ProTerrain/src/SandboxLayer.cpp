#include "SandboxLayer.h"
#include "GLCore/Renderer/MeshRenderer.h"

using namespace GLCore;

SandboxLayer::SandboxLayer()
    : m_PerspectiveCamera(30.0f, 1.778f, 0.1f, 1000.f)
{}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    MeshRenderer::Init();
    

	// Init here
	glClearColor(0.8f, 0.2f, 0.3f, 1.0f);
}

void SandboxLayer::OnDetach()
{
	// Shutdown here
}

void SandboxLayer::OnEvent(Event& event)
{
	// Events here
	m_PerspectiveCamera.OnEvent(event);
}

void SandboxLayer::OnUpdate(Timestep ts)
{
    m_PerspectiveCamera.OnUpdate(ts);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    MeshRenderer::BeginScene(m_PerspectiveCamera);
    
    MeshRenderer::Draw();
}

void SandboxLayer::OnImGuiRender()
{
     ImGui::Begin("Controls");
//     ImGui::Text("Quads: %d", Renderer::GetStats().QuadCount);
//     ImGui::Text("Draws: %d", Renderer::GetStats().DrawCount);
     ImGui::End();
}
