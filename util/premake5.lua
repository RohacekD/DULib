project "DULib"
	kind "StaticLib"
	files 
	{
		"/**.h",
		"/**.cpp",
		"%{wks.location}/util/DULib.natvis",
	}
	usage "PUBLIC"
		includedirs
		{
			".",
		}