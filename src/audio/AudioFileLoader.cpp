#include "audio/AudioFileLoader.h"
#include <fstream>
#include <iostream>
#include <cstring>

AudioFileLoader::AudioFileLoader() {}

bool AudioFileLoader::loadWavFile(const std::string& filename, AudioBuffer& buffer) {
  std::cout << "Starting to load WAV file: " << filename << std::endl;

  int sampleRate, channels, frameCount;

  std::cout << "Reading WAV header..." << std::endl;

  if (!readWavHeader(filename, sampleRate, channels, frameCount)) {
    std::cerr << "Failed to read WAV header from: " << filename << std::endl;
    return false;
  }

  std::cout << "Header read successfully:" << std::endl;
  std::cout << "  Sample Rate: " << sampleRate << " Hz" << std::endl;
  std::cout << "  Channels: " << channels << std::endl;
  std::cout << "  Frame Count: " << frameCount << std::endl;
  std::cout << "  Duration: " << (float) frameCount / sampleRate << " seconds" << std::endl;

  // Initialize buffer with file parameters
  std::cout << "Initializing audio buffer..." << std::endl;
  buffer = AudioBuffer(sampleRate, channels);
  buffer.resize(frameCount);

  std::cout << "Reading WAV data..." << std::endl;

  if (!readWavData(filename, buffer)) {
    std::cerr << "Failed to read WAV data from: " << filename << std::endl;
    return false;
  }

  std::cout << "WAV file loaded successfully!" << std::endl;

  return true;
}

bool AudioFileLoader::canLoadFile(const std::string& filename) const {
  std::ifstream file(filename, std::ios::binary);

  if (!file.is_open()) {
    return false;
  }

  char header[12];
  file.read(header, 12);
  file.close();

  // Check for "RIFF" and "WAVE" identifiers
  return (strncmp(header, "RIFF", 4) == 0 && strncmp(header + 8, "WAVE", 4) == 0);
}

bool AudioFileLoader::getFileInfo(const std::string& filename, int& sampleRate, int& channels, int& frameCount) const {
  return readWavHeader(filename, sampleRate, channels, frameCount);
}

bool AudioFileLoader::readWavHeader(const std::string& filename, int& sampleRate, int& channels, int& frameCount) const {
  std::ifstream file(filename, std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "Cannot open file: " << filename << std::endl;
    return false;
  }

  // Read RIFF header
  char riffHeader[12];
  file.read(riffHeader, 12);

  if (strncmp(riffHeader, "RIFF", 4) != 0 || strncmp(riffHeader + 8, "WAVE", 4) != 0) {
    std::cerr << "Not a valid WAV file: " << filename << std::endl;
    return false;
  }

  std::cout << "WAV file header valid" << std::endl;

  // Parse chunks sequentially
  char chunkId[4];
  uint32_t chunkSize;
  bool formatFound = false;
  bool dataFound = false;

  while (!file.eof() && (!formatFound || !dataFound)) {
    if (file.read(chunkId, 4).gcount() != 4) break;

    if (file.read(reinterpret_cast<char*>(&chunkSize), 4).gcount() != 4) break;

    std::cout << "Found chunk: " << std::string(chunkId, 4) << " (size: " << chunkSize << ")" << std::endl;

    if (strncmp(chunkId, "fmt ", 4) == 0) {
      formatFound = true;

      uint16_t audioFormat, numChannels, bitsPerSample;
      uint32_t sampleRateValue, byteRate;
      uint16_t blockAlign;

      file.read(reinterpret_cast<char*>(&audioFormat), 2);
      file.read(reinterpret_cast<char*>(&numChannels), 2);
      file.read(reinterpret_cast<char*>(&sampleRateValue), 4);
      file.read(reinterpret_cast<char*>(&byteRate), 4);
      file.read(reinterpret_cast<char*>(&blockAlign), 2);
      file.read(reinterpret_cast<char*>(&bitsPerSample), 2);

      std::cout << "Format chunk: format=" << audioFormat << ", channels=" << numChannels
                << ", sampleRate=" << sampleRateValue << ", bitsPerSample=" << bitsPerSample << std::endl;

      if (audioFormat != 1) {       // PCM format
        std::cerr << "Unsupported audio format (not PCM): " << audioFormat << std::endl;
        return false;
      }

      if (bitsPerSample != 16) {
        std::cerr << "Unsupported bit depth (not 16-bit): " << bitsPerSample << std::endl;
        return false;
      }

      sampleRate = sampleRateValue;
      channels = numChannels;

      // Skip remaining format chunk data
      if (chunkSize > 16) {
        file.seekg(chunkSize - 16, std::ios::cur);
      }
    }
    else if (strncmp(chunkId, "data", 4) == 0) {
      dataFound = true;
      frameCount = chunkSize / (channels * sizeof(int16_t));       // 16-bit PCM
      std::cout << "Data chunk found. Frame count: " << frameCount << std::endl;
      break;       // Found data chunk, we can stop
    }
    else {
      // Skip this chunk
      file.seekg(chunkSize, std::ios::cur);
    }
  }

  file.close();
  return formatFound && dataFound;
}

