#include "ui/WaveformView.h"
#include <algorithm>
#include <cmath>

WaveformView::WaveformView(int x, int y, int width, int height)
  : x_(x), y_(y), width_(width), height_(height),
  color_({ 255, 255, 255, 255 }), zoom_(1.0f), scrollOffset_(0),
  audioBuffer_(nullptr), dataUpdated_(false) {}

void WaveformView::setAudioBuffer(const AudioBuffer& buffer) {
  audioBuffer_ = &buffer;
  dataUpdated_ = false;
}

void WaveformView::render(SDL_Renderer* renderer) {
  if (!audioBuffer_ || audioBuffer_->getFrameCount() == 0) {
    return;
  }

  updateWaveformData();
  drawWaveform(renderer);
}

void WaveformView::setPosition(int x, int y) {
  x_ = x;
  y_ = y;
}

void WaveformView::setSize(int width, int height) {
  width_ = width;
  height_ = height;
  dataUpdated_ = false;
}

void WaveformView::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  color_ = { r, g, b, a };
}

void WaveformView::setZoom(float zoom) {
  zoom_ = std::max(0.1f, zoom);
  dataUpdated_ = false;
}

void WaveformView::setScrollOffset(int offset) {
  scrollOffset_ = std::max(0, offset);
  dataUpdated_ = false;
}

void WaveformView::updateWaveformData() {
  if (!audioBuffer_ || dataUpdated_) {
    return;
  }

  waveformData_.clear();
  waveformData_.reserve(width_);

  size_t frameCount = audioBuffer_->getFrameCount();
  size_t channelCount = audioBuffer_->getChannelCount();

  if (frameCount == 0) return;

  // Calculate how many frames to skip for each pixel
  size_t framesPerPixel = std::max(1UL, frameCount / width_);

  for (int pixel = 0; pixel < width_; ++pixel) {
    size_t startFrame = pixel * framesPerPixel + scrollOffset_;

    if (startFrame >= frameCount) break;

    // Calculate RMS value for this pixel range
    float sum = 0.0f;
    size_t count = 0;

    for (size_t frame = startFrame;
         frame < startFrame + framesPerPixel && frame < frameCount;
         ++frame
    ) {
      for (size_t channel = 0; channel < channelCount; ++channel) {
        float sample = audioBuffer_->getSample(frame, channel);
        sum += sample * sample;
        count++;
      }
    }

    if (count > 0) {
      float rms = std::sqrt(sum / count);
      waveformData_.push_back(rms * zoom_);
    }
    else {
      waveformData_.push_back(0.0f);
    }
  }

  dataUpdated_ = true;
}

void WaveformView::drawWaveform(SDL_Renderer* renderer) {
  if (waveformData_.empty()) return;

  SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);

  int centerY = y_ + height_ / 2;

  for (int x = 0; x < static_cast<int>(waveformData_.size()) && x < width_; ++x) {
    float amplitude = waveformData_[x];
    int barHeight = static_cast<int>(amplitude * height_ / 2);

    // Draw center line
    SDL_RenderDrawPoint(renderer, x_ + x, centerY);

    // Draw waveform bars
    for (int y = 1; y <= barHeight; ++y) {
      if (centerY + y < y_ + height_) {
        SDL_RenderDrawPoint(renderer, x_ + x, centerY + y);
      }

      if (centerY - y >= y_) {
        SDL_RenderDrawPoint(renderer, x_ + x, centerY - y);
      }
    }
  }
}