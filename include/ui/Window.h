#pragma once

#include <SDL2/SDL.h>
#include <string>

class Window {
public:
  Window(const std::string& title, int width, int height);
  ~Window();

  // Window management
  bool initialize();
  void close();
  bool isOpen() const { return window_ != nullptr; }

  // Rendering
  void clear();
  void present();
  SDL_Renderer* getRenderer() const { return renderer_; }

  // Window properties
  int getWidth() const { return width_; }
  int getHeight() const { return height_; }
  void setTitle(const std::string& title);

  // Event handling
  bool pollEvent(SDL_Event& event);

private:
  SDL_Window* window_;
  SDL_Renderer* renderer_;
  std::string title_;
  int width_;
  int height_;
  bool initialized_;
};