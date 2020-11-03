cmake_minimum_required(VERSION 2.8)
project(AltseedRHI_Test CXX)

FILE(GLOB hs *.h)
FILE(GLOB cpps *.cpp)

foreach(FILE ${hs}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    # Group into "Source Files" and "Header Files"
    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "Files${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

foreach(FILE ${cpps}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    # Group into "Source Files" and "Header Files"
    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "Files${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

set(common_lib)
set(common_inc)

find_package(OpenGL)

if(MSVC)

else()
	find_package(OpenGL)
	list(APPEND common_inc ${OPENGL_INCLUDE_DIR})
	list(APPEND common_lib ${OPENGL_LIBRARY})

	if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		# used in GLFW
		find_library(COCOA_FRAMEWORK Cocoa)
		find_library(COCOA_FRAMEWORK Cocoa)
		find_library(IOKIT_FRAMEWORK IOKit)
		find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
		find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
		list(APPEND common_lib
			${COCOA_FRAMEWORK}
			${OPENGL_gl_LIBRARY}
			${OPENGL_glu_LIBRARY}
			${IOKIT_FRAMEWORK}
			${CORE_FOUNDATION_FRAMEWORK}
			${CORE_VIDEO_FRAMEWORK})
	endif()

	include_directories(../include/ ../Downloads/dev/include/)
	link_directories(${PROJECT_SOURCE_DIR}/../lib/)
endif()

add_executable(AltseedRHI_Test ${cpps} ${hs})

if(MSVC)

else()
	target_link_libraries(AltseedRHI_Test AltseedRHI AltseedPlatform glfw3 png16 z ${common_lib}) 
endif()

set_target_properties(AltseedRHI_Test PROPERTIES LINKER_LANGUAGE CXX)
