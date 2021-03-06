#include "glpch.h"
#include "Renderer.h"
#include "GLCore/Util/PerlinNoise.h"
#include "GLCore/Renderer/VertexArray.h"
#include "GLCore/Renderer/Shader.h"
#include <glad/glad.h>

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
        Ref<Shader> TerrainShader;
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

        s_Data.TerrainShader = Shader::Create("test",
            Shader::ReadFileAsString("assets/shaders/test.vert.glsl"),
            Shader::ReadFileAsString("assets/shaders/test.frag.glsl"));
        s_Data.TerrainShader->Bind();
        s_Data.TerrainShader->SetIntArray("u_Textures", samplers, MaxTextureSlots);

        s_Data.TextureSlots[0] = s_Data.WhiteTexture;
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
        DrawQuad({ position.x, position.y, 0.0f  }, size, color);
    }

    void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture);
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

        const glm::vec3 vertexPositions[] = {
            {position.x,            position.y,                position.z},
            {position.x + size.x,   position.y,                position.z},
            {position.x + size.x,   position.y + size.y,                position.z},
            {position.x,            position.y + size.y,                position.z}
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
        s_Data.TerrainShader->Bind();
        s_Data.TerrainShader->SetMat4("u_ViewProjection",
            camera.GetViewProjectionMatrix());
    }

    void Renderer::BeginScene(const PerspectiveCamera& camera)
    {
        glm::mat4 viewProj = camera.GetViewProjection();
        s_Data.TerrainShader->Bind();
        s_Data.TerrainShader->SetMat4("u_ViewProjection",
            viewProj);
    }
}
