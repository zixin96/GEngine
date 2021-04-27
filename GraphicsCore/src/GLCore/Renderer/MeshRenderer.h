#pragma once

#include "PerspectiveCamera.h"

namespace GLCore
{
    class MeshRenderer
    {
    public:
        static void Init();
        static void Shutdown();
        static void BeginScene(PerspectiveCamera& camera);

        static void Draw();
        static void RecomputeTerrainData();

        struct TerrainStats
        {
            float Lacunarity = 2.5f;
            float Persistance = 0.35f;
        };

        static TerrainStats& GetTerrainStats();

    };
}

