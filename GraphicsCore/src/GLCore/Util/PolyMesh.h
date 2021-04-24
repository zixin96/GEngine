#pragma once
#include <glm/glm.hpp>

namespace GLCore::Utils
{
    class PolyMesh
    {
    public:
        PolyMesh(uint32_t width, uint32_t height, uint32_t subdivisionWidth, uint32_t subdivisionHeight);
        ~PolyMesh();

        glm::vec3* m_VerticesPos;
        glm::vec2* m_TextureCoords;
        glm::vec3* m_Normals;
        uint32_t* m_Indices;
        uint32_t m_TriangleIndex = 0;
        
        void AddTriangle(uint32_t a, uint32_t b, uint32_t c);

        uint32_t m_NumFaces;
        uint32_t numVertices;
        uint32_t* verticesArray;
    };
}


