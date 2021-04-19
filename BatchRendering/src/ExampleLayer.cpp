#include "ExampleLayer.h"

using namespace GLCore;
using namespace GLCore::Utils;

ExampleLayer::ExampleLayer()
	: m_CameraController(16.0f / 9.0f)
{

}

ExampleLayer::~ExampleLayer()
{

}

void ExampleLayer::OnAttach()
{
	EnableGLDebugging();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_Shader = Shader::FromGLSLTextFiles(
		"assets/shaders/test.vert.glsl",
		"assets/shaders/test.frag.glsl"
	);

	m_QuadVA = VertexArray::Create();

	float vertices[] = {
		-1.5f, -0.5f, 0.0f, 0.18f,	0.6f, 0.96f, 1.0f,	0.0f, 0.0f,			0.0f,
		 -0.5f, -0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f,	1.0f, 0.0f,			0.0f,
		 -0.5f,  0.5f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f,	1.0f, 1.0f,			0.0f,
		-1.5f,  0.5f, 0.0f, 0.18f,	0.6f, 0.96f, 1.0f,	0.0f, 1.0f,			0.0f,

		0.5f, -0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f,	0.0f, 0.0f,			1.0f,
		 1.5f, -0.5f, 0.0f,1.0f, 0.93f, 0.24f, 1.0f,	1.0f, 0.0f,			1.0f,
		 1.5f,  0.5f, 0.0f,1.0f, 0.93f, 0.24f, 1.0f,	1.0f, 1.0f,			1.0f,
		0.5f,  0.5f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f,	0.0f, 1.0f,			1.0f

	};

	m_QuadVB = VertexBuffer::Create(sizeof(vertices), vertices);
	m_QuadVB->SetLayout({
		{ ShaderDataType::Float3, "a_Position"},
		{ ShaderDataType::Float4, "a_Color"},
		{ ShaderDataType::Float2, "a_TexCoord"},
		{ ShaderDataType::Float, "a_TexIndex"}
    });
	m_QuadVA->AddVertexBuffer(m_QuadVB);


	uint32_t indices[] = { 
		0, 1, 2, 2, 3, 0 ,
		4, 5, 6, 6, 7, 4
	};

	m_QuadIB = IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices);
	m_QuadVA->SetIndexBuffer(m_QuadIB);


	m_SnowTexture = Texture2D::Create("assets/textures/snow_field_aerial_col_1k.png");
	m_BrickTexture = Texture2D::Create("assets/textures/brick_floor_003_diffuse_1k.png");

	glUseProgram(m_Shader->GetRendererID());
	int samplers[2] = { 0 , 1 };
	m_Shader->SetIntArray("u_Textures", samplers, 2);
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnEvent(Event& event)
{
	m_CameraController.OnEvent(event);

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>(
		[&](MouseButtonPressedEvent& e)
		{
			m_SquareColor = m_SquareAlternateColor;
			return false;
		});
	dispatcher.Dispatch<MouseButtonReleasedEvent>(
		[&](MouseButtonReleasedEvent& e)
		{
			m_SquareColor = m_SquareBaseColor;
			return false;
		});
}

void ExampleLayer::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_Shader->GetRendererID());

	m_Shader->SetMat4("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());

	m_QuadVA->Bind();

	m_SnowTexture->Bind(0);
	m_BrickTexture->Bind(1);

	m_Shader->SetMat4("u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Controls");
	if (ImGui::ColorEdit4("Square Base Color", glm::value_ptr(m_SquareBaseColor)))
		m_SquareColor = m_SquareBaseColor;
	ImGui::ColorEdit4("Square Alternate Color", glm::value_ptr(m_SquareAlternateColor));
	ImGui::End();
}
