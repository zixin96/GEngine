#include "glpch.h"
#include "Texture.h"
#include "OpenGLTexture2D.h"

namespace GLCore::Utils
{
    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        return CreateRef<OpenGLTexture2D>(path);
    }

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        return CreateRef<OpenGLTexture2D>(width, height);
    }
}
