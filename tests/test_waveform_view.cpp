#include "ui/WaveformView.h"
#include "audio/AudioBuffer.h"
#include <cassert>
#include <iostream>

void testWaveformViewConstruction() {
  WaveformView view(10, 20, 300, 200);

  assert(view.getX() == 10);
  assert(view.getY() == 20);
  assert(view.getWidth() == 300);
  assert(view.getHeight() == 200);
  std::cout << "✓ WaveformView construction test passed" << std::endl;
}

void testWaveformViewPosition() {
  WaveformView view(0, 0, 100, 100);

  view.setPosition(50, 75);
  assert(view.getX() == 50);
  assert(view.getY() == 75);
  std::cout << "✓ WaveformView position test passed" << std::endl;
}

void testWaveformViewSize() {
  WaveformView view(0, 0, 100, 100);

  view.setSize(200, 150);
  assert(view.getWidth() == 200);
  assert(view.getHeight() == 150);
  std::cout << "✓ WaveformView size test passed" << std::endl;
}

void testWaveformViewColor() {
  WaveformView view(0, 0, 100, 100);

  view.setColor(255, 0, 0, 128);   // Red with alpha
  // Note: We can't easily test the color was set without rendering
  // but the call shouldn't crash
  std::cout << "✓ WaveformView color test passed" << std::endl;
}

void testWaveformViewZoom() {
  WaveformView view(0, 0, 100, 100);

  view.setZoom(2.0f);
  view.setZoom(0.5f);
  // Note: We can't easily test the zoom was set without rendering
  // but the calls shouldn't crash
  std::cout << "✓ WaveformView zoom test passed" << std::endl;
}

void testWaveformViewAudioBuffer() {
  WaveformView view(0, 0, 100, 100);
  AudioBuffer buffer(44100, 2);

  buffer.resize(1000);

  // Add some test data
  for (size_t i = 0; i < 1000; ++i) {
    buffer.setSample(i, 0, 0.5f);
    buffer.setSample(i, 1, 0.3f);
  }

  view.setAudioBuffer(buffer);
  // Note: We can't easily test the buffer was set without rendering
  // but the call shouldn't crash
  std::cout << "✓ WaveformView audio buffer test passed" << std::endl;
}

void testWaveformViewEmptyBuffer() {
  WaveformView view(0, 0, 100, 100);
  AudioBuffer buffer(44100, 2);

  // Empty buffer

  view.setAudioBuffer(buffer);
  // Should not crash with empty buffer
  std::cout << "✓ WaveformView empty buffer test passed" << std::endl;
}