#pragma once

#include "AudioBuffer.h"
#include <SDL2/SDL.h>
#include <atomic>
#include <vector>

class AudioPlayer {
public:
    AudioPlayer();
    ~AudioPlayer();
    
    // Audio device management
    bool initialize(int sampleRate = 44100, int channels = 2);
    void shutdown();
    
    // Playback control
    void play(const AudioBuffer& buffer);
    void pause();
    void resume();
    void stop();
    
    // Playback state
    bool isPlaying() const { return playing_; }
    bool isPaused() const { return paused_; }
    float getPlaybackPosition() const; // 0.0 to 1.0
    float getDuration() const { return duration_; }
    
    // Volume control
    void setVolume(float volume); // 0.0 to 1.0
    float getVolume() const { return volume_; }
    
    // Audio buffer management
    void setAudioBuffer(const AudioBuffer& buffer);
    const AudioBuffer* getAudioBuffer() const { return audioBuffer_; }

private:
    static void audioCallback(void* userdata, Uint8* stream, int len);
    void fillAudioBuffer(Uint8* stream, int len);
    
    SDL_AudioDeviceID deviceId_;
    const AudioBuffer* audioBuffer_;
    
    std::atomic<bool> playing_;
    std::atomic<bool> paused_;
    std::atomic<size_t> currentFrame_;
    
    float volume_;
    float duration_;
    
    // Audio format
    int sampleRate_;
    int channels_;
    SDL_AudioFormat format_;
}; 