# GEngine 

This is a work-in-progress OpenGL platform I use to build graphics applications. 

## Usage

OpenGL Version: **4.6**

C++ Version: **C++17**

Currently only supports Windows, Visual Studio 2019. 

1. Clone the repository: `git clone --recursive XXX`
2. Run `Scripts/Win-Premake.bat` - this will create `ProTerrain.sln` for procedural terrain and make sure everything is setup correctly
3. Open `ProTerrain.sln` and build `Debug` `x64` - ProTerrain should be the startup project so really you can just hit `F5` to build and debug the startup project

Open `SDF.sln` for SDF.

Open `L-System`for L-System.

## Notes

- A **much** more elegant way to set buffer objects: NO MORE manually setting offsets and strides! 
```c++
Ref<VertexArray> vao;
Ref<VertexBuffer> vbo;
Ref<IndexBuffer> ebo;

vao = VertexArray::Create();

// VBO Setup
float vertices[] = 
{ 
    // vertex attributes data 
};
vbo = VertexBuffer::Create(sizeof(vertices), vertices);
vbo->SetLayout({
    { ShaderDataType::Float3, "a_Position"},
    { ShaderDataType::Float4, "a_Color"}
    // maybe more
});
vao->AddVertexBuffer(vbo);

// EBO Setup
uint32_t indices[] = 
{ 
    // indices data
};
ebo = IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t),indices);
vao->SetIndexBuffer(ebo);
```

Ref: macro for shared pointers

- Creating static vbo: `VertexBuffer::Create(two arguments)`. dynamic vbo: `VertexBuffer::Create(single argument)`


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


# ProTerrain

**Signed** perlin noise


