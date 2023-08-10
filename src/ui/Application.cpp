#include "ui/Application.h"
#include <iostream>
#include <cmath>
#include <filesystem>

Application::Application()
  : running_(false), audioLoaded_(false), currentGain_(1.0f), showWaveform_(true), audioPlaying_(false) {}

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
  audioPlayer_ = std::make_unique<AudioPlayer>();
  fileLoader_ = std::make_unique<AudioFileLoader>();

  if (!audioPlayer_->initialize()) {
    std::cerr << "Failed to initialize audio player" << std::endl;
    return false;
  }

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
  std::cout << "  SPACE - Play/Pause audio" << std::endl;
  std::cout << "  S - Stop audio" << std::endl;
  std::cout << "  L - Load WAV file (if available)" << std::endl;

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

void Application::loadAudioFile(const std::string& filename) {
  if (!fileLoader_ || !audioBuffer_) return;

  if (!fileLoader_->canLoadFile(filename)) {
    std::cout << "Cannot load file: " << filename << " (not a valid WAV file)" << std::endl;
    return;
  }

  if (fileLoader_->loadWavFile(filename, *audioBuffer_)) {
    audioLoaded_ = true;
    waveformView_->setAudioBuffer(*audioBuffer_);

    // Stop any current playback
    if (audioPlaying_) {
      audioPlayer_->stop();
      audioPlaying_ = false;
    }

    std::cout << "Audio file loaded successfully" << std::endl;
  }
  else {
    std::cout << "Failed to load audio file: " << filename << std::endl;
  }
}

void Application::applyGainEffect(float gain) {
  if (!gainEffect_ || !audioBuffer_ || !waveformView_) return;

  currentGain_ = gain;
  gainEffect_->setGain(gain);

  // Apply effect directly to the original buffer
  gainEffect_->process(*audioBuffer_);

  // Update waveform with the modified audio buffer
  waveformView_->setAudioBuffer(*audioBuffer_);

  if (audioPlayer_) {
    audioPlayer_->setVolume(gain);
  }

  std::cout << "Applied gain: " << gain << std::endl;
}

void Application::togglePlayback() {
  if (!audioPlayer_ || !audioBuffer_ || !audioLoaded_) return;

  if (audioPlaying_) {
    if (audioPlayer_->isPaused()) {
      audioPlayer_->resume();
      std::cout << "Audio resumed" << std::endl;
    }
    else {
      audioPlayer_->pause();
      std::cout << "Audio paused" << std::endl;
    }
  }
  else {
    audioPlayer_->play(*audioBuffer_);
    audioPlaying_ = true;
    std::cout << "Audio started" << std::endl;
  }
}

void Application::stopPlayback() {
  if (!audioPlayer_) return;

  audioPlayer_->stop();
  audioPlaying_ = false;
  std::cout << "Audio stopped" << std::endl;
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

          case SDLK_SPACE: {
            togglePlayback();
            break;
          }

          case SDLK_s: {
            stopPlayback();
            break;
          }

          case SDLK_l: {
            if (std::filesystem::exists("sample.wav")) {
              std::cout << "Loading sample.wav file..." << std::endl;
              loadAudioFile("sample.wav");
              std::cout << "File loading completed." << std::endl;
            }
            else {
              std::cout << "No sample.wav file found. Create a WAV file named 'sample.wav' to test file loading." << std::endl;
            }
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

  if (showWaveform_) {
    SDL_Rect border = { 45, 95, 810, 310 };
    SDL_RenderDrawRect(window_->getRenderer(), &border);
  }

  window_->present();
}