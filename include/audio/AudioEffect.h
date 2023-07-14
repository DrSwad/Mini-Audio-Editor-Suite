#pragma once

#include "AudioBuffer.h"

class AudioEffect {
public:
  virtual ~AudioEffect() = default;

  virtual void process(AudioBuffer& buffer) = 0;

  void setEnabled(bool enabled) { enabled_ = enabled; }
  bool isEnabled() const { return enabled_; }

  virtual const char* getName() const = 0;

protected:
  bool enabled_ = true;
};