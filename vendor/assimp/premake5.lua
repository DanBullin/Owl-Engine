project "assimp"
    kind "StaticLib"
    language "C++"
    staticruntime "off"
	systemversion "latest"
	
	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../build/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
		"include",
		"code",
		"contrib/IrrXML/",
		"contrib/zlib/",
		".",
		"contrib/rapidjson/include",
		"contrib/unzip",
		"contrib/Open3DGC",
		"contrib/clipper/",
		"contrib/openddlparser/include",
		"contrib/pugixml/src",
		"contrib/utf8cpp/source/",
		"contrib/zip/src/"
	}
	files
	{
		"code/**/*.cpp",
		"contrib/clipper/*.cpp",
		"contrib/Open3DGC/*.cpp",
		"contrib/pugixml/src/*.cpp",
		"contrib/unzip/*.c",
		"contrib/zip/src/*.c",
		"contrib/zlib/*.c"
    }
	
	links
	{
		--"googletest",
		--"IrrXML",
		--"zlib",
	}
		
	filter "system:windows"
		defines 
		{ 
			"WIN32",
			"_WINDOWS",
			"WIN32_LEAN_AND_MEAN",
			"UNICODE",
			"_UNICODE",
			"_SCL_SECURE_NO_WARNINGS",
			"_CRT_SECURE_NO_WARNINGS",
			"ASSIMP_BUILD_NO_IFC_IMPORTER",
			"ASSIMP_BUILD_NO_C4D_IMPORTER",
			"ASSIMP_BUILD_NO_GLTF_IMPORTER",
			"ASSIMP_BUILD_NO_GLTF1_IMPORTER",
			"ASSIMP_BUILD_NO_GLTF2_IMPORTER",
			"ASSIMP_BUILD_NO_OPENGEX_IMPORTER"
		}
		
		
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
		optimize "On"