#include "audio/AudioBuffer.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

AudioBuffer::AudioBuffer(size_t sampleRate, size_t channels)
  : sampleRate_(sampleRate), channels_(channels), frameCount_(0) {
  if (channels == 0) {
    throw std::invalid_argument("Channel count must be greater than 0");
  }
}

AudioBuffer::AudioBuffer(const AudioBuffer& other)
  : data_(other.data_), sampleRate_(other.sampleRate_),
  channels_(other.channels_), frameCount_(other.frameCount_) {}

AudioBuffer& AudioBuffer::operator=(const AudioBuffer& other) {
  if (this != &other) {
    data_ = other.data_;
    sampleRate_ = other.sampleRate_;
    channels_ = other.channels_;
    frameCount_ = other.frameCount_;
  }
  return *this;
}

void AudioBuffer::resize(size_t frames) {
  frameCount_ = frames;
  data_.resize(frames * channels_);
}

void AudioBuffer::clear() {
  std::fill(data_.begin(), data_.end(), 0.0f);
}

size_t AudioBuffer::getFrameCount() const {
  return frameCount_;
}

size_t AudioBuffer::getSampleRate() const {
  return sampleRate_;
}

size_t AudioBuffer::getChannelCount() const {
  return channels_;
}

float AudioBuffer::getSample(size_t frame, size_t channel) const {
  if (frame >= frameCount_ || channel >= channels_) {
    return 0.0f;
  }
  return data_[frame * channels_ + channel];
}

void AudioBuffer::setSample(size_t frame, size_t channel, float value) {
  if (frame >= frameCount_ || channel >= channels_) {
    return;
  }
  data_[frame * channels_ + channel] = value;
}

void AudioBuffer::normalize() {
  float peak = getPeakAmplitude();

  if (peak > 0.0f) {
    applyGain(1.0f / peak);
  }
}

void AudioBuffer::applyGain(float gain) {
  for (auto& sample : data_) {
    sample *= gain;
  }
}

void AudioBuffer::mix(const AudioBuffer& other, float mixLevel) {
  size_t minFrames = std::min(frameCount_, other.frameCount_);
  size_t minChannels = std::min(channels_, other.channels_);

  for (size_t frame = 0; frame < minFrames; ++frame) {
    for (size_t channel = 0; channel < minChannels; ++channel) {
      float currentSample = getSample(frame, channel);
      float otherSample = other.getSample(frame, channel);
      setSample(frame, channel, currentSample * (1.0f - mixLevel) + otherSample * mixLevel);
    }
  }
}

float AudioBuffer::getPeakAmplitude() const {
  if (data_.empty()) return 0.0f;

  float peak = 0.0f;

  for (float sample : data_) {
    peak = std::max(peak, std::abs(sample));
  }
  return peak;
}

float AudioBuffer::getRMSAmplitude() const {
  if (data_.empty()) return 0.0f;

  float sum = 0.0f;

  for (float sample : data_) {
    sum += sample * sample;
  }
  return std::sqrt(sum / data_.size());
}