bool AudioFileLoader::readWavData(const std::string& filename, AudioBuffer& buffer) const {
  std::ifstream file(filename, std::ios::binary);

  if (!file.is_open()) {
    return false;
  }

  // Read RIFF header
  char riffHeader[12];
  file.read(riffHeader, 12);

  if (strncmp(riffHeader, "RIFF", 4) != 0 || strncmp(riffHeader + 8, "WAVE", 4) != 0) {
    std::cerr << "Not a valid WAV file" << std::endl;
    return false;
  }

  // Parse chunks sequentially
  char chunkId[4];
  uint32_t chunkSize;
  bool formatFound = false;
  bool dataFound = false;

  while (!file.eof() && (!formatFound || !dataFound)) {
    if (file.read(chunkId, 4).gcount() != 4) break;

    if (file.read(reinterpret_cast<char*>(&chunkSize), 4).gcount() != 4) break;

    if (strncmp(chunkId, "fmt ", 4) == 0) {
      formatFound = true;

      uint16_t audioFormat, numChannels, bitsPerSample;
      uint32_t sampleRateValue, byteRate;
      uint16_t blockAlign;

      file.read(reinterpret_cast<char*>(&audioFormat), 2);
      file.read(reinterpret_cast<char*>(&numChannels), 2);
      file.read(reinterpret_cast<char*>(&sampleRateValue), 4);
      file.read(reinterpret_cast<char*>(&byteRate), 4);
      file.read(reinterpret_cast<char*>(&blockAlign), 2);
      file.read(reinterpret_cast<char*>(&bitsPerSample), 2);

      // Skip remaining format chunk data
      if (chunkSize > 16) {
        file.seekg(chunkSize - 16, std::ios::cur);
      }
    }
    else if (strncmp(chunkId, "data", 4) == 0) {
      dataFound = true;

      // Read audio data
      size_t frameCount = buffer.getFrameCount();
      size_t channelCount = buffer.getChannelCount();

      std::cout << "Reading " << frameCount << " frames with " << channelCount << " channels..." << std::endl;
      std::cout << "Total samples to read: " << frameCount * channelCount << std::endl;

      // Read all data at once for better performance
      size_t totalSamples = frameCount * channelCount;
      std::vector<int16_t> rawSamples(totalSamples);

      std::cout << "Allocating " << totalSamples * sizeof(int16_t) << " bytes..." << std::endl;

      std::cout << "Starting file read..." << std::endl;
      file.read(reinterpret_cast<char*>(rawSamples.data()), totalSamples * sizeof(int16_t));
      std::cout << "File read completed. Bytes read: " << file.gcount() << std::endl;

      if (static_cast<size_t>(file.gcount()) != totalSamples * sizeof(int16_t)) {
        std::cerr << "Failed to read all audio data" << std::endl;
        return false;
      }

      std::cout << "Loading progress: 50% - Data read, converting to float..." << std::endl;

      // Convert 16-bit PCM to float and copy to buffer
      for (size_t frame = 0; frame < frameCount; ++frame) {
        for (size_t channel = 0; channel < channelCount; ++channel) {
          size_t sampleIndex = frame * channelCount + channel;
          // Convert 16-bit PCM to float (-1.0 to 1.0 range)
          float sample = static_cast<float>(rawSamples[sampleIndex]) / 32768.0f;
          buffer.setSample(frame, channel, sample);
        }
      }

      std::cout << "Loading progress: 100%" << std::endl;
      break;       // Found data chunk, we can stop
    }
    else {
      // Skip this chunk
      file.seekg(chunkSize, std::ios::cur);
    }
  }

  file.close();
  return formatFound && dataFound;
}