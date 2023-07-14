#include <iostream>
#include "audio/AudioBuffer.h"
#include "audio/GainEffect.h"

int main() {
  std::cout << "Mini Audio Editor Suite" << std::endl;
  std::cout << "==========================" << std::endl;

  AudioBuffer buffer(44100, 2);
  buffer.resize(1024);

  std::cout << "AudioBuffer created successfully:" << std::endl;
  std::cout << "  Sample Rate: " << buffer.getSampleRate() << " Hz" << std::endl;
  std::cout << "  Channels: " << buffer.getChannelCount() << std::endl;
  std::cout << "  Frames: " << buffer.getFrameCount() << std::endl;

  GainEffect gainEffect(0.5f);
  std::cout << "GainEffect created with gain: " << gainEffect.getGain() << std::endl;

  for (size_t i = 0; i < 100; ++i) {
    buffer.setSample(i, 0, 1.0f);
    buffer.setSample(i, 1, 0.5f);
  }

  gainEffect.process(buffer);
  std::cout << "Applied gain effect. Sample at frame 50, channel 0: "
            << buffer.getSample(50, 0) << std::endl;

  return 0;
}