#include "glpch.h"
#include "Buffer.h"
#include "OpenGLVertexBuffer.h"

namespace GLCore::Utils
{

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, float* vertices)
    {
        return CreateRef<OpenGLVertexBuffer>(size, vertices);
    }

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        return CreateRef<OpenGLVertexBuffer>(size);
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t count, uint32_t* indices)
    {
        return CreateRef<OpenGLIndexBuffer>(count, indices);
    }

}
