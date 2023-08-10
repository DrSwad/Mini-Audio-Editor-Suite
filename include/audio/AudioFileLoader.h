#pragma once

#include "AudioBuffer.h"
#include <string>

class AudioFileLoader {
public:
    AudioFileLoader();
    
    // Load audio file into buffer
    bool loadWavFile(const std::string& filename, AudioBuffer& buffer);
    
    // Check if file can be loaded
    bool canLoadFile(const std::string& filename) const;
    
    // Get file info without loading
    bool getFileInfo(const std::string& filename, int& sampleRate, int& channels, int& frameCount) const;

private:
    // WAV file structure helpers
    bool readWavHeader(const std::string& filename, int& sampleRate, int& channels, int& frameCount) const;
    bool readWavData(const std::string& filename, AudioBuffer& buffer) const;
}; 