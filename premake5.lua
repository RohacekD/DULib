IncludeDir = "%{wks.location}/include"

workspace "DULib"
	architecture "x64"
	cppdialect "C++17"
	systemversion "latest"

	configurations{
		"Debug",
		"Release"
	}


	project "DULib"
		kind "StaticLib"

		files 
		{
				IncludeDir.."/**.h",
				IncludeDir.."/**.cpp",
				"%{wks.location}/util/DULib.natvis",
		}
		usage "PUBLIC"
			includedirs
			{
				IncludeDir,
			}
	include "tests"