#ifdef _DEBUG
#include <print>
#endif
#include "Windows.hpp"
#include "OpenGl.hpp"

int main() {
  Windows windows(1024, 768);
  windows.initOpenGlFunctions();

  while (!windows.getKeyState(0x1B /*VK_ESCAPE*/)) {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    windows.swapBuffers();
  }

  windows.exit();
}