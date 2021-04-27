# Simulating Terrain

A procedurally generated terrain built upon my work-in-progress game engine. 

![ProTerrainDemo](ProTerrainDemo.gif)

## Usage

OpenGL Version: **4.6**

C++ Version: **C++17**

Currently only supports Windows, Visual Studio 2019. 

1. Clone the repository: `git clone --recursive https://github.com/zixin96/GEngine.git`
2. Run `Scripts/Win-Premake.bat` - this will create `ProTerrain.sln` for procedural terrain and make sure everything is setup correctly
3. Open `ProTerrain.sln` and build `Debug` `x64` - ProTerrain should be the startup project so really you can just hit `F5` to build and debug the startup project

## Camera Control

Rotate: Alt + Left Mouse Button
Pan: Alt + Middle Mouse Button
Zoom: Mouse Scroll or Alt + Right Mouse Button 

## Terminology

- Amplitude deals with y axis.

- Frequency deals with x axis.

- Octave   
    - Add details while preserving overall shape by layering(adding) multiple levels of noise. 
    - Each level of the noise map is called an octave. 

- Lacunarity: controls increase in frequency of octaves. e.g 2. 
    - Increasing L => Increasing # of small features

- Persistance: controls decrease in amplitude of octaves (range from 0 to 1) e.g 0.5. 
    - Affects how much these small features influence the overall shape of the map. 0: small features doesn't affect the terrain. 1: small features has large impact on the terrain

## Terrain

Terrain is created using fractal Perlin Noise with 5 octaves. Normals are created using gradient method. Phong lighting model is used. 

## References

[1] Hazel. Hazel Engine by TheCherno. 
    Used under Apache-2.0 License.
    https://github.com/TheCherno/Hazel.git

[2] OpenGL. OpenGL support library so you can just dive in and write OpenGL code by TheCherno.
    Used under Apache-2.0 License.
    https://github.com/TheCherno/OpenGL.git

[3] Procedural-Landmass-Generation. Procedural Landmass Generation in Unity by SebLague.
    Used under MIT License.
    https://github.com/SebLague/Procedural-Landmass-Generation.git

[4] scratchapixel. Procedural Generation of Virtual Worlds by Jean-Colas Prunier.
    https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/procedural-patterns-noise-part-1

###### TODO

1. Infinite terrain.
2. Use ZEngine 





