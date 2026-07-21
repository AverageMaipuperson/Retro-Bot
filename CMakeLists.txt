cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(rbot)

set(VERSIONS 10 11 12 13 14 15 16 17 18)

add_definitions(-DGAME_VERSION=${GAME_VERSION})
if (NOT GAME_VERSION IN_LIST VERSIONS)
    message(FATAL_ERROR "GAME_VERSION is not defined.")
endif()

if(GAME_VERSION LESS 15)
    set(ANDROID_ABI armeabi)
else()
    set(ANDROID_ABI armeabi-v7a)
endif()

# add_compile_options(-include "src/ndk.hpp")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.cc)
list(FILTER SOURCES EXCLUDE REGEX "src/features/")
add_library(rbot SHARED ${SOURCES})

math(EXPR VERSION_MAJOR "${GAME_VERSION} / 10")
math(EXPR VERSION_MINOR "${GAME_VERSION} % 10")
set(VERSION_DIR "${VERSION_MAJOR}.${VERSION_MINOR}")
target_include_directories(rbot PUBLIC src/version/${VERSION_DIR})

set(include_DIR ./include)

target_include_directories(rbot PUBLIC include/cocos)

target_include_directories(rbot PUBLIC include/robtop)

target_include_directories(rbot PUBLIC include/KittyMemory)


if(GAME_VERSION GREATER 16)
target_include_directories(rbot PUBLIC include/cocos/cocos2d-2.2.3/cocos2dx/include)

target_include_directories(rbot PUBLIC include/cocos/cocos2d-2.2.3/cocos2dx/platform)

target_include_directories(rbot PUBLIC include/cocos/cocos2d-2.2.3/cocos2dx/platform/android)

target_include_directories(rbot PUBLIC include/cocos/cocos2d-2.2.3/cocos2dx/platform/win32)

target_include_directories(rbot PUBLIC include/cocos/cocos2d-2.2.3/cocos2dx)

target_include_directories(rbot PUBLIC include/cocos/cocos2d-2.2.3/cocos2dx/kazmath/include)
else()

target_include_directories(rbot PUBLIC include/cocos/cocos2d-2.0.1/cocos2dx)

target_include_directories(rbot PUBLIC include/cocos/cocos2d-2.0.1/cocos2dx/include)

target_include_directories(rbot PUBLIC include/cocos/cocos2d-2.0.1/cocos2dx/platform)

target_include_directories(rbot PUBLIC include/cocos/cocos2d-2.0.1/cocos2dx/platform/android)

target_include_directories(rbot PUBLIC include/cocos/cocos2d-2.0.1/cocos2dx/platform/win32)

target_include_directories(rbot PUBLIC include/cocos/cocos2d-2.0.1/cocos2dx/kazmath/include)
endif()

target_include_directories(rbot PUBLIC include/jdk-25.0.1/include)

target_include_directories(rbot PUBLIC include/jdk-25.0.1/include/linux)

target_include_directories(rbot PUBLIC include/cocos/CocosDenshion/include)

target_include_directories(rbot PUBLIC include)

target_include_directories(rbot PUBLIC include/rbot)

target_include_directories(rbot PUBLIC include/version)

target_include_directories(rbot PUBLIC include/custom_nodes)

target_include_directories(rbot PUBLIC include/mod)

target_include_directories(rbot PUBLIC src)

target_include_directories(rbot PUBLIC include/rapidjson/include)

target_include_directories(rbot PUBLIC include/rapidjson/thirdparty)

target_include_directories(rbot PUBLIC include/rapidjson/include/rapidjson)

target_include_directories(rbot PUBLIC include/fmt/include)

set(DCMAKE_BUILD_TYPE Debug)
set(DOBBY_DEBUG OFF CACHE INTERNAL "" FORCE)
set(DOBBY_GENERATE_SHARED OFF CACHE INTERNAL "" FORCE)
add_definitions(-DLOGGING_DISABLE -DNDEBUG)
add_subdirectory(include/Dobby)

target_compile_definitions(rbot PRIVATE FMT_HEADER_ONLY=1)

find_library(gles2-lib GLESv2)

target_link_libraries(rbot PRIVATE dobby dl log)

if(GAME_VERSION GREATER 16)
set(LIBGAME_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/version/${VERSION_DIR}/libcocos2dcpp.so")
else()
set(LIBGAME_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/version/${VERSION_DIR}/libgame.so")
endif()
target_link_libraries(rbot PUBLIC "${LIBGAME_PATH}" log android ${gles2-lib})
target_compile_options(rbot PUBLIC -fdebug-prefix-map=${CMAKE_SOURCE_DIR}=.)
set(ANDROID_STL "c++_shared")