project "ReactPhysics3d"
    kind "StaticLib"
    language "C++"
    staticruntime "off"
	systemversion "latest"
    
	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../build/" .. outputdir .. "/%{prj.name}")

	files
	{
        "include/**.h",
		"src/**.cpp"
    }
	
	includedirs
	{
		"include",
		"include/body",
		"include/collision",
		"include/constraint",
		"include/containers",
		"include/engine",
		"include/mathematics",
		"include/memory",
		"include/utils"
	} 
    
	filter "system:windows"
		defines 
		{ 
			"WIN32",
			"_WINDOWS"
		}
		
	 filter { "system:windows", "configurations:Debug" }
		
		defines "DEBUG"
		runtime "Debug"
		symbols "On"
		
    filter { "system:windows", "configurations:Release" }
		defines "NDEBUG"
		runtime "Release"
		optimize "On"

