#include "glpch.h"
#include "PerlinNoise.h"
#include <random>
#include <functional>
#include <glm/gtc/constants.hpp>
#include "Utils.h"


PerlinNoise::PerlinNoise(const uint32_t& seed /*= 1996*/)
{
    std::mt19937 generator(seed);
    std::uniform_real_distribution<float> distribution;
    auto dice = std::bind(distribution, generator);
    for (uint32_t i = 0; i < m_TableSize; ++i) {
        // we need to make sure m_Gradients are uniformly distributed
        float theta = acos(2 * dice() - 1);
        float phi = 2 * dice() * glm::pi<float>();

        float x = cos(phi) * sin(theta);
        float y = sin(phi) * sin(theta);
        float z = cos(theta);
        m_Gradients[i] = glm::vec3(x, y, z);
        m_PermutationTable[i] = i;
    }
    std::uniform_int_distribution<unsigned> distributionInt;
    auto diceInt = std::bind(distributionInt, generator);
    // create permutation table
    for (unsigned i = 0; i < m_TableSize; ++i)
    {
        std::swap(m_PermutationTable[i], m_PermutationTable[diceInt() & m_TableSizeMask]);
    }
    // extend the permutation table in the index range [256:512]
    for (unsigned i = 0; i < m_TableSize; ++i) 
    {
        m_PermutationTable[m_TableSize + i] = m_PermutationTable[i];
    }
}

float PerlinNoise::eval(const glm::vec3& point, glm::vec3& derivative) const
{
    int xi0 = ((int)std::floor(point.x)) & m_TableSizeMask;
    int yi0 = ((int)std::floor(point.y)) & m_TableSizeMask;
    int zi0 = ((int)std::floor(point.z)) & m_TableSizeMask;

    int xi1 = (xi0 + 1) & m_TableSizeMask;
    int yi1 = (yi0 + 1) & m_TableSizeMask;
    int zi1 = (zi0 + 1) & m_TableSizeMask;

    float tx = point.x - ((int)std::floor(point.x));
    float ty = point.y - ((int)std::floor(point.y));
    float tz = point.z - ((int)std::floor(point.z));

    float u = quintic(tx);
    float v = quintic(ty);
    float w = quintic(tz);

    // generate vectors going from the grid points to p
    float x0 = tx, x1 = tx - 1;
    float y0 = ty, y1 = ty - 1;
    float z0 = tz, z1 = tz - 1;

    float a = gradientDotV(hash(xi0, yi0, zi0), x0, y0, z0);
    float b = gradientDotV(hash(xi1, yi0, zi0), x1, y0, z0);
    float c = gradientDotV(hash(xi0, yi1, zi0), x0, y1, z0);
    float d = gradientDotV(hash(xi1, yi1, zi0), x1, y1, z0);
    float e = gradientDotV(hash(xi0, yi0, zi1), x0, y0, z1);
    float f = gradientDotV(hash(xi1, yi0, zi1), x1, y0, z1);
    float g = gradientDotV(hash(xi0, yi1, zi1), x0, y1, z1);
    float h = gradientDotV(hash(xi1, yi1, zi1), x1, y1, z1);

    float du = quinticDeriv(tx);
    float dv = quinticDeriv(ty);
    float dw = quinticDeriv(tz);

    // Analytical approach to compute noise value and normal

    float k0 = a;
    float k1 = (b - a);
    float k2 = (c - a);
    float k3 = (e - a);
    float k4 = (a + d - b - c);
    float k5 = (a + f - b - e);
    float k6 = (a + g - c - e);
    float k7 = (b + c + e + h - a - d - f - g);

    derivative.x = du * (k1 + k4 * v + k5 * w + k7 * v * w);
    derivative.y = dv * (k2 + k4 * u + k6 * w + k7 * v * w);
    derivative.z = dw * (k3 + k5 * u + k6 * v + k7 * v * w);

    return k0 + k1 * u + k2 * v + k3 * w + k4 * u * v + k5 * u * w + k6 * v * w + k7 * u * v * w;
}

