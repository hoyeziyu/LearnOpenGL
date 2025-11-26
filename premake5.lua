workspace "LearnOpenGL"
	architecture "x64"
	startproject "LearnOpenGL"

	configurations
	{
		"Debug",
		"Release",
	}

	-- 根据当前的配置动态生成,buildcfg(eg. Debug, Release), system(eg. Windows, Linux), architecture(eg. x86, x64)
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	-- 自定义变量，包含相对于根文件夹（解决方案目录）的目录
	IncludeDir = {}
	IncludeDir["GLFW"] = "LearnOpenGL/vendor/GLFW/include"
	IncludeDir["Glad"] = "LearnOpenGL/vendor/Glad/include"

	 -- group 是一个用于组织项目的逻辑分组的关键字
	group "Dependencies"
		include "LearnOpenGL/vendor/GLFW"
 		include "LearnOpenGL/vendor/Glad"
	group ""

project "LearnOpenGL"
	location "LearnOpenGL"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links
	{
		"Glad",  -- 确保这里链接 Glad 库
		"GLFW"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On" -- 是否使用静态运行时库
		systemversion "latest"

	filter "configurations:Debug"
 		defines "HZ_DEBUG"
		runtime "Debug"
 		symbols "on"
 
 	filter "configurations:Release"
 		defines "HZ_RELEASE"
		runtime "Release"
 		optimize "on"
