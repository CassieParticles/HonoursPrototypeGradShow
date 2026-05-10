cmake_minimum_required(VERSION 3.29)
project(HonoursPrototype)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(HonoursPrototype main.cpp
        Project/Application.cpp
        Project/Application.h
        Project/GameObjects/TestObject.cpp
        Project/GameObjects/TestObject.h
        Project/MarchingSquares/Object/MarchingSquaresObject.cpp
        Project/MarchingSquares/Object/MarchingSquaresObject.h
        Project/MarchingSquares/VoxelGrid.cpp
        Project/MarchingSquares/VoxelGrid.h
        Project/MarchingSquares/Object/MarchingSquaresRenderable.cpp
        Project/MarchingSquares/Object/MarchingSquaresRenderable.h
        Project/GameObjects/StaticPhysicsFloor.cpp
        Project/GameObjects/StaticPhysicsFloor.h
        Project/GameObjects/DynamicPhysicsBox.cpp
        Project/GameObjects/DynamicPhysicsBox.h
        Project/MarchingSquares/Object/MarchingSquaresPhysics.cpp
        Project/MarchingSquares/Object/MarchingSquaresPhysics.h
        Project/MarchingSquares/Triangle.h
        Project/MarchingSquares/TriangleBuilder.cpp
        Project/MarchingSquares/TriangleBuilder.h
        Project/GameObjects/CameraController.cpp
        Project/GameObjects/CameraController.h
        Project/MarchingSquares/MarchingSquaresManager.cpp
        Project/MarchingSquares/MarchingSquaresManager.h
        Project/MarchingSquares/Drawable/MSDrawableObject.cpp
        Project/MarchingSquares/Drawable/MSDrawableObject.h
        Project/CmdVarPass.cpp
        Project/CmdVarPass.h
        Project/MarchingSquares/Drawable/MSDrawableRenderable.cpp
        Project/MarchingSquares/Drawable/MSDrawableRenderable.h)

add_subdirectory(Engine)
target_include_directories(HonoursPrototype PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Engine)
target_link_libraries(HonoursPrototype PUBLIC HonoursFramework)


target_link_libraries(HonoursPrototype PUBLIC -static)
