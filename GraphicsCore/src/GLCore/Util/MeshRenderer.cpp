#include "glpch.h"
#include "MeshRenderer.h"
#include <GLCoreUtils.h>
#include "GLCore/Util/PerlinNoise.h"

namespace GLCore::Utils
{

    struct RendererData
    {
        Ref<VertexArray> MeshVertexArray;
        Ref<VertexBuffer> MeshVertexBuffer;
        Ref<IndexBuffer> MeshIndexBuffer;
        Ref<Shader> TextureShader;

        PolyMesh* Mesh;
        PerlinNoise Noise;
        float* NoiseMap;
        uint32_t NoiseMapWidth = 512;
        uint32_t NoiseMapHeight = 512;
        uint32_t NumLayers = 5;
    };

    static RendererData s_Data;

    void MeshRenderer::Init()
    {
        s_Data.Mesh = new PolyMesh(3, 3, 100, 100);

        s_Data.MeshVertexArray = VertexArray::Create();
        
        std::vector<float> vertices;
        vertices.reserve(s_Data.Mesh->numVertices * 8);

        for (size_t i = 0; i < s_Data.Mesh->numVertices; i++)
        {
            vertices.push_back(s_Data.Mesh->m_VerticesPos[i].x);
            vertices.push_back(s_Data.Mesh->m_VerticesPos[i].y);
            vertices.push_back(s_Data.Mesh->m_VerticesPos[i].z);

            vertices.push_back(s_Data.Mesh->m_Normals[i].x);
            vertices.push_back(s_Data.Mesh->m_Normals[i].y);
            vertices.push_back(s_Data.Mesh->m_Normals[i].z);

            vertices.push_back(s_Data.Mesh->m_TextureCoords[i].x);
            vertices.push_back(s_Data.Mesh->m_TextureCoords[i].y);
        }
        s_Data.MeshVertexBuffer = VertexBuffer::Create(vertices.size() * sizeof(float), vertices.data());

        s_Data.MeshVertexBuffer->SetLayout({
            { ShaderDataType::Float3,	"a_Position" },
            { ShaderDataType::Float3,	"a_Normal" },
            { ShaderDataType::Float2,	"a_TexCoord" },
        });
        s_Data.MeshVertexArray->AddVertexBuffer(s_Data.MeshVertexBuffer);
        
        s_Data.MeshIndexBuffer = IndexBuffer::Create(s_Data.Mesh->m_NumFaces * 6, s_Data.Mesh->m_Indices);
        s_Data.MeshVertexArray->SetIndexBuffer(s_Data.MeshIndexBuffer);

        s_Data.TextureShader = Shader::Create("terrain",
            Shader::ReadFileAsString("assets/shaders/terrain.vert.glsl"),
            Shader::ReadFileAsString("assets/shaders/terrain.frag.glsl"));
    }

    void MeshRenderer::Shutdown()
    {
        delete s_Data.Mesh; 
        delete[] s_Data.NoiseMap;
    }

    void MeshRenderer::BeginScene(const PerspectiveCamera& camera)
    {
        glm::mat4 viewProj = camera.GetViewProjection();
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection",
            viewProj);
    }

    void MeshRenderer::Draw()
    {
        s_Data.MeshVertexArray->Bind();
        glDrawElements(GL_TRIANGLES, s_Data.MeshIndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

}
