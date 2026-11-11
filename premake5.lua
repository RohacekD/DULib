workspace "DULib"
	architecture "x64"
	cppdialect "C++20"
	systemversion "latest"

	configurations{
		"Debug",
		"Release"
	}

	filter "configurations:Debug"
		symbols "On"
		editandcontinue "Off"
		optimize "Off"
	filter "configurations:Release"
		symbols "On"
		optimize "Full"
	filter {}

	include "include"
	include "tests"