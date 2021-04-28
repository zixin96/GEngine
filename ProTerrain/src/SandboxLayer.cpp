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
}

void SandboxLayer::OnDetach()
{
	// Shutdown here
    MeshRenderer::Shutdown();
}

void SandboxLayer::OnEvent(Event& event)
{
	// Events here
	m_PerspectiveCamera.OnEvent(event);
}

void SandboxLayer::OnUpdate(Timestep ts)
{
    m_PerspectiveCamera.OnUpdate(ts);

    glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    MeshRenderer::BeginScene(m_PerspectiveCamera);
    
    MeshRenderer::Draw();
}

void SandboxLayer::OnImGuiRender()
{
     ImGui::Begin("Controls");
     if (ImGui::InputFloat("Lacunarity", &MeshRenderer::GetTerrainStats().Lacunarity))
     {
         MeshRenderer::RecomputeTerrainData();
     }
     if(ImGui::InputFloat("Persistance", &MeshRenderer::GetTerrainStats().Persistance))
     {
         MeshRenderer::RecomputeTerrainData();
     }
     ImGui::Text("Rotate: Alt + Left Mouse Button");
     ImGui::Text("Pan: Alt + Middle Mouse Button");
     ImGui::Text("Zoom: Mouse Scroll or Alt + Right Mouse Button");
     ImGui::End();
}
