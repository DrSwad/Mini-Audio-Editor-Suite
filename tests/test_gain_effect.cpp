#include "audio/GainEffect.h"
#include <cassert>
#include <cmath>
#include <iostream>

void testGainEffectConstruction() {
  GainEffect effect(2.0f);

  assert(std::abs(effect.getGain() - 2.0f) < 0.001f);
  assert(effect.isEnabled());
  std::cout << "✓ GainEffect construction test passed" << std::endl;
}

void testGainEffectProcessing() {
  GainEffect effect(0.5f);
  AudioBuffer buffer(44100, 2);

  buffer.resize(100);

  // Set test data
  for (size_t i = 0; i < 100; ++i) {
    buffer.setSample(i, 0, 1.0f);
    buffer.setSample(i, 1, 0.5f);
  }

  effect.process(buffer);

  // Check that gain was applied
  assert(std::abs(buffer.getSample(50, 0) - 0.5f) < 0.001f);
  assert(std::abs(buffer.getSample(50, 1) - 0.25f) < 0.001f);

  std::cout << "✓ GainEffect processing test passed" << std::endl;
}

void testGainEffectDisabled() {
  GainEffect effect(0.5f);

  effect.setEnabled(false);

  AudioBuffer buffer(44100, 2);
  buffer.resize(100);

  // Set test data
  for (size_t i = 0; i < 100; ++i) {
    buffer.setSample(i, 0, 1.0f);
    buffer.setSample(i, 1, 0.5f);
  }

  float originalLeft = buffer.getSample(50, 0);
  float originalRight = buffer.getSample(50, 1);

  effect.process(buffer);

  // Check that no gain was applied when disabled
  assert(std::abs(buffer.getSample(50, 0) - originalLeft) < 0.001f);
  assert(std::abs(buffer.getSample(50, 1) - originalRight) < 0.001f);

  std::cout << "✓ GainEffect disabled test passed" << std::endl;
}

void testGainEffectSetGain() {
  GainEffect effect(1.0f);

  effect.setGain(3.0f);
  assert(std::abs(effect.getGain() - 3.0f) < 0.001f);

  // Test negative gain (should clamp to 0)
  effect.setGain(-1.0f);
  assert(std::abs(effect.getGain() - 0.0f) < 0.001f);

  std::cout << "✓ GainEffect setGain test passed" << std::endl;
}
