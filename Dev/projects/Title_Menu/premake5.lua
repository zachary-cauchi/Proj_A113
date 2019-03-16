workspace "Title_Menu"
	architecture "ARM"

	configurations {
		"Debug"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["libgba"] = "../../../vendor/libgba/include"

--include "../../../vendor/libgba/lib"

project "Title_Menu"
	kind "Makefile"
	language "C"
	debugger "GDB"

	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	targetextension ".gba"

	files
	{
		"Title_Menu/include/**.h",
		"Title_Menu/build/**.h",
		"Title_Menu/source/**.c",
		"premake5.lua",
	}

	includedirs {
		"Title_Menu/include",
		"%{IncludeDirs.libgba}"
	}

	links "libgba"

	buildcommands {
		"cd %{prj.name} && make"
	}

	cleancommands {
		"cd %{prj.name} && make clean"
	}

