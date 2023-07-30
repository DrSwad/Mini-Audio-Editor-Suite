#include <iostream>
#include "ui/Application.h"

int main() {
  std::cout << "Mini Audio Editor Suite" << std::endl;
  std::cout << "==========================" << std::endl;

  Application app;

  if (app.initialize()) {
    app.run();
  }
  else {
    std::cerr << "Failed to initialize application" << std::endl;
    return 1;
  }

  return 0;
}