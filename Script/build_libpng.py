import os.path
import aceutils

aceutils.cdToScript()
aceutils.cd(r'../')

aceutils.cd(r'Downloads')

aceutils.rmdir(r'zlib_bin')
aceutils.rmdir(r'libpng_bin')

aceutils.editCmakeForACE(r'zlib/CMakeLists.txt')
aceutils.editCmakeForACE(r'libpng/CMakeLists.txt')

aceutils.mkdir(r'zlib_bin')
aceutils.mkdir(r'libpng_bin')

if aceutils.isWin():
    print('already installed.')
else:
    aceutils.cd(r'zlib_bin')
    if aceutils.isMac():
        aceutils.call(r'cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../dev "-DCMAKE_OSX_ARCHITECTURES=x86_64' +
                      (';i386' if aceutils.Isi386() else '') + r'" ../zlib/')
    else:
        aceutils.call(
            r'cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../dev ../zlib/')
    aceutils.call(r'make install')
    aceutils.cd(r'../')

    aceutils.cd(r'libpng_bin')
    if aceutils.isMac():
        aceutils.copy(r'../libpng/scripts/pnglibconf.h.prebuilt',
                      r'../libpng/pnglibconf.h')
        aceutils.call(
            r'cmake -G "Unix Makefiles" -D PNG_STATIC:BOOL=ON -D CMAKE_INSTALL_PREFIX=../dev "-DCMAKE_OSX_ARCHITECTURES=x86_64' + (';i386' if aceutils.Isi386() else '') + r'" ../libpng/')
    else:
        aceutils.call(
            r'cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../dev ../libpng/')
    aceutils.call(r'make install')
    aceutils.cd(r'../')

    aceutils.rm(r'dev/lib/libz.so')
    aceutils.rm(r'dev/lib/libz.so.1')
    aceutils.rm(r'dev/lib/libz.so.1.2.11')

    aceutils.rm(r'dev/lib/libpng.so')
    aceutils.rm(r'dev/lib/libpng16.so')
    aceutils.rm(r'dev/lib/libpng16.so.16')
    aceutils.rm(r'dev/lib/libpng16.so.16.21.0')

    aceutils.rm(r'dev/lib/libz.dylib')
    aceutils.rm(r'dev/lib/libz.1.dylib')
    aceutils.rm(r'dev/lib/libz.1.2.11.dylib')

    aceutils.rm(r'dev/lib/libpng.dylib')
    aceutils.rm(r'dev/lib/libpng16.dylib')
    aceutils.rm(r'dev/lib/libpng16.16.dylib')
    aceutils.rm(r'dev/lib/libpng16.16.21.0.dylib')

    aceutils.copy(r'dev/lib/libpng16.a', r'../lib/')
    aceutils.copy(r'dev/lib/libz.a', r'../lib/')
