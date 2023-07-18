#include "ui/Window.h"
#include <iostream>

Window::Window(const std::string& title, int width, int height)
  : window_(nullptr), renderer_(nullptr), title_(title),
  width_(width), height_(height), initialized_(false) {}

Window::~Window() {
  close();
}

bool Window::initialize() {
  if (initialized_) return true;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize: " << SDL_GetError() << std::endl;
    return false;
  }

  window_ = SDL_CreateWindow(
    title_.c_str(),
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    width_,
    height_,
    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
  );

  if (!window_) {
    std::cerr << "Window could not be created: " << SDL_GetError() << std::endl;
    return false;
  }

  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);

  if (!renderer_) {
    std::cerr << "Renderer could not be created: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window_);
    window_ = nullptr;
    return false;
  }

  // Set renderer properties
  SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);

  initialized_ = true;
  return true;
}

void Window::close() {
  if (renderer_) {
    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
  }

  if (window_) {
    SDL_DestroyWindow(window_);
    window_ = nullptr;
  }
  initialized_ = false;
}

void Window::clear() {
  if (renderer_) {
    SDL_RenderClear(renderer_);
  }
}

void Window::present() {
  if (renderer_) {
    SDL_RenderPresent(renderer_);
  }
}

void Window::setTitle(const std::string& title) {
  title_ = title;

  if (window_) {
    SDL_SetWindowTitle(window_, title_.c_str());
  }
}

bool Window::pollEvent(SDL_Event& event) {
  return SDL_PollEvent(&event) != 0;
}