#pragma once

#include "AudioEffect.h"

class GainEffect : public AudioEffect {
public:
  GainEffect(float gain = 1.0f);

  void process(AudioBuffer& buffer) override;
  const char* getName() const override { return "Gain"; }

  void setGain(float gain);
  float getGain() const { return gain_; }

private:
  float gain_;
};