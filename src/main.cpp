#include <iostream>
#include "audio/AudioBuffer.h"

int main() {
  std::cout << "Mini Audio Editor Suite" << std::endl;
  std::cout << "==========================" << std::endl;

  AudioBuffer buffer(44100, 2);
  buffer.resize(1024);

  std::cout << "AudioBuffer created successfully:" << std::endl;
  std::cout << "  Sample Rate: " << buffer.getSampleRate() << " Hz" << std::endl;
  std::cout << "  Channels: " << buffer.getChannelCount() << std::endl;
  std::cout << "  Frames: " << buffer.getFrameCount() << std::endl;

  return 0;
}