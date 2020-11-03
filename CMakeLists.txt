cmake_minimum_required(VERSION 2.8)
project(AltseedRHI CXX)

option(BUILD_TEST "Build test" OFF)

if (MSVC)
	#
else()
	set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC" ${CMAKE_CXX_FLAGS})
endif()

# MSVC Flag
if(MSVC)
  if(POLICY CMP0091)

    cmake_policy(GET CMP0091 CMP0091_STATE)

    if(CMP0091_STATE EQUAL NEW)

      if(USE_MSVC_RUNTIME_LIBRARY_DLL)
        cmake_policy(SET CMP0091 NEW)
        set(CMAKE_MSVC_RUNTIME_LIBRARY
            "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
      else()
        cmake_policy(SET CMP0091 NEW)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
      endif()

    else()

      if(NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
        foreach(flag
                CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
                CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)

          if(${flag} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
          endif()
          if(${flag} MATCHES "/MDd")
            string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
          endif()
        endforeach()
      endif()
    endif()

  else()

    if(NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
      foreach(flag
              CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
              CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)

        if(${flag} MATCHES "/MD")
          string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
        endif()
        if(${flag} MATCHES "/MDd")
          string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
        endif()
      endforeach()
    endif()

  endif()
endif()

add_subdirectory(src)

if (BUILD_TEST)
add_subdirectory(src_test)
endif()

