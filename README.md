# Mini Audio Editor Suite

An audio editor built with C++ and SDL2 to apply a collection of effects to an audio with real-time processing, audio playback and waveform visualization support.

## Building the Project

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.16 or higher
- SDL2 development libraries

### Ubuntu/Debian Setup

```bash
sudo apt update
sudo apt install build-essential cmake libsdl2-dev
```

### Building

```bash
mkdir build
cd build
cmake ..
make
```

### Running

```bash
./MiniAudioEditorSuite
```

### Testing

Run the unit tests:
```bash
cd build
./tests/UnitTests
```

Run the main application:
```bash
cd build
./MiniAudioEditorSuite
``` 
