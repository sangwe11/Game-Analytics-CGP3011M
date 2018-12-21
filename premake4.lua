solution "GameAnalytics"
	configurations { "Debug", "Release" }

	configuration { "Debug" }
		targetdir "bin/Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration { "Release" }
		targetdir "bin/Release"
		defines { "NDEBUG" }
		flags { "Optimize" }

	configuration { "windows" }
		defines { "WINDOWS" }
	configuration { "linux" }
		defines { "LINUX" }
	configuration {}

	project "GameAnalytics"
		language "C++"
		kind "ConsoleApp"

		-- C++ on linux
		configuration { "linux" }
			buildoptions "-std=c++11"
		configuration {}

		files { "./src/**.h", "./src/**.cpp" }
		excludes { "./includes/**", "./libraries/**" }
		
		includedirs { "./includes/" }
		libdirs { "./libraries/" }
		
		configuration "windows"
			links { "SDL/SDL2", "SDL/SDL2main", "SDL/SDL2_image", "opengl32", "GL/glew32", "freetype2/freetype" }
		configuration "linux"
			links { "SDL2", "SDL2main", "SDL2_image", "GL", "GLEW", "freetype2" }
		configuration {}
		
		if os.get() == "windows" then
			os.copyfile("./libraries/GL/glew32.dll", "glew32.dll")
			os.copyfile("./libraries/SDL/SDL2.dll", "SDL2.dll")
			os.copyfile("./libraries/SDL/SDL2_image.dll", "SDL2_image.dll")
			os.copyfile("./libraries/SDL/libjpeg-9.dll", "libjpeg-9.dll")
			os.copyfile("./libraries/SDL/libpng16-16.dll", "libpng16-16.dll")
			os.copyfile("./libraries/SDL/libtiff-5.dll", "libtiff-5.dll")
			os.copyfile("./libraries/SDL/libwebp-4.dll", "libwebp-4.dll")
			os.copyfile("./libraries/freetype2/freetype6.dll", "freetype6.dll")
			os.copyfile("./libraries/freetype2/zlib1.dll", "zlib1.dll")
        end