workspace "Premake_DKP"
	architecture "ARM"

	configurations {
		"Debug"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["libgba"] = "../../../vendor/libgba/include"

project "OBJ_DKP"
	kind "Makefile"
	language "C"
	debugger "GDB"

	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	targetextension ".gba"

	files
	{
		"OBJ_DKP/include/**.h",
		"OBJ_DKP/source/**.c",
		"premake5.lua",
	}

	includedirs {
		"OBJ_DKP/include",
		"%{IncludeDirs.libgba}"
	}

	links "libgba"

	buildcommands {
		"cd %{prj.name} && make"
	}

	cleancommands {
		"cd %{prj.name} && make clean"
	}

