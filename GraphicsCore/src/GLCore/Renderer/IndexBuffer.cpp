#include "glpch.h"
#include "IndexBuffer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace GLCore
{
    Ref<IndexBuffer> IndexBuffer::Create(uint32_t count, uint32_t* indices)
    {
        return CreateRef<OpenGLIndexBuffer>(count, indices);
    }
}
