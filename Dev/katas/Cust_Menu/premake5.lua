workspace "Cust_Menu"
	architecture "ARM"

	configurations {
		"Debug"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["libgba"] = "../../../vendor/libgba/include"

--include "../../../vendor/libgba/lib"

project "Cust_Menu"
	kind "Makefile"
	language "C"
	debugger "GDB"

	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	targetextension ".gba"

	files
	{
		"Cust_Menu/include/**.h",
		"Cust_Menu/source/**.c",
		"premake5.lua",
	}

	includedirs {
		"Cust_Menu/include",
		"%{IncludeDirs.libgba}"
	}

	links "libgba"

	buildcommands {
		"cd %{prj.name} && make"
	}

	cleancommands {
		"cd %{prj.name} && make clean"
	}

