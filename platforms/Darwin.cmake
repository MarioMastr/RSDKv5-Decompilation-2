project(RetroEngine)

# the problem with using homebrew as a source for all your precompiled libraries is that
# they don't allow for universal binaries: they only come in either arm64 or x86_64.
# if you want universal binaries, compile both arm64 and x86_64 separately and use
# the command 'lipo' to combine them together. alternatively, just compile for
# whichever architecture you use
set(CMAKE_OSX_ARCHITECTURES "arm64")

set(MACOSX_BUNDLE_ICON_FILE ${RETRO_NAME}.icns)
set(RETRO_ICON ${CMAKE_CURRENT_SOURCE_DIR}/${RETRO_NAME}/${RETRO_NAME}.icns)
set_source_files_properties(${RETRO_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

add_executable(RetroEngine MACOSX_BUNDLE ${RETRO_ICON} ${RETRO_FILES} dependencies/mac/cocoaHelpers.mm)

set(RETRO_SUBSYSTEM "SDL2" CACHE STRING "The subsystem to use")
option(USE_MINIAUDIO "Whether or not to use MiniAudio." OFF)

if(USE_MINIAUDIO)
    target_compile_definitions(RetroEngine PRIVATE RETRO_AUDIODEVICE_MINI=1)
endif()

find_package(Ogg CONFIG)

if(NOT ${Ogg_FOUND})
    message(NOTICE "libogg not found, please install libogg from homebrew")
else()
    message("found libogg")
    target_include_directories(RetroEngine PUBLIC ${Ogg_INCLUDE_DIR})
    target_link_libraries(RetroEngine "${Ogg_INCLUDE_DIR}/../lib/libogg.a")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(THEORA REQUIRED theora)

if (${THEORA_FOUND})
    message("found theora")
    target_include_directories(RetroEngine PUBLIC ${THEORA_INCLUDE_DIRS})
    target_link_directories(RetroEngine PUBLIC ${THEORA_LIBRARY_DIRS})
    target_link_libraries(RetroEngine libtheora.a)
else()
    message(NOTICE "theora not found, please install theora from homebrew")
endif()

if(RETRO_SUBSYSTEM STREQUAL "OGL")
    target_compile_definitions(RetroEngine PRIVATE RETRO_AUDIODEVICE_MINI=1)
    pkg_check_modules(GLFW REQUIRED glfw3)

    if(NOT GLFW_FOUND)
        message("could not find glfw, please install glfw from homebrew")
    else()
        message("found GLFW")
        target_include_directories(RetroEngine PUBLIC ${GLFW_INCLUDE_DIR})
        target_link_directories(RetroEngine PUBLIC ${GLFW_LIBRARY_DIRS})
    endif()

    find_package(GLEW CONFIG)

    if(NOT GLEW_FOUND)
        message(NOTICE "could not find glew, please install glew form homebrew")

    else()
        message("found GLEW")
        add_library(glew ALIAS GLEW::glew_s)
    endif()

    target_link_libraries(RetroEngine
        glew
        libglfw3.a
        "-framework AppKit"
        "-framework IOKit"
    )
elseif(RETRO_SUBSYSTEM STREQUAL "SDL2")
    message("found SDL2")
    find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(RetroEngine SDL2::SDL2-static)
elseif(RETRO_SUBSYSTEM STREQUAL "SDL3")
    message("found SDL3")
    find_package(SDL3 CONFIG REQUIRED)
    target_link_libraries(RetroEngine SDL3::SDL3-static)
else()
    message(FATAL_ERROR "RETRO_SUBSYSTEM must be OGL, SDL2, or SDL3 if available")
endif()

message(NOTICE "configuring for the " ${RETRO_SUBSYSTEM} " subsystem")

target_include_directories(RetroEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/mac)
