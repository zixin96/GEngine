#include "BatchRenderingLayer.h"
#include "GLCore/Renderer/Renderer.h"

using namespace GLCore;

BatchRenderingLayer::BatchRenderingLayer()
{

}

BatchRenderingLayer::~BatchRenderingLayer()
{

}

void BatchRenderingLayer::OnAttach()
{
	EnableGLDebugging();

    m_PerspectiveCamera = PerspectiveCamera(30.0f, 1.778f, 0.1f, 1000.f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Renderer::Init();

     m_SnowTexture = Texture2D::Create("assets/textures/snow_field_aerial_col_1k.png");
     m_BrickTexture = Texture2D::Create("assets/textures/brick_floor_003_diffuse_1k.png");
}

void BatchRenderingLayer::OnDetach()
{
    Renderer::Shutdown();
}

void BatchRenderingLayer::OnEvent(Event& event)
{
    m_PerspectiveCamera.OnEvent(event);
}


void BatchRenderingLayer::OnUpdate(Timestep ts)
{
    m_PerspectiveCamera.OnUpdate(ts);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Renderer::BeginScene(m_PerspectiveCamera);
    Renderer::ResetStats();
    Renderer::BeginBatch();

    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 5; x++)
        {
            Renderer::DrawQuad({ x, y, 1.0f }, { 1.0f, 1.0f }, (x + y) % 2 == 0 ? m_SnowTexture : m_BrickTexture);
        }
    }

    for (float y = -10.0f; y < 10.0f; y += 0.25f)
    {
        for (float x = -10.0f; x < 10.0f; x += 0.25f)
        {
            glm::vec4 color = { (x + 10) / 20.0f, 0.2f, (y + 10) / 20.0f, 1.0f };
            Renderer::DrawQuad({ x, y, 0.0f }, { 0.2f, 0.2f }, color);
        }
    }

    Renderer::EndBatch();
    Renderer::Flush();
}

void BatchRenderingLayer::OnImGuiRender()
{
	ImGui::Begin("Controls");
    ImGui::Text("Quads: %d", Renderer::GetStats().QuadCount);
    ImGui::Text("Draws: %d", Renderer::GetStats().DrawCount);
	ImGui::End();
}
