outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Tests"
    kind "ConsoleApp"
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
		"../"..IncludeDir,
		"../vendor/Catch2/single_include/catch2",
	}

	files 
	{
		"**.h",
		"**.cpp"
	}

	filter "system:linux"
		links {"gcov"}

		buildoptions {"-fprofile-arcs", "-ftest-coverage", "--coverage"}
		linkoptions { "--coverage" }