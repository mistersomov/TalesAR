cmake_minimum_required(VERSION 3.22.1)

project("talesar")

add_library(arcore SHARED IMPORTED)
set_target_properties(arcore PROPERTIES IMPORTED_LOCATION
    ${ARCORE_LIBPATH}/${ANDROID_ABI}/libarcore_sdk_c.so
    INTERFACE_INCLUDE_DIRECTORIES ${ARCORE_INCLUDE}
)
find_package(game-activity REQUIRED CONFIG)
set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u Java_com_google_androidgamesdk_GameActivity_initializeNativeCode")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR=1)

add_library(${PROJECT_NAME} SHARED
        src/main/cpp/main.cpp
        src/main/cpp/TalesArApp.cpp
        src/main/cpp/engine/TalesArEngine.cpp
        src/main/cpp/ar/TalesArSession.cpp
        src/main/cpp/facade/JniFacade.cpp
        src/main/cpp/exception/Exception.cpp
        src/main/cpp/utils/NativeWindowWrapper.hpp
)
target_include_directories(${PROJECT_NAME} PRIVATE src/main/cpp)
target_link_libraries(${PROJECT_NAME}
        arcore
        game-activity::game-activity_static
        vulkan
        android
        log
)