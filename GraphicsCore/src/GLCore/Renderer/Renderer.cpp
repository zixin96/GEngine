#include "glpch.h"
#include "Renderer.h"
#include <GLCoreUtils.h>
#include "GLCore/Util/PerlinNoise.h"

namespace GLCore
{
    static const uint32_t MaxQuadCount = 10000;
    static const uint32_t MaxVertexCount = MaxQuadCount * 4;
    static const uint32_t MaxIndexCount = MaxQuadCount * 6;
    static const uint32_t MaxTextureSlots = 32;	
   
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoords;
        float TexIndex;
    };

    struct RendererData
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Texture2D> WhiteTexture; // White texture is used when we don't want to apply any textures
        Ref<Shader> TextureShader;
        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1;  // 0 is white texture

        Renderer::Statistics Stats;

        PerlinNoise Noise;
        float* NoiseMap;
        uint32_t NoiseMapWidth = 512;
        uint32_t NoiseMapHeight = 512;
        uint32_t NumLayers = 5;
    };

    static RendererData s_Data;

    void Renderer::Init()
    {
        s_Data.QuadVertexArray = VertexArray::Create();
        s_Data.QuadVertexBuffer = VertexBuffer::Create(MaxVertexCount * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->SetLayout({
            { ShaderDataType::Float3,	"a_Position" },
            { ShaderDataType::Float4,	"a_Color" },
            { ShaderDataType::Float2,	"a_TexCoord" },
            { ShaderDataType::Float,	"a_TexIndex" }
            });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        s_Data.QuadVertexBufferBase = new QuadVertex[MaxVertexCount];

        uint32_t* quadIndices = new uint32_t[MaxIndexCount];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < MaxIndexCount; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> quadEBO;
        quadEBO = IndexBuffer::Create(MaxIndexCount, quadIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadEBO);
        delete[] quadIndices;

        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[MaxTextureSlots];
        for (uint32_t i = 0; i < MaxTextureSlots; i++)
        {
            samplers[i] = i;
        }

        s_Data.TextureShader = Shader::Create("test",
            Shader::ReadFileAsString("assets/shaders/test.vert.glsl"),
            Shader::ReadFileAsString("assets/shaders/test.frag.glsl"));
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetIntArray("u_Textures", samplers, MaxTextureSlots);

        s_Data.TextureSlots[0] = s_Data.WhiteTexture;

        
        // Initialize noise and noisemap
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
                    // TODO: need a eval taking only a point?
                    // fractal += (1.0f + s_Data.Noise.eval(pt)) * 0.5f * amplitude;
                    pt *= 2.0f;
                    amplitude *= 0.5;
                }
                if (fractal > maxVal) maxVal = fractal;
                s_Data.NoiseMap[j * s_Data.NoiseMapWidth + i] = fractal;
            }
        }

        for (uint32_t i = 0; i < s_Data.NoiseMapWidth * s_Data.NoiseMapHeight; ++i)
            s_Data.NoiseMap[i] /= maxVal;



    }

    void Renderer::Shutdown()
    {
        delete[] s_Data.QuadVertexBufferBase;
        delete[] s_Data.NoiseMap;
    }

    void Renderer::BeginBatch()
    {
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
    }

    void Renderer::EndBatch()
    {
        GLsizeiptr size = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, (uint32_t)size);
    }

    void Renderer::Flush()
    {
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        {
            s_Data.TextureSlots[i]->Bind(i);
        }
        s_Data.QuadVertexArray->Bind();
        glDrawElements(GL_TRIANGLES, s_Data.QuadIndexCount, GL_UNSIGNED_INT, nullptr);
        s_Data.Stats.DrawCount++;
        s_Data.QuadIndexCount = 0;
        s_Data.TextureSlotIndex = 1;
    }

    void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, 0.0f,position.y }, size, color);
    }

    void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
    {
        DrawQuad({ position.x, 0.0f,  position.y }, size, texture);
    }

    void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        if (s_Data.QuadIndexCount >= MaxIndexCount)
        {
            EndBatch();
            Flush();
            BeginBatch();
        }

        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };


        

        // displace: postprocessing step
