#include "glpch.h"
#include "MeshRenderer.h"
#include "GLCore/Util/PerlinNoise.h"
#include "GLCore/Renderer/VertexArray.h"
#include "GLCore/Renderer/Shader.h"
#include "GLCore/Renderer/PolyMesh.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

namespace GLCore
{
    const glm::vec4 lightColor{ 1.0f, 1.0f, 1.0f, 1.0f };
    glm::vec3 lightPos{ 5.0f, 20.0f, 5.0f };

    struct MeshVertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
    };

    struct RendererData
    {
        Ref<VertexArray> MeshVertexArray;
        Ref<VertexBuffer> MeshVertexBuffer;
        Ref<IndexBuffer> MeshIndexBuffer;
        Ref<Shader> TerrainShader;

        PolyMesh* Mesh;
        PerlinNoise Noise;
        float* NoiseMap;
        uint32_t NoiseMapWidth = 512;
        uint32_t NoiseMapHeight = 512;
        uint32_t NumLayers = 5;

        MeshRenderer::TerrainStats Stats;


        Ref<VertexArray> LightSourceVertexArray;
        Ref<VertexBuffer> LightSourceVertexBuffer;
        Ref<IndexBuffer> LightSourceIndexBuffer;
        Ref<Shader> LightSourceShader;


        PerspectiveCamera* Cam;
    };

    static RendererData s_Data;

    void MeshRenderer::Init()
    {
        // Initialize mesh and noise map
        s_Data.Mesh = new PolyMesh(5, 5, 200, 200);
        int seed = 1996;
        s_Data.Noise = PerlinNoise(seed);
        s_Data.NoiseMap = new float[s_Data.NoiseMapWidth * s_Data.NoiseMapHeight]{ 0 };

        s_Data.TerrainShader = Shader::Create("terrain",
            Shader::ReadFileAsString("assets/shaders/terrain.vert.glsl"),
            Shader::ReadFileAsString("assets/shaders/terrain.frag.glsl"));

        RecomputeTerrainData();

        s_Data.MeshVertexArray = VertexArray::Create();
        s_Data.MeshVertexBuffer = VertexBuffer::Create(sizeof(MeshVertex) * s_Data.Mesh->numVertices);

        s_Data.MeshVertexBuffer->SetLayout({
            { ShaderDataType::Float3,	"a_Position" },
            { ShaderDataType::Float3,	"a_Normal" },
            { ShaderDataType::Float2,	"a_TexCoord" },
        });
        s_Data.MeshVertexArray->AddVertexBuffer(s_Data.MeshVertexBuffer);
        
        s_Data.MeshIndexBuffer = IndexBuffer::Create(s_Data.Mesh->m_NumFaces * 6, s_Data.Mesh->m_Indices);
        s_Data.MeshVertexArray->SetIndexBuffer(s_Data.MeshIndexBuffer);


        s_Data.LightSourceShader = Shader::Create("lightSource",
            Shader::ReadFileAsString("assets/shaders/lightSource.vert.glsl"),
            Shader::ReadFileAsString("assets/shaders/lightSource.frag.glsl"));

        s_Data.LightSourceVertexArray = VertexArray::Create();
        float cubeVertices[] = {
            // front
            -1.0, -1.0,  1.0,
             1.0, -1.0,  1.0,
             1.0,  1.0,  1.0,
            -1.0,  1.0,  1.0,
            // back
            -1.0, -1.0, -1.0,
             1.0, -1.0, -1.0,
             1.0,  1.0, -1.0,
            -1.0,  1.0, -1.0
        };
        s_Data.LightSourceVertexBuffer = VertexBuffer::Create(sizeof(cubeVertices), cubeVertices);
        s_Data.LightSourceVertexBuffer->SetLayout({
            { ShaderDataType::Float3,	"a_Position" },
        });
        s_Data.LightSourceVertexArray->AddVertexBuffer(s_Data.LightSourceVertexBuffer);

        uint32_t cubeIndices[] =
        {
            // front
            0, 1, 2,
            2, 3, 0,
            // right
            1, 5, 6,
            6, 2, 1,
            // back
            7, 6, 5,
            5, 4, 7,
            // left
            4, 0, 3,
            3, 7, 4,
            // bottom
            4, 5, 1,
            1, 0, 4,
            // top
            3, 2, 6,
            6, 7, 3
        };
        s_Data.LightSourceIndexBuffer = IndexBuffer::Create(36, cubeIndices);
        s_Data.LightSourceVertexArray->SetIndexBuffer(s_Data.LightSourceIndexBuffer);
    }

    void MeshRenderer::Shutdown()
    {
        delete s_Data.Mesh; 
        delete[] s_Data.NoiseMap;
    }

    void MeshRenderer::BeginScene(PerspectiveCamera& camera)
    {
        s_Data.Cam = &camera;
        s_Data.TerrainShader->Bind();
        glm::mat4 viewProj = camera.GetViewProjection();
        s_Data.TerrainShader->SetMat4("u_ViewProjection",
            viewProj);
        s_Data.TerrainShader->SetMat4("u_Model",
            glm::scale(glm::mat4(1.0f), glm::vec3(10.0f)));
        float timeValue = static_cast<float>(glfwGetTime());
        float changing = (sin(timeValue) / 2.0f) + 0.5f;
        s_Data.TerrainShader->SetFloat("u_Changing",
            changing);
        s_Data.TerrainShader->SetVec4("u_LightColor",
            lightColor);
   

        s_Data.LightSourceShader->Bind();
        s_Data.LightSourceShader->SetMat4("u_ViewProjection",
            viewProj);
      
        s_Data.LightSourceShader->SetVec4("u_LightColor",
            lightColor);
    }

    void MeshRenderer::Draw()
    {
        lightPos.x = 1.0f + sin(glfwGetTime()) * 20.0f;
        lightPos.z = sin(glfwGetTime() / 2.0f) * 10.0f;

        s_Data.TerrainShader->Bind();
        s_Data.TerrainShader->SetVec3("u_ViewPos",
            s_Data.Cam->GetPosition());
        s_Data.TerrainShader->SetVec3("u_LightPos",
            lightPos);
        // Set dynamic vertex buffer
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
        s_Data.MeshVertexArray->Bind();
        s_Data.MeshVertexBuffer->SetData(vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(float)));
        glDrawElements(GL_TRIANGLES, s_Data.MeshIndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);


        s_Data.LightSourceShader->Bind();
        s_Data.LightSourceShader->SetMat4("u_Model",
            glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
        s_Data.LightSourceVertexArray->Bind();
        glDrawElements(GL_TRIANGLES, s_Data.LightSourceIndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void MeshRenderer::RecomputeTerrainData()
    {
        float maxVal = 0;
        for (uint32_t j = 0; j < s_Data.NoiseMapHeight; ++j) {
            for (uint32_t i = 0; i < s_Data.NoiseMapWidth; ++i) {
                float fractal = 0;
                float amplitude = 1;
                glm::vec3 pt = glm::vec3(i, 0, j) * (1 / 128.f);
                for (uint32_t k = 0; k < s_Data.NumLayers; ++k) {
                    fractal += (1.0f + s_Data.Noise.eval(pt)) * 0.5f * amplitude;
                    pt *= s_Data.Stats.Lacunarity;
                    amplitude *= s_Data.Stats.Persistance;
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

            s_Data.Mesh->m_Normals[s_Data.Mesh->verticesArray[off]] = glm::normalize(glm::cross(bitangent, tangent));
            off += nverts;
        }
    }

    MeshRenderer::TerrainStats& MeshRenderer::GetTerrainStats()
    {
        return s_Data.Stats;
    }

}
