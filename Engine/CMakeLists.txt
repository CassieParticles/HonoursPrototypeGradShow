cmake_minimum_required(VERSION 3.29)

add_library(HonoursFramework STATIC
        Core/BaseApplication.cpp
        Core/BaseApplication.h
        Core/InputHandler.cpp
        Core/InputHandler.h
        Rendering/Camera.cpp
        Rendering/Camera.h
        Core/Timer.cpp
        Core/Timer.h
        Rendering/BaseRenderable.cpp
        Rendering/BaseRenderable.h
        Rendering/Camera.cpp
        Rendering/Camera.h
        GameObjects/GameObject.cpp
        GameObjects/GameObject.h
        GameObjects/Transform.cpp
        GameObjects/Transform.h
        Rendering/ColourBoxRenderable.cpp
        Rendering/ColourBoxRenderable.h
        Physics/PhysicsWorld.cpp
        Physics/PhysicsWorld.h
        )

#SFML
include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

target_link_libraries(HonoursFramework PUBLIC SFML::Graphics)

FetchContent_Declare (
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG v0.13.1
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable (tracy)

target_link_libraries(HonoursFramework PUBLIC TracyClient)

FetchContent_Declare (
    Box2D
    GIT_REPOSITORY https://github.com/erincatto/box2d
    GIT_TAG v3.1.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable (Box2D)

target_link_libraries(HonoursFramework PUBLIC box2d::box2d)


target_include_directories(HonoursFramework PUBLIC libs/includes)