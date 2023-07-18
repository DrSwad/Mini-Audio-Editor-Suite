#include "ui/Window.h"
#include <cassert>
#include <iostream>

void testWindowConstruction() {
    Window window("Test Window", 800, 600);
    assert(window.getWidth() == 800);
    assert(window.getHeight() == 600);
    assert(!window.isOpen()); // Not initialized yet
    std::cout << "✓ Window construction test passed" << std::endl;
}

void testWindowInitialization() {
    Window window("Test Window", 800, 600);
    bool initialized = window.initialize();
    assert(initialized);
    assert(window.isOpen());
    assert(window.getRenderer() != nullptr);
    std::cout << "✓ Window initialization test passed" << std::endl;
    
    // Clean up
    window.close();
}

void testWindowRendering() {
    Window window("Test Window", 800, 600);
    assert(window.initialize());
    
    // Test basic rendering operations
    window.clear();
    window.present();
    
    // If we get here without crashes, rendering works
    std::cout << "✓ Window rendering test passed" << std::endl;
    
    window.close();
}

void testWindowEventHandling() {
    Window window("Test Window", 800, 600);
    assert(window.initialize());
    
    SDL_Event event;
    // This should not crash even if no events are available
    bool hasEvent = window.pollEvent(event);
    
    std::cout << "✓ Window event handling test passed" << std::endl;
    
    window.close();
}

void testWindowTitle() {
    Window window("Test Window", 800, 600);
    assert(window.initialize());
    
    window.setTitle("New Title");
    // Note: We can't easily test if the title actually changed in SDL
    // without more complex window management, but the call shouldn't crash
    
    std::cout << "✓ Window title test passed" << std::endl;
    
    window.close();
} 