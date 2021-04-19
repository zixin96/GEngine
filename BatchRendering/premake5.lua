project "BatchRendering"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"../GraphicsCore/vendor/spdlog/include",
		"../GraphicsCore/src",
		"../GraphicsCore/vendor",
		"../GraphicsCore/%{IncludeDir.glm}",
		"../GraphicsCore/%{IncludeDir.Glad}",
		"../GraphicsCore/%{IncludeDir.ImGui}"
	}

	links
	{
		"GraphicsCore"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GLCORE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "GLCORE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GLCORE_RELEASE"
		runtime "Release"
        optimize "on"
