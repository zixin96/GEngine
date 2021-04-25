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
    };
}

