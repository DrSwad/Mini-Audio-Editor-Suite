#include "ui/Application.h"
#include <iostream>
#include <cmath>

Application::Application()
  : running_(false), audioLoaded_(false), currentGain_(1.0f), showWaveform_(true) {}

Application::~Application() {
  shutdown();
}

bool Application::initialize() {
  window_ = std::make_unique<Window>("Mini Audio Editor Suite", 1024, 768);

  if (!window_->initialize()) {
    std::cerr << "Failed to initialize window" << std::endl;
    return false;
  }

  waveformView_ = std::make_unique<WaveformView>(50, 100, 800, 300);
  waveformView_->setColor(0, 255, 0, 255);

  audioBuffer_ = std::make_unique<AudioBuffer>(44100, 2);
  gainEffect_ = std::make_unique<GainEffect>(currentGain_);

  loadTestAudio();

  running_ = true;
  std::cout << "Application initialized successfully" << std::endl;
  return true;
}

void Application::run() {
  if (!running_) return;

  std::cout << "Starting application loop..." << std::endl;
  std::cout << "Controls:" << std::endl;
  std::cout << "  ESC - Quit" << std::endl;
  std::cout << "  +/- - Adjust gain" << std::endl;
  std::cout << "  W - Toggle waveform" << std::endl;

  while (running_) {
    handleEvents();
    update();
    render();

    SDL_Delay(16);
  }
}

void Application::shutdown() {
  running_ = false;

  if (window_) {
    window_->close();
  }
  SDL_Quit();
}

void Application::loadTestAudio() {
  if (!audioBuffer_) return;

  // Create a test audio signal (sine wave)
  audioBuffer_->resize(44100);   // 1 second at 44.1kHz

  for (size_t i = 0; i < audioBuffer_->getFrameCount(); ++i) {
    float time = static_cast<float>(i) / audioBuffer_->getSampleRate();
    float frequency = 440.0f;     // A4 note
    float amplitude = 0.3f;

    float sample = amplitude * std::sin(2.0f * M_PI * frequency * time);

    // Add some harmonics for more interesting waveform
    sample += 0.1f * amplitude * std::sin(2.0f * M_PI * frequency * 2.0f * time);
    sample += 0.05f * amplitude * std::sin(2.0f * M_PI * frequency * 3.0f * time);

    audioBuffer_->setSample(i, 0, sample);
    audioBuffer_->setSample(i, 1, sample);
  }

  audioLoaded_ = true;
  waveformView_->setAudioBuffer(*audioBuffer_);

  std::cout << "Loaded test audio: 1 second sine wave at 440Hz" << std::endl;
}

void Application::applyGainEffect(float gain) {
  if (!gainEffect_ || !audioBuffer_ || !waveformView_) return;

  currentGain_ = gain;
  gainEffect_->setGain(gain);

  // Apply effect directly to the original buffer
  gainEffect_->process(*audioBuffer_);

  // Update waveform with the modified audio buffer
  waveformView_->setAudioBuffer(*audioBuffer_);

  std::cout << "Applied gain: " << gain << std::endl;
}

void Application::handleEvents() {
  if (!window_) return;

  SDL_Event event;

  while (window_->pollEvent(event)) {
    switch (event.type) {
      case SDL_QUIT: {
        quit();
        break;
      }

      case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE: {
            quit();
            break;
          }

          case SDLK_EQUALS:           // + key
          case SDLK_PLUS: {
            applyGainEffect(currentGain_ + 0.1f);
            break;
          }

          case SDLK_MINUS: {
            applyGainEffect(currentGain_ - 0.1f);
            break;
          }

          case SDLK_w: {
            showWaveform_ = !showWaveform_;
            std::cout << "Waveform display: " << (showWaveform_ ? "ON" : "OFF") << std::endl;
            break;
          }
        }
        break;
      }
    }
  }
}

void Application::update() {
}

void Application::render() {
  if (!window_ || !window_->getRenderer()) return;

  window_->clear();

  if (showWaveform_ && waveformView_) {
    waveformView_->render(window_->getRenderer());
  }

  SDL_SetRenderDrawColor(window_->getRenderer(), 255, 255, 255, 255);

  // Draw a simple border around the waveform
  if (showWaveform_) {
    SDL_Rect border = { 45, 95, 810, 310 };
    SDL_RenderDrawRect(window_->getRenderer(), &border);
  }

  window_->present();
}