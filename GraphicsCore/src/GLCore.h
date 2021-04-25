#pragma once

// Main header file - include into application for complete access

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include "GLCore/Core/Application.h"


#include "GLCore/Renderer/Shader.h"
#include "GLCore/Renderer/VertexBuffer.h"
#include "GLCore/Renderer/IndexBuffer.h"
#include "GLCore/Renderer/VertexArray.h"
#include "GLCore/Renderer/Texture.h"
#include "GLCore/Renderer/OrthographicCamera.h"
#include "GLCore/Renderer/OrthographicCameraController.h"
#include "GLCore/Renderer/PerspectiveCamera.h"
#include "GLCore/Renderer/PolyMesh.h"

#include "Platform/OpenGL/OpenGLDebug.h"
