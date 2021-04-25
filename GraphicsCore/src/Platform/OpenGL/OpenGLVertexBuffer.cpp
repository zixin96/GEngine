#include "glpch.h"
#include "OpenGLVertexBuffer.h"
#include <glad/glad.h>

namespace GLCore
{
    static GLenum OpenGLUsage(VertexBufferUsage usage)
    {
        switch (usage)
        {
        case VertexBufferUsage::Static:    return GL_STATIC_DRAW;
        case VertexBufferUsage::Dynamic:   return GL_DYNAMIC_DRAW;
        }
        GLCORE_ASSERT(false, "Unknown vertex buffer usage");
        return 0;
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, void* data, VertexBufferUsage usage /*= VertexBufferUsage::Static*/)
        : m_Usage(usage)
    {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, data, OpenGLUsage(usage));
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, VertexBufferUsage usage /*= VertexBufferUsage::Dynamic*/)
    {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, OpenGLUsage(usage));
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
       

        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
       

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
       

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
}
