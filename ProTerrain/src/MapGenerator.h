#pragma once

class MapGenerator
{
public:
    MapGenerator(int mapWidth, int mapHeight, float noiseScale);
    void GenerateMap();
private:
    int m_MapWidth;
    int m_MapHeight;
    float m_NoiseScale;
};

