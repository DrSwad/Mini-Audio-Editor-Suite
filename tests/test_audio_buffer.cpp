#include "audio/AudioBuffer.h"
#include <cassert>
#include <cmath>
#include <iostream>

void testAudioBufferConstruction() {
  AudioBuffer buffer(44100, 2);

  assert(buffer.getSampleRate() == 44100);
  assert(buffer.getChannelCount() == 2);
  assert(buffer.getFrameCount() == 0);
  std::cout << "✓ AudioBuffer construction test passed" << std::endl;
}

void testAudioBufferResize() {
  AudioBuffer buffer(44100, 2);

  buffer.resize(1024);
  assert(buffer.getFrameCount() == 1024);
  std::cout << "✓ AudioBuffer resize test passed" << std::endl;
}

void testAudioBufferSampleAccess() {
  AudioBuffer buffer(44100, 2);

  buffer.resize(100);

  // Test setting and getting samples
  buffer.setSample(50, 0, 0.5f);
  buffer.setSample(50, 1, -0.3f);

  assert(std::abs(buffer.getSample(50, 0) - 0.5f) < 0.001f);
  assert(std::abs(buffer.getSample(50, 1) - (-0.3f)) < 0.001f);

  // Test bounds checking
  assert(buffer.getSample(200, 0) == 0.0f); // Out of bounds
  assert(buffer.getSample(50, 5) == 0.0f);  // Invalid channel

  std::cout << "✓ AudioBuffer sample access test passed" << std::endl;
}

void testAudioBufferGain() {
  AudioBuffer buffer(44100, 2);

  buffer.resize(100);

  // Set some test data
  for (size_t i = 0; i < 100; ++i) {
    buffer.setSample(i, 0, 0.5f);
    buffer.setSample(i, 1, 0.3f);
  }

  buffer.applyGain(2.0f);

  assert(std::abs(buffer.getSample(50, 0) - 1.0f) < 0.001f);
  assert(std::abs(buffer.getSample(50, 1) - 0.6f) < 0.001f);

  std::cout << "✓ AudioBuffer gain test passed" << std::endl;
}

void testAudioBufferPeakAmplitude() {
  AudioBuffer buffer(44100, 2);

  buffer.resize(100);

  // Set a peak value
  buffer.setSample(50, 0, 0.8f);
  buffer.setSample(50, 1, -0.6f);

  assert(std::abs(buffer.getPeakAmplitude() - 0.8f) < 0.001f);

  std::cout << "✓ AudioBuffer peak amplitude test passed" << std::endl;
}

void testAudioBufferNormalize() {
  AudioBuffer buffer(44100, 2);

  buffer.resize(100);

  // Set some test data with peak at 0.5
  for (size_t i = 0; i < 100; ++i) {
    buffer.setSample(i, 0, 0.5f);
    buffer.setSample(i, 1, 0.3f);
  }

  buffer.normalize();

  // Peak should now be 1.0
  assert(std::abs(buffer.getPeakAmplitude() - 1.0f) < 0.001f);

  std::cout << "✓ AudioBuffer normalize test passed" << std::endl;
}

void testAudioBufferMix() {
  AudioBuffer buffer1(44100, 2);
  AudioBuffer buffer2(44100, 2);

  buffer1.resize(100);
  buffer2.resize(100);

  // Set different values in each buffer
  for (size_t i = 0; i < 100; ++i) {
    buffer1.setSample(i, 0, 1.0f);
    buffer1.setSample(i, 1, 0.5f);
    buffer2.setSample(i, 0, 0.0f);
    buffer2.setSample(i, 1, 1.0f);
  }

  buffer1.mix(buffer2, 0.5f);

  // Mixed values should be 0.5 and 0.75
  assert(std::abs(buffer1.getSample(50, 0) - 0.5f) < 0.001f);
  assert(std::abs(buffer1.getSample(50, 1) - 0.75f) < 0.001f);

  std::cout << "✓ AudioBuffer mix test passed" << std::endl;
}
