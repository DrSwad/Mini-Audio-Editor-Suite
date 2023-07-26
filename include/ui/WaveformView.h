#pragma once

#include "Window.h"
#include "audio/AudioBuffer.h"
#include <vector>

class WaveformView {
public:
  WaveformView(int x, int y, int width, int height);

  void setAudioBuffer(const AudioBuffer& buffer);

  void render(SDL_Renderer* renderer);

  void setPosition(int x, int y);
  void setSize(int width, int height);
  void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

  void setZoom(float zoom);
  void setScrollOffset(int offset);

  int getX() const { return x_; }
  int getY() const { return y_; }
  int getWidth() const { return width_; }
  int getHeight() const { return height_; }

private:
  void updateWaveformData();
  void drawWaveform(SDL_Renderer* renderer);

  int x_, y_, width_, height_;
  SDL_Color color_;
  float zoom_;
  int scrollOffset_;

  const AudioBuffer* audioBuffer_;
  std::vector<float> waveformData_;
  bool dataUpdated_;
};