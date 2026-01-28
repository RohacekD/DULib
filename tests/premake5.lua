outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Tests"
    kind "ConsoleApp"
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

	uses {"DULib"}

	includedirs
	{
		"../vendor/Catch2/single_include/catch2",
	}

	files 
	{
		"**.h",
		"**.cpp"
	}

--	filter "action:gmake"
--		links {"gcov"}
--
--		buildoptions {"-fprofile-arcs", "-ftest-coverage", "--coverage"}
--		linkoptions { "--coverage" }