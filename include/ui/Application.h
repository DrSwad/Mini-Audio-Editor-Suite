#pragma once

#include "Window.h"
#include "WaveformView.h"
#include "audio/AudioBuffer.h"
#include "audio/GainEffect.h"
#include "audio/AudioPlayer.h"
#include "audio/AudioFileLoader.h"
#include <memory>

class Application {
public:
  Application();
  ~Application();

  bool initialize();
  void run();
  void shutdown();

  void loadTestAudio();
  void loadAudioFile(const std::string& filename);
  void applyGainEffect(float gain);
  void togglePlayback();
  void stopPlayback();

  void handleEvents();
  void update();
  void render();

  bool isRunning() const { return running_; }
  void quit() { running_ = false; }

private:
  std::unique_ptr<Window> window_;
  std::unique_ptr<WaveformView> waveformView_;
  std::unique_ptr<AudioBuffer> audioBuffer_;
  std::unique_ptr<GainEffect> gainEffect_;
  std::unique_ptr<AudioPlayer> audioPlayer_;
  std::unique_ptr<AudioFileLoader> fileLoader_;

  bool running_;
  bool audioLoaded_;

  float currentGain_;
  bool showWaveform_;
  bool audioPlaying_;
};