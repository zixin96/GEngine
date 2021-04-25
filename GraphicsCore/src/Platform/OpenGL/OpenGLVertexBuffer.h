#pragma once
#include "GLCore/Renderer/VertexBuffer.h"
#include "GLCore/Core/Buffer.h"

namespace GLCore
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(uint32_t size, void* data, VertexBufferUsage usage = VertexBufferUsage::Static);
        OpenGLVertexBuffer(uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);

        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
        virtual const BufferLayout& GetLayout() const override { return m_Layout; }

        virtual void SetData(const void* data, uint32_t size) override;

    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
        VertexBufferUsage m_Usage;
    };

}


