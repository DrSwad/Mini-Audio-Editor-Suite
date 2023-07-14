#include <iostream>

void testAudioBufferConstruction();
void testAudioBufferResize();
void testAudioBufferSampleAccess();
void testAudioBufferGain();
void testAudioBufferPeakAmplitude();
void testAudioBufferNormalize();
void testAudioBufferMix();

void testGainEffectConstruction();
void testGainEffectProcessing();
void testGainEffectDisabled();
void testGainEffectSetGain();

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

  testGainEffectConstruction();
  testGainEffectProcessing();
  testGainEffectDisabled();
  testGainEffectSetGain();

  std::cout << "=========================" << std::endl;
  std::cout << "All tests passed!" << std::endl;
  return 0;
}