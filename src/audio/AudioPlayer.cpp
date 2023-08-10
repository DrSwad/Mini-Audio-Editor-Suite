#include "audio/AudioPlayer.h"
#include <iostream>
#include <algorithm>
#include <cmath>

AudioPlayer::AudioPlayer()
  : deviceId_(0), audioBuffer_(nullptr), playing_(false), paused_(false),
  currentFrame_(0), volume_(1.0f), duration_(0.0f),
  sampleRate_(44100), channels_(2), format_(AUDIO_F32) {}

AudioPlayer::~AudioPlayer() {
  shutdown();
}

bool AudioPlayer::initialize(int sampleRate, int channels) {
  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    std::cerr << "SDL audio could not initialize: " << SDL_GetError() << std::endl;
    return false;
  }

  sampleRate_ = sampleRate;
  channels_ = channels;

  SDL_AudioSpec desired, obtained;
  SDL_zero(desired);
  desired.freq = sampleRate_;
  desired.format = format_;
  desired.channels = channels_;
  desired.samples = 1024;   // Buffer size
  desired.callback = audioCallback;
  desired.userdata = this;

  deviceId_ = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, 0);

  if (deviceId_ == 0) {
    std::cerr << "Failed to open audio device: " << SDL_GetError() << std::endl;
    return false;
  }

  std::cout << "Audio device initialized: " << sampleRate_ << "Hz, "
            << channels_ << " channels" << std::endl;
  return true;
}

void AudioPlayer::shutdown() {
  if (deviceId_ != 0) {
    stop();
    SDL_CloseAudioDevice(deviceId_);
    deviceId_ = 0;
  }
}

void AudioPlayer::play(const AudioBuffer& buffer) {
  setAudioBuffer(buffer);

  if (deviceId_ == 0) {
    std::cerr << "Audio device not initialized" << std::endl;
    return;
  }

  currentFrame_ = 0;
  playing_ = true;
  paused_ = false;

  SDL_PauseAudioDevice(deviceId_, 0);
  std::cout << "Started audio playback" << std::endl;
}

void AudioPlayer::pause() {
  if (playing_ && !paused_) {
    paused_ = true;
    SDL_PauseAudioDevice(deviceId_, 1);
    std::cout << "Audio paused" << std::endl;
  }
}

void AudioPlayer::resume() {
  if (playing_ && paused_) {
    paused_ = false;
    SDL_PauseAudioDevice(deviceId_, 0);
    std::cout << "Audio resumed" << std::endl;
  }
}

void AudioPlayer::stop() {
  if (playing_) {
    playing_ = false;
    paused_ = false;
    currentFrame_ = 0;
    SDL_PauseAudioDevice(deviceId_, 1);
    std::cout << "Audio stopped" << std::endl;
  }
}

float AudioPlayer::getPlaybackPosition() const {
  if (!audioBuffer_ || audioBuffer_->getFrameCount() == 0) {
    return 0.0f;
  }

  return static_cast<float>(currentFrame_) / audioBuffer_->getFrameCount();
}

void AudioPlayer::setVolume(float volume) {
  volume_ = std::max(0.0f, std::min(1.0f, volume));
}

void AudioPlayer::setAudioBuffer(const AudioBuffer& buffer) {
  audioBuffer_ = &buffer;
  duration_ = static_cast<float>(buffer.getFrameCount()) / buffer.getSampleRate();
}

void AudioPlayer::audioCallback(void* userdata, Uint8* stream, int len) {
  AudioPlayer* player = static_cast<AudioPlayer*>(userdata);

  player->fillAudioBuffer(stream, len);
}

void AudioPlayer::fillAudioBuffer(Uint8* stream, int len) {
  if (!audioBuffer_ || !playing_ || paused_) {
    SDL_memset(stream, 0, len);
    return;
  }

  size_t frameCount = audioBuffer_->getFrameCount();
  size_t channelCount = audioBuffer_->getChannelCount();
  int bytesPerFrame = channelCount * sizeof(float);
  int framesToWrite = len / bytesPerFrame;

  float* output = reinterpret_cast<float*>(stream);

  for (int i = 0; i < framesToWrite; ++i) {
    if (currentFrame_ >= frameCount) {
      // End of audio reached
      playing_ = false;
      currentFrame_ = 0;
      break;
    }

    for (size_t channel = 0; channel < channelCount; ++channel) {
      float sample = audioBuffer_->getSample(currentFrame_, channel);
      output[i * channelCount + channel] = sample * volume_;
    }

    currentFrame_++;
  }

  // Fill remaining buffer with silence
  for (int i = framesToWrite; i < len / (int) sizeof(float); ++i) {
    output[i] = 0.0f;
  }
}