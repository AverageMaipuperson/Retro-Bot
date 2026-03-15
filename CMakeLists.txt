cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ANDROID_ABI armeabi-v7a)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(violet)

add_library(violet SHARED 
./src/epic.cpp 
./src/CoderLayer.cpp
./src/FPSCounter.cpp
./src/VCustomEncrypt.cpp
./src/NoclipOptionsLayer.cpp
./src/SpeedhackOptionsLayer.cpp
./vendor/cpp-base64-2.rc.08/base64.cpp
./src/KittyMemory/KittyMemory.cpp
./src/KittyMemory/MemoryPatch.cpp
./src/KittyMemory/KittyUtils.cpp
./src/KittyMemory/MemoryBackup.cpp
./src/HidePauseLayer.cpp
./src/CustomSongLayer.cpp
./src/SongUploaderLayer.cpp
./src/CustomLabelOptionsLayer.cpp
./src/minibpm/src/MiniBpm.cpp
./src/NoclipTintOptionsLayer.cpp
./src/UploadSongLayer.cpp
./src/SongFilterLayer.cpp
./src/CCDropdown.cpp
./src/PlaytestLayer.cpp
)

set(VENDOR_DIR ./vendor)

target_include_directories(violet PUBLIC vendor/cocos)

target_include_directories(violet PUBLIC vendor/robtop)

target_include_directories(violet PUBLIC vendor/KittyMemory)

target_include_directories(violet PUBLIC vendor/cocos/cocos2dx)

target_include_directories(violet PUBLIC vendor/cocos/cocos2dx/include)

target_include_directories(violet PUBLIC vendor/cocos/cocos2dx/platform)

target_include_directories(violet PUBLIC vendor/cocos/cocos2dx/platform/android)

target_include_directories(violet PUBLIC vendor/cocos/cocos2dx/platform/win32)

target_include_directories(violet PUBLIC vendor/cocos/cocos2dx/kazmath/include)

target_include_directories(violet PUBLIC vendor/jdk-25.0.1/include)

target_include_directories(violet PUBLIC vendor/jdk-25.0.1/include/linux)

target_include_directories(violet PUBLIC vendor/cocos/CocosDenshion/include)

target_include_directories(violet PUBLIC fmod/fmodstudioapi20311android/api/core/inc)

set(DCMAKE_BUILD_TYPE Debug)
set(DOBBY_DEBUG OFF CACHE INTERNAL "" FORCE)
set(DOBBY_GENERATE_SHARED OFF CACHE INTERNAL "" FORCE)
add_definitions(-DLOGGING_DISABLE -DNDEBUG)
add_subdirectory(vendor/Dobby)

add_library(fmod SHARED IMPORTED)
set_target_properties(fmod PROPERTIES IMPORTED_LOCATION
    "${CMAKE_CURRENT_SOURCE_DIR}/fmod/fmodstudioapi20311android/api/core/lib/armeabi-v7a/libfmodL.so")


target_include_directories(violet PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_library(gles2-lib GLESv2)

target_link_libraries(violet PRIVATE dobby dl log)
target_link_libraries(violet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libgame.so log android fmod ${gles2-lib})
target_compile_options(violet PUBLIC -fdebug-prefix-map=${CMAKE_SOURCE_DIR}=.)
set(ANDROID_STL "c++_shared")