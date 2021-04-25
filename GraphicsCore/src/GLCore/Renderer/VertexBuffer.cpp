#include "glpch.h"
#include "VertexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace GLCore
{
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, float* vertices)
    {
        return CreateRef<OpenGLVertexBuffer>(size, vertices);
    }

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        return CreateRef<OpenGLVertexBuffer>(size);
    }

}
