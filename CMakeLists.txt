cmake_minimum_required(VERSION 3.16)
project(MiniAudioEditorSuite VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(SDL2 REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})

# Source files
set(SOURCES
    src/main.cpp
    src/audio/AudioBuffer.cpp
    src/audio/GainEffect.cpp
    src/ui/Window.cpp
    src/ui/WaveformView.cpp
    src/ui/Application.cpp
)

# Header files
set(HEADERS
    include/audio/AudioBuffer.h
    include/audio/AudioEffect.h
    include/audio/GainEffect.h
    include/ui/Window.h
    include/ui/WaveformView.h
    include/ui/Application.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# Include directories for headers
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Enable testing
enable_testing()
add_subdirectory(tests) 