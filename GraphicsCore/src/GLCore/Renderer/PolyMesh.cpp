#include "glpch.h"
#include "PolyMesh.h"


namespace GLCore
{

    PolyMesh::PolyMesh(uint32_t width, uint32_t height, uint32_t subdivisionWidth, uint32_t subdivisionHeight)
    {
        uint32_t numVerticesAlongX = subdivisionWidth + 1;
        uint32_t numVerticesAlongZ = subdivisionHeight + 1;
        numVertices = numVerticesAlongX * numVerticesAlongZ;

        m_VerticesPos = new glm::vec3[numVertices];
        m_Normals = new glm::vec3[numVertices]; 
        m_TextureCoords = new glm::vec2[numVertices];

        float invSubdivisionWidth = 1.f / subdivisionWidth;
        float invSubdivisionHeight = 1.f / subdivisionHeight;

        for (uint32_t j = 0; j <= subdivisionHeight; ++j) {
            for (uint32_t i = 0; i <= subdivisionWidth; ++i) {
               m_VerticesPos[j * (subdivisionWidth + 1) + i] = glm::vec3(width * (i * invSubdivisionWidth - 0.5), 0, height * (j * invSubdivisionHeight - 0.5));
               m_TextureCoords[j * (subdivisionWidth + 1) + i] = glm::vec2(i * invSubdivisionWidth, j * invSubdivisionHeight);
            }
        }

        int vertexIndex = 0;
        m_NumFaces = subdivisionWidth * subdivisionHeight;
        m_Indices = new uint32_t[m_NumFaces * 6];

        for (uint32_t z = 0; z < numVerticesAlongZ; ++z)
        {
            for (uint32_t x = 0; x < numVerticesAlongX; ++x)
            {
                if (x < numVerticesAlongX - 1 && z < numVerticesAlongZ - 1)
                {
                    AddTriangle(vertexIndex, vertexIndex + numVerticesAlongX + 1, vertexIndex + numVerticesAlongX);
                    AddTriangle(vertexIndex + numVerticesAlongX + 1, vertexIndex, vertexIndex + 1);
                }
                vertexIndex++;
            }
        }

        verticesArray = new uint32_t[4 * m_NumFaces];
        for (uint32_t j = 0, k = 0; j < subdivisionHeight; ++j) {
            for (uint32_t i = 0; i < subdivisionWidth; ++i) {
                verticesArray[k] = j * (subdivisionWidth + 1) + i;
                verticesArray[k + 1] = j * (subdivisionWidth + 1) + i + 1;
                verticesArray[k + 2] = (j + 1) * (subdivisionWidth + 1) + i + 1;
                verticesArray[k + 3] = (j + 1) * (subdivisionWidth + 1) + i;
                k += 4;
            }
        }
        
    }

    PolyMesh::~PolyMesh()
    {
        delete[] m_VerticesPos;
        delete[] m_TextureCoords;
        delete[] m_Normals;
        delete[] m_Indices;

        delete[] verticesArray;
    }

    void PolyMesh::AddTriangle(uint32_t a, uint32_t b, uint32_t c)
    {
        m_Indices[m_TriangleIndex] = a;
        m_Indices[m_TriangleIndex + 1] = b;
        m_Indices[m_TriangleIndex + 2] = c;
        m_TriangleIndex += 3;
    }

}

