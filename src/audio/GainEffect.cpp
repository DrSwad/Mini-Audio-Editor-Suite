#include "audio/GainEffect.h"
#include <algorithm>

GainEffect::GainEffect(float gain) : gain_(gain) {
  if (gain < 0.0f) gain_ = 0.0f;
}

void GainEffect::process(AudioBuffer& buffer) {
  if (!enabled_) return;

  buffer.applyGain(gain_);
}

void GainEffect::setGain(float gain) {
  gain_ = std::max(0.0f, gain);
}