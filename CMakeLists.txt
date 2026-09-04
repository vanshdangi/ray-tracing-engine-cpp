cmake_minimum_required(VERSION 3.26)

project(ray_tracing_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------- Sources ----------------
set(SOURCES
    src/main.cpp
    
    src/rendering/camera.cpp
    src/rendering/renderer.cpp

    src/core/image.cpp

    src/world/scene.cpp
    src/world/hitRecord.cpp

    src/world/objects/sphere.cpp
    src/world/objects/cube.cpp
)

# ---------------- Executable ----------------
add_executable(ray_tracing_engine
    ${SOURCES}
)

# ---------------- Includes ----------------
target_include_directories(ray_tracing_engine
    PRIVATE
        include
)

# ---------------- SFML ----------------
find_package(SFML 3 REQUIRED COMPONENTS Graphics)

target_link_libraries(ray_tracing_engine
    PRIVATE
        SFML::Graphics
)

# ---------------- Tests ----------------
enable_testing()
add_subdirectory(tests)