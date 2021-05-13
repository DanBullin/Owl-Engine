project "box2d"
    kind "StaticLib"
    language "C++"
    staticruntime "off"
	systemversion "latest"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/" .. outputdir .. "/%{prj.name}")

	files
	{
        "include/**.h",
		"src/**.h",
		"src/**.cpp"
    }
	
	includedirs
	{
		"include",
		"src",
	} 
		
	 filter { "system:windows", "configurations:Debug" }
		
		defines "DEBUG"
		runtime "Debug"
		symbols "On"
		
    filter { "system:windows", "configurations:Release" }
		defines "NDEBUG"
		runtime "Release"
		optimize "On"

