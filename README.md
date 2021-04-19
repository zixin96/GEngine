# GEngine 

This is a work-in-progress OpenGL platform I use to build graphics applications. 

## Usage

Currently only supports Windows. 

```
git clone --recursive 
```

Run `scripts/Win-Premake.bat` and open `ProTerrain.sln` in Visual Studio 2019 for Procedural Terrain.

Open `SDF.sln` for SDF.

Open `L-System`for L-System.

## Features

- A **much** more elegant way to set buffer objects: verbose OpenGL setup is abtracted away!

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
vao->SetIndexBuffer(m_QuadIB);
```



VBO: vertex buffer object

VAO: vertex array object

EBO: element buffer object/index buffer object

Ref: macro for shared pointers