float PerlinNoise::eval(const glm::vec3& point) const
{
    int xi0 = ((int)std::floor(point.x)) & m_TableSizeMask;
    int yi0 = ((int)std::floor(point.y)) & m_TableSizeMask;
    int zi0 = ((int)std::floor(point.z)) & m_TableSizeMask;

    int xi1 = (xi0 + 1) & m_TableSizeMask;
    int yi1 = (yi0 + 1) & m_TableSizeMask;
    int zi1 = (zi0 + 1) & m_TableSizeMask;

    float tx = point.x - ((int)std::floor(point.x));
    float ty = point.y - ((int)std::floor(point.y));
    float tz = point.z - ((int)std::floor(point.z));

    float u = quintic(tx);
    float v = quintic(ty);
    float w = quintic(tz);

    // m_Gradients at the corner of the cell
    const glm::vec3& c000 = m_Gradients[hash(xi0, yi0, zi0)];
    const glm::vec3& c100 = m_Gradients[hash(xi1, yi0, zi0)];
    const glm::vec3& c010 = m_Gradients[hash(xi0, yi1, zi0)];
    const glm::vec3& c110 = m_Gradients[hash(xi1, yi1, zi0)];

    const glm::vec3& c001 = m_Gradients[hash(xi0, yi0, zi1)];
    const glm::vec3& c101 = m_Gradients[hash(xi1, yi0, zi1)];
    const glm::vec3& c011 = m_Gradients[hash(xi0, yi1, zi1)];
    const glm::vec3& c111 = m_Gradients[hash(xi1, yi1, zi1)];

    // generate vectors going from the grid points to point
    float x0 = tx, x1 = tx - 1;
    float y0 = ty, y1 = ty - 1;
    float z0 = tz, z1 = tz - 1;

    glm::vec3 p000 = glm::vec3(x0, y0, z0);
    glm::vec3 p100 = glm::vec3(x1, y0, z0);
    glm::vec3 p010 = glm::vec3(x0, y1, z0);
    glm::vec3 p110 = glm::vec3(x1, y1, z0);

    glm::vec3 p001 = glm::vec3(x0, y0, z1);
    glm::vec3 p101 = glm::vec3(x1, y0, z1);
    glm::vec3 p011 = glm::vec3(x0, y1, z1);
    glm::vec3 p111 = glm::vec3(x1, y1, z1);

    // linear interpolation
    float a = lerp(dot(c000, p000), dot(c100, p100), u);
    float b = lerp(dot(c010, p010), dot(c110, p110), u);
    float c = lerp(dot(c001, p001), dot(c101, p101), u);
    float d = lerp(dot(c011, p011), dot(c111, p111), u);

    float e = lerp(a, b, v);
    float f = lerp(c, d, v);

    return lerp(e, f, w); // g
}

void PerlinNoise::ComputeDeriv(const glm::vec3& point, glm::vec3& derivative) const
{
    int xi0 = ((int)std::floor(point.x)) & m_TableSizeMask;
    int yi0 = ((int)std::floor(point.y)) & m_TableSizeMask;
    int zi0 = ((int)std::floor(point.z)) & m_TableSizeMask;

    int xi1 = (xi0 + 1) & m_TableSizeMask;
    int yi1 = (yi0 + 1) & m_TableSizeMask;
    int zi1 = (zi0 + 1) & m_TableSizeMask;

    float tx = point.x - ((int)std::floor(point.x));
    float ty = point.y - ((int)std::floor(point.y));
    float tz = point.z - ((int)std::floor(point.z));

    float u = quintic(tx);
    float v = quintic(ty);
    float w = quintic(tz);

    // generate vectors going from the grid points to p
    float x0 = tx, x1 = tx - 1;
    float y0 = ty, y1 = ty - 1;
    float z0 = tz, z1 = tz - 1;

    float a = gradientDotV(hash(xi0, yi0, zi0), x0, y0, z0);
    float b = gradientDotV(hash(xi1, yi0, zi0), x1, y0, z0);
    float c = gradientDotV(hash(xi0, yi1, zi0), x0, y1, z0);
    float d = gradientDotV(hash(xi1, yi1, zi0), x1, y1, z0);
    float e = gradientDotV(hash(xi0, yi0, zi1), x0, y0, z1);
    float f = gradientDotV(hash(xi1, yi0, zi1), x1, y0, z1);
    float g = gradientDotV(hash(xi0, yi1, zi1), x0, y1, z1);
    float h = gradientDotV(hash(xi1, yi1, zi1), x1, y1, z1);

    float du = quinticDeriv(tx);
    float dv = quinticDeriv(ty);
    float dw = quinticDeriv(tz);

    // Analytical approach to compute noise value and normal

    float k0 = a;
    float k1 = (b - a);
    float k2 = (c - a);
    float k3 = (e - a);
    float k4 = (a + d - b - c);
    float k5 = (a + f - b - e);
    float k6 = (a + g - c - e);
    float k7 = (b + c + e + h - a - d - f - g);

    derivative.x = du * (k1 + k4 * v + k5 * w + k7 * v * w);
    derivative.y = dv * (k2 + k4 * u + k6 * w + k7 * v * w);
    derivative.z = dw * (k3 + k5 * u + k6 * v + k7 * v * w);
}

uint8_t PerlinNoise::hash(const int& x, const int& y, const int& z) const
{
    return m_PermutationTable[m_PermutationTable[m_PermutationTable[x] + y] + z];
}

float PerlinNoise::gradientDotV(uint8_t permutation, float x, float y, float z) const
{
    switch (permutation & 15) {
        case  0: return  x + y; // (1,1,0)
        case  1: return -x + y; // (-1,1,0)
        case  2: return  x - y; // (1,-1,0)
        case  3: return -x - y; // (-1,-1,0)
        case  4: return  x + z; // (1,0,1)
        case  5: return -x + z; // (-1,0,1)
        case  6: return  x - z; // (1,0,-1)
        case  7: return -x - z; // (-1,0,-1)
        case  8: return  y + z; // (0,1,1),
        case  9: return -y + z; // (0,-1,1),
        case 10: return  y - z; // (0,1,-1),
        case 11: return -y - z; // (0,-1,-1)
        case 12: return  y + x; // (1,1,0)
        case 13: return -x + y; // (-1,1,0)
        case 14: return -y + z; // (0,-1,1)
        case 15: return -y - z; // (0,-1,-1)
    }
    __debugbreak();
    return 0.0f;
}
