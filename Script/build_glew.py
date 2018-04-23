import aceutils

pathname = 'glew-1.13.0'

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):
	aceutils.rm(r'glew-1.13.0.zip')
	aceutils.rmdir(pathname)
	aceutils.rmdir(r"glew_bin")
	aceutils.rmdir(r"glew_bin_x64")

	#aceutils.wget(r'https://sourceforge.net/projects/glew/files/glew/1.13.0/glew-1.13.0.zip')
	aceutils.wget(r'http://sourceforge.net/projects/glew/files/glew/1.13.0/glew-1.13.0.zip')

	aceutils.unzip(r'glew-1.13.0.zip')
	aceutils.editCmakeForACE(pathname + r'/build/cmake/CMakeLists.txt')
	aceutils.mkdir(r"glew_bin")
	aceutils.mkdir(r"glew_bin_x64")

	with aceutils.CurrentDir('glew_bin'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake+r'-UGLEW_USE_STATIC_LIBS ../' + pathname + '/build/cmake')
			aceutils.call(aceutils.cmd_compile + r'ALL_BUILD.vcxproj /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'ALL_BUILD.vcxproj /p:configuration=Release')
		elif aceutils.isMac():
			aceutils.call(r'cmake -G "Unix Makefiles" -UGLEW_USE_STATIC_LIBS -D CMAKE_INSTALL_PREFIX:PATH=../dev "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../' + pathname + '/build/cmake')
			aceutils.call(r'make install')
		else:
			aceutils.call(r'cmake -G "Unix Makefiles" -UGLEW_USE_STATIC_LIBS -D CMAKE_INSTALL_PREFIX=../dev ../' + pathname + '/')
			aceutils.call(r'make install')

	with aceutils.CurrentDir('glew_bin_x64'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake_x64+r'-UGLEW_USE_STATIC_LIBS ../' + pathname + '/build/cmake')
			aceutils.call(aceutils.cmd_compile + r'ALL_BUILD.vcxproj /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'ALL_BUILD.vcxproj /p:configuration=Release')

	if aceutils.isWin():
		aceutils.mkdir(r'../lib/')
		aceutils.mkdir(r'../lib/x86/')
		aceutils.mkdir(r'../lib/x86/Debug')
		aceutils.mkdir(r'../lib/x86/Release')

		aceutils.mkdir(r'../lib/x64/')
		aceutils.mkdir(r'../lib/x64/Debug')
		aceutils.mkdir(r'../lib/x64/Release')

		aceutils.mkdir(r'../include')
		aceutils.mkdir(r'../include/GL')

		aceutils.copy(pathname + r'/include/GL/glew.h', r'../include/GL/')
		aceutils.copy(pathname + r'/include/GL/glxew.h', r'../include/GL/')
		aceutils.copy(pathname + r'/include/GL/wglew.h', r'../include/GL/')

		aceutils.copy(r'glew_bin/lib/Debug/libglew32d.lib', r'../lib/x86/Debug/')
		aceutils.copy(r'glew_bin/lib/Release/libglew32.lib', r'../lib/x86/Release/')

		aceutils.copy(r'glew_bin_x64/lib/Debug/libglew32d.lib', r'../lib/x64/Debug/')
		aceutils.copy(r'glew_bin_x64/lib/Release/libglew32.lib', r'../lib/x64/Release/')
	else:
		aceutils.copy(r'dev/lib/libGLEW.a', r'../lib/')
