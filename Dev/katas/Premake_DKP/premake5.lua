workspace "Premake_DKP"
	architecture "ARM"

	configurations {
		"Debug"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["libgba"] = "../../../vendor/libgba/include"

--include "../../../vendor/libgba/lib"

project "Premake_DKP"
	kind "Makefile"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	targetextension ".gba"


	files
	{
		"Premake_DKP/include/**.h",
		"Premake_DKP/source/**.c",
		"premake5.lua",
	}

	includedirs {
		"Premake_DKP/include",
		"%{IncludeDirs.libgba}"
	}

	links "libgba"

	buildcommands {
		"cd %{prj.name} && make"
	}

	postbuildcommands
	{
		("{COPY} %{prj.name}/%{prj.name}.gba bin/" .. outputdir .. "/%{prj.name}")
	}

	cleancommands {
		"cd %{prj.name} && make clean"
	}

