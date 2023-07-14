# Test executable
add_executable(UnitTests
    test_main.cpp
    test_audio_buffer.cpp
    test_gain_effect.cpp
    ../src/audio/AudioBuffer.cpp
    ../src/audio/GainEffect.cpp
)

target_include_directories(UnitTests PRIVATE ../include)
target_link_libraries(UnitTests ${SDL2_LIBRARIES})

# Add test
add_test(NAME UnitTests COMMAND UnitTests) 