#pragma once

#include "PerspectiveCamera.h"

namespace GLCore
{
    class MeshRenderer
    {
    public:
        static void Init();
        static void Shutdown();
        static void BeginScene(const PerspectiveCamera& camera);

        static void Draw();

        struct TerrainStats
        {
            float Lacunarity = 5.0f;
            float Persistance = 0.5f;
        };

        static TerrainStats& GetTerrainStats();

    };
}

