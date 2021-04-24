#pragma once

#include <glm/glm.hpp>
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "Texture.h"
namespace GLCore::Utils
{
    class Renderer 
    {
    public:
        static void Init();
        static void Shutdown();
        static void BeginScene(const OrthographicCamera& camera);
        static void BeginScene(const PerspectiveCamera& camera);

        static void BeginBatch();
        static void EndBatch();
        static void Flush();

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);


        struct Statistics
        {
            uint32_t DrawCount = 0;
            uint32_t QuadCount = 0;
        };

        static const Statistics& GetStats();
        static void ResetStats();
    };
}

