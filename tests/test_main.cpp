#include <iostream>

void testAudioBufferConstruction();
void testAudioBufferResize();
void testAudioBufferSampleAccess();
void testAudioBufferGain();
void testAudioBufferPeakAmplitude();
void testAudioBufferNormalize();
void testAudioBufferMix();

int main() {
  std::cout << "Running all unit tests..." << std::endl;
  std::cout << "=========================" << std::endl;

  testAudioBufferConstruction();
  testAudioBufferResize();
  testAudioBufferSampleAccess();
  testAudioBufferGain();
  testAudioBufferPeakAmplitude();
  testAudioBufferNormalize();
  testAudioBufferMix();

  std::cout << "=========================" << std::endl;
  std::cout << "All tests passed!" << std::endl;
  return 0;
}