#include "SandboxLayer.h"
#include "GLCORE/Util/Renderer.h"

using namespace GLCore;
using namespace GLCore::Utils;

SandboxLayer::SandboxLayer()
{}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();


    m_PerspectiveCamera = PerspectiveCamera(30.0f, 1.778f, 0.1f, 1000.f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Renderer::Init();

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

    Renderer::BeginScene(m_PerspectiveCamera);
    Renderer::ResetStats();
    Renderer::BeginBatch();

    for (float y = -10.0f; y < 10.0f; y += 0.25f)
    {
        for (float x = -10.0f; x < 10.0f; x += 0.25f)
        {
            glm::vec4 color = { (x + 10) / 20.0f, 0.2f, (y + 10) / 20.0f, 1.0f };
            Renderer::DrawQuad({ x, 0.0f, y }, { 0.25f, 0.25f }, color);
        }
    }

    Renderer::EndBatch();
    Renderer::Flush();
}

void SandboxLayer::OnImGuiRender()
{
    ImGui::Begin("Controls");
    ImGui::Text("Quads: %d", Renderer::GetStats().QuadCount);
    ImGui::Text("Draws: %d", Renderer::GetStats().DrawCount);
    ImGui::End();
}
