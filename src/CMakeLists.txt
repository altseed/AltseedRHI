cmake_minimum_required(VERSION 2.8)
project(AltseedRHI CXX)

FILE(GLOB hs *.h)
FILE(GLOB cpps *.cpp)

if (MSVC)
	FILE(GLOB hs_dx11 DX11/*.h)
	FILE(GLOB cpps_dx11 DX11/*.cpp)
	FILE(GLOB hs_gl GL/*.h)
	FILE(GLOB cpps_gl GL/*.cpp)
	FILE(GLOB hs_win Win/*.h)
	FILE(GLOB cpps_win Win/*.cpp)
	FILE(GLOB hs_th 3rdParty/*/*.h)
	FILE(GLOB cpps_th 3rdParty/*/*.cpp)

	list(APPEND hs ${hs_dx11})
	list(APPEND cpps ${cpps_dx11})
	list(APPEND hs ${hs_gl})
	list(APPEND cpps ${cpps_gl})
	list(APPEND hs ${hs_win})
	list(APPEND cpps ${cpps_win})
	list(APPEND hs ${hs_th})
	list(APPEND cpps ${cpps_th})

else()
	FILE(GLOB hs_gl GL/*.h)
	FILE(GLOB cpps_gl GL/*.cpp)
	FILE(GLOB hs_nix Nix/*.h)
	FILE(GLOB cpps_nix Nix/*.cpp)
	FILE(GLOB hs_th 3rdParty/nv_dds/*.h)
	FILE(GLOB cpps_th 3rdParty/nv_dds/*.cpp)

	list(APPEND hs ${hs_gl})
	list(APPEND cpps ${cpps_gl})
	list(APPEND hs ${hs_nix})
	list(APPEND cpps ${cpps_nix})
	list(APPEND hs ${hs_th})
	list(APPEND cpps ${cpps_th})

endif()

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

add_library(AltseedRHI ${cpps} ${hs})

include_directories(../include/ ../Downloads/dev/include/)

set_target_properties(AltseedRHI PROPERTIES LINKER_LANGUAGE CXX)
