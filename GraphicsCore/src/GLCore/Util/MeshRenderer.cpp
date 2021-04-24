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

        // Initialize noise and noise map
        int seed = 1996;
        s_Data.Noise = PerlinNoise(seed);
        s_Data.NoiseMap = new float[s_Data.NoiseMapWidth * s_Data.NoiseMapHeight]{ 0 };

        float maxVal = 0;
        for (uint32_t j = 0; j < s_Data.NoiseMapHeight; ++j) {
            for (uint32_t i = 0; i < s_Data.NoiseMapWidth; ++i) {
                float fractal = 0;
                float amplitude = 1;
                glm::vec3 pt = glm::vec3(i, 0, j) * (1 / 128.f);
                for (uint32_t k = 0; k < s_Data.NumLayers; ++k) {
                    fractal += (1.0f + s_Data.Noise.eval(pt)) * 0.5f * amplitude;
                    pt *= 2.0f;
                    amplitude *= 0.5;
                }
                if (fractal > maxVal) maxVal = fractal;
                s_Data.NoiseMap[j * s_Data.NoiseMapWidth + i] = fractal;
            }
        }

        for (uint32_t i = 0; i < s_Data.NoiseMapWidth * s_Data.NoiseMapHeight; ++i)
        {
            s_Data.NoiseMap[i] /= maxVal;
        }

        // displace y coordinate of every vertex
        for (uint32_t i = 0; i < s_Data.Mesh->numVertices; ++i) {
            glm::vec2 st = s_Data.Mesh->m_TextureCoords[i];
            uint32_t x = std::min(static_cast<uint32_t>(st.x * s_Data.NoiseMapWidth), s_Data.NoiseMapWidth - 1);
            uint32_t y = std::min(static_cast<uint32_t>(st.y * s_Data.NoiseMapHeight), s_Data.NoiseMapHeight - 1);
            s_Data.Mesh->m_VerticesPos[i].y = 2 * s_Data.NoiseMap[y * s_Data.NoiseMapWidth + x] - 1;
            s_Data.Mesh->m_Normals[i] = glm::vec3(0.0f, 1.0f, 0.0f);
        }

       
        for (uint32_t k = 0, off = 0; k < s_Data.Mesh->m_NumFaces; ++k) {
            uint32_t nverts = 4;
            const glm::vec3& va = s_Data.Mesh->m_VerticesPos[s_Data.Mesh->verticesArray[off]];
            const glm::vec3& vb = s_Data.Mesh->m_VerticesPos[s_Data.Mesh->verticesArray[off + 1]];
            const glm::vec3& vc = s_Data.Mesh->m_VerticesPos[s_Data.Mesh->verticesArray[off + nverts - 1]];

            glm::vec3 tangent = vb - va;
            glm::vec3 bitangent = vc - va;

            s_Data.Mesh->m_Normals[s_Data.Mesh->verticesArray[off]] = glm::normalize(glm::cross(bitangent,tangent));
            off += nverts;
        }


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
