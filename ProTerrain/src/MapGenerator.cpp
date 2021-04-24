#include "MapGenerator.h"
#include <vector>

MapGenerator::MapGenerator(int mapWidth, int mapHeight, float noiseScale)
    : m_MapWidth(mapWidth), m_MapHeight(mapHeight), m_NoiseScale(noiseScale)
{}

void MapGenerator::GenerateMap()
{
    //std::vector<std::vector<float>> noiseMap = Noise::GenerateNoiseMap(m_MapWidth, m_MapHeight, m_NoiseScale);
    //MapDisplay display;
    //display.DrawNoiseMap(noiseMap);
}
