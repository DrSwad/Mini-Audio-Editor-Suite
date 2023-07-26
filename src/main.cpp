#include <iostream>
#include "audio/AudioBuffer.h"
#include "audio/GainEffect.h"
#include "ui/Window.h"
#include "ui/WaveformView.h"

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

  Window window("Mini Audio Editor Suite", 1024, 768);

  if (window.initialize()) {
    std::cout << "Window created successfully:" << std::endl;
    std::cout << "  Width: " << window.getWidth() << std::endl;
    std::cout << "  Height: " << window.getHeight() << std::endl;

    WaveformView waveform(50, 100, 400, 200);
    waveform.setAudioBuffer(buffer);
    waveform.setColor(0, 255, 0, 255);

    std::cout << "Created waveform view with audio data" << std::endl;

    window.clear();
    waveform.render(window.getRenderer());
    window.present();

    std::cout << "Rendered waveform visualization. Window will close in 3 seconds..." << std::endl;
    SDL_Delay(3000);

    window.close();
  }
  else {
    std::cout << "Failed to create window!" << std::endl;
  }

  return 0;
}