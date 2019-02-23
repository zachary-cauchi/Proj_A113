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
	debugger "GDB"

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

	cleancommands {
		"cd %{prj.name} && make clean"
	}

