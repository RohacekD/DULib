IncludeDir = "include"

workspace "DULib"
	architecture "x64"
	cppdialect "C++17"
	systemversion "latest"

	configurations{
		"Debug",
		"Release"
	}

include "tests"

project "DULib"
	kind "StaticLib"

	includedirs
	{
		IncludeDir,
	}

	files 
	{
		IncludeDir.."/**.h",
		IncludeDir.."/**.cpp"
	}