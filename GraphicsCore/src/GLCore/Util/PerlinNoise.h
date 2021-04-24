#pragma once
#include <stdint.h>
#include <glm/glm.hpp>

class PerlinNoise
{
public:
    PerlinNoise(const uint32_t& seed = 1996);
    ~PerlinNoise() = default;
    float eval(const glm::vec3& point, glm::vec3 derivative) const;
private:
    uint8_t hash(const int& x, const int& y, const int& z) const;
    float gradientDotV(uint8_t permutation, float x, float y, float z) const;
private:
    static const uint32_t m_TableSize = 256;
    static const uint32_t m_TableSizeMask = m_TableSize - 1;
    glm::vec3 m_Gradients[m_TableSize];
    uint32_t m_PermutationTable[m_TableSize * 2];
};

