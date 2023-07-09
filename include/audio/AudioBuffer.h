#pragma once

#include <vector>
#include <cstdint>
#include <memory>

class AudioBuffer {
public:
  AudioBuffer(size_t sampleRate = 44100, size_t channels = 2);
  AudioBuffer(const AudioBuffer& other);
  AudioBuffer& operator=(const AudioBuffer& other);

  // Buffer management
  void resize(size_t frames);
  void clear();
  size_t getFrameCount() const;
  size_t getSampleRate() const;
  size_t getChannelCount() const;

  // Audio data access
  float getSample(size_t frame, size_t channel) const;
  void setSample(size_t frame, size_t channel, float value);

  // Audio operations
  void normalize();
  void applyGain(float gain);
  void mix(const AudioBuffer& other, float mixLevel = 0.5f);

  // Utility
  float getPeakAmplitude() const;
  float getRMSAmplitude() const;

private:
  std::vector<float> data_;
  size_t sampleRate_;
  size_t channels_;
  size_t frameCount_;
};