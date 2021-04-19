workspace "ProTerrain"
	architecture "x64"
	startproject "ProTerrain"

	configurations
	{
		"Debug",
		"Release"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to GraphicsCore
IncludeDir = {}
IncludeDir["GLFW"] = "vendor/GLFW/include"
IncludeDir["Glad"] = "vendor/Glad/include"
IncludeDir["ImGui"] = "vendor/imgui"
IncludeDir["glm"] = "vendor/glm"
IncludeDir["stb_image"] = "vendor/stb_image"

-- Projects
group "Dependencies"
	include "GraphicsCore/vendor/GLFW"
	include "GraphicsCore/vendor/Glad"
	include "GraphicsCore/vendor/imgui"
group ""

include "GraphicsCore"
include "ProTerrain"


workspace "BatchRendering"
    startproject "BatchRendering"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to GraphicsCore
IncludeDir = {}
IncludeDir["GLFW"] = "vendor/GLFW/include"
IncludeDir["Glad"] = "vendor/Glad/include"
IncludeDir["ImGui"] = "vendor/imgui"
IncludeDir["glm"] = "vendor/glm"
IncludeDir["stb_image"] = "vendor/stb_image"

-- Projects
group "Dependencies"
    includeexternal "GraphicsCore/vendor/GLFW"
    includeexternal "GraphicsCore/vendor/Glad"
    includeexternal "GraphicsCore/vendor/imgui"
group ""

includeexternal "GraphicsCore"
include "BatchRendering"