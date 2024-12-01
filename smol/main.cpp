#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#ifdef _DEBUG
#include <print>
#endif

int main() {
  const HANDLE kStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  const int kWidth = 1024;
  const int kHeight = 768;

#ifdef _DEBUG
  std::println("Hi, Debug!");
#elif NDEBUG
  {
    const char message[] = "Hi, Optimized!\n";
    WriteConsoleA(kStdOut, message, sizeof(message) - 1, nullptr, nullptr);
  }
#endif

  // By default STATIC window will have title also drawn inside the window. But that'd be erased when window is cleared
  const HWND hWindow = CreateWindowA(
      "STATIC", "Smol", WS_OVERLAPPED,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      kWidth, kHeight, nullptr, nullptr, nullptr, nullptr);
  ShowCursor(0);  // Hides mouse cursor over the window
  ShowWindow(hWindow, SW_SHOW);
  UpdateWindow(hWindow);

  while (!GetAsyncKeyState(VK_ESCAPE)) {
  }

  {
    const char message[] = "Bye!\n";
    WriteConsoleA(kStdOut, message, sizeof(message) - 1, nullptr, nullptr);
  }
  ExitProcess(0);
}