workspace "Hangar_Demo"
	architecture "ARM"

	configurations {
		"Debug"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["libgba"] = "../../../vendor/libgba/include"

--include "../../../vendor/libgba/lib"

project "Hangar_Demo"
	kind "Makefile"
	language "C"
	debugger "GDB"

	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	targetextension ".gba"

	files
	{
		"Hangar_Demo/include/**.h",
		"Hangar_Demo/build/**.h",
		"Hangar_Demo/source/**.c",
		"premake5.lua",
	}

	includedirs {
		"Hangar_Demo/include",
		"%{IncludeDirs.libgba}"
	}

	links "libgba"

	buildcommands {
		"cd %{prj.name} && make"
	}

	cleancommands {
		"cd %{prj.name} && make clean"
	}