//         for (uint32_t i = 0; i < poly->numVertices; ++i) {
//             glm::vec2 st = poly->st[i];
//             uint32_t x = std::min(static_cast<uint32_t>(st.x * s_Data.NoiseMapWidth), s_Data.NoiseMapWidth - 1);
//             uint32_t y = std::min(static_cast<uint32_t>(st.y * s_Data.NoiseMapHeight), s_Data.NoiseMapHeight - 1);
//             poly->vertices[i].y = 2 * s_Data.NoiseMap[y * s_Data.NoiseMapWidth + x] - 1;
//         }



        glm::vec3 deri1{ 0.0f };
        glm::vec3 deri2{ 0.0f };
        glm::vec3 deri3{ 0.0f };
        glm::vec3 deri4{ 0.0f };

        const glm::vec3 vertexPositions[] = {
            {position.x,            position.y + s_Data.Noise.eval(glm::vec3(position.x,            0.0f, position.z)                      , deri1),                position.z},
            {position.x + size.x,   position.y + s_Data.Noise.eval(glm::vec3(position.x + size.x,   0.0f, position.z)                      , deri2),                position.z},
            {position.x + size.x,   position.y + s_Data.Noise.eval(glm::vec3(position.x + size.x,   0.0f, position.z + size.y)             , deri3),                position.z + size.y},
            {position.x,            position.y + s_Data.Noise.eval(glm::vec3(position.x,            0.0f, position.z + size.y)             , deri4),                position.z + size.y}
        };

        // TODO: Check normals
        const glm::vec3 vertexNormals[] = {
            glm::normalize(glm::vec3(-deri1.x, 1.0f, -deri1.z)),
            glm::normalize(glm::vec3(-deri2.x, 1.0f, -deri2.z)),
            glm::normalize(glm::vec3(-deri3.x, 1.0f, -deri3.z)),
            glm::normalize(glm::vec3(-deri4.x, 1.0f, -deri4.z))
        };


        const float textureIndex = 0.0f;

        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = vertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoords = textureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
    }

    void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
    {
        if (s_Data.QuadIndexCount >= MaxIndexCount || s_Data.TextureSlotIndex > 31)
        {
            EndBatch();
            Flush();
            BeginBatch();
        }

        constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

        // Check if this texture has already been used
        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }

        // This is a new texture!
        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
        const glm::vec3 vertexPositions[] = {
            {position.x,            position.y,             position.z},
            {position.x + size.x,   position.y,             position.z},
            {position.x + size.x,   position.y + size.y,    position.z},
            {position.x,            position.y + size.y,    position.z}
        };

        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = vertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoords = textureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
    }

   
    const Renderer::Statistics& Renderer::GetStats()
    {
        return s_Data.Stats;
    }

    void Renderer::ResetStats()
    {
        memset(&s_Data.Stats, 0, sizeof(Statistics));
    }

    void Renderer::BeginScene(const OrthographicCamera& camera)
    {
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection",
            camera.GetViewProjectionMatrix());
    }

    void Renderer::BeginScene(const PerspectiveCamera& camera)
    {
        glm::mat4 viewProj = camera.GetViewProjection();
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection",
            viewProj);
    }

//     void Renderer::DrawQuad(uint32_t width /*= 1*/, uint32_t height /*= 1*/, uint32_t subdivisionWidth /*= 40*/, uint32_t subdivisionHeight /*= 40*/)
//     {
//         if (s_Data.QuadIndexCount >= MaxIndexCount)
//         {
//             EndBatch();
//             Flush();
//             BeginBatch();
//         }
// 
//         constexpr size_t quadVertexCount = 4;
//         constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
// 
// 
// 
// 
//         // displace: postprocessing step
//         for (uint32_t i = 0; i < poly->numVertices; ++i) {
//             glm::vec2 st = poly->st[i];
//             uint32_t x = std::min(static_cast<uint32_t>(st.x * s_Data.NoiseMapWidth), s_Data.NoiseMapWidth - 1);
//             uint32_t y = std::min(static_cast<uint32_t>(st.y * s_Data.NoiseMapHeight), s_Data.NoiseMapHeight - 1);
//             poly->vertices[i].y = 2 * s_Data.NoiseMap[y * s_Data.NoiseMapWidth + x] - 1;
//         }
// 
// 
// 
//         glm::vec3 deri1{ 0.0f };
//         glm::vec3 deri2{ 0.0f };
//         glm::vec3 deri3{ 0.0f };
//         glm::vec3 deri4{ 0.0f };
// 
//         const glm::vec3 vertexPositions[] = {
//             {position.x,            position.y + s_Data.Noise.eval(glm::vec3(position.x,            0.0f, position.z)                      , deri1),                position.z},
//             {position.x + size.x,   position.y + s_Data.Noise.eval(glm::vec3(position.x + size.x,   0.0f, position.z)                      , deri2),                position.z},
//             {position.x + size.x,   position.y + s_Data.Noise.eval(glm::vec3(position.x + size.x,   0.0f, position.z + size.y)             , deri3),                position.z + size.y},
//             {position.x,            position.y + s_Data.Noise.eval(glm::vec3(position.x,            0.0f, position.z + size.y)             , deri4),                position.z + size.y}
//         };
// 
//         // TODO: Check normals
//         const glm::vec3 vertexNormals[] = {
//             glm::normalize(glm::vec3(-deri1.x, 1.0f, -deri1.z)),
//             glm::normalize(glm::vec3(-deri2.x, 1.0f, -deri2.z)),
//             glm::normalize(glm::vec3(-deri3.x, 1.0f, -deri3.z)),
//             glm::normalize(glm::vec3(-deri4.x, 1.0f, -deri4.z))
//         };
// 
// 
//         const float textureIndex = 0.0f;
// 
//         for (size_t i = 0; i < quadVertexCount; i++)
//         {
//             s_Data.QuadVertexBufferPtr->Position = vertexPositions[i];
//             s_Data.QuadVertexBufferPtr->Color = color;
//             s_Data.QuadVertexBufferPtr->TexCoords = textureCoords[i];
//             s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
//             s_Data.QuadVertexBufferPtr++;
//         }
// 
//         s_Data.QuadIndexCount += 6;
//         s_Data.Stats.QuadCount++;
//     }

}
