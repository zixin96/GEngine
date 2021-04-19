#include "glpch.h"
#include "VertexArray.h"
#include "OpenGLVertexArray.h"

namespace GLCore::Utils
{
    Ref<VertexArray> VertexArray::Create()
    {
        return CreateRef<OpenGLVertexArray>();
    }
}
