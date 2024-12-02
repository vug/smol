#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#ifdef _DEBUG
#include <print>
#endif
#include "OpenGl.hpp"

extern "C" {
  int _fltused = 0;
}

void *GetAnyGLFuncAddress(const char *name) {
  void *p = (void *)wglGetProcAddress(name);
  if (p == 0 || (p == (void *)0x1) || (p == (void *)0x2) ||
      (p == (void *)0x3) || (p == (void *)-1)) {
    HMODULE module = LoadLibraryA("opengl32.dll");
    p = (void *)GetProcAddress(module, name);
  }

  return p;
}

#define GET_PROC_ADDRESS(method) \
  method = (FnPtrT(method))GetAnyGLFuncAddress(#method);

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

  // By default STATIC window will have title also drawn inside the window. But
  // that'd be erased when window is cleared
  const HWND hWindow = CreateWindowA(
      "STATIC", "Smol", WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT, kWidth,
      kHeight, nullptr, nullptr, nullptr, nullptr);
  ShowCursor(0);  // Hides mouse cursor over the window
  ShowWindow(hWindow, SW_SHOW);
  UpdateWindow(hWindow);

  HDC hDevice = GetDC(hWindow);
  const PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32,0,0,0,0,0,0,0,0,0,0,0,0,0,24,8,0,PFD_MAIN_PLANE,0,0,0,0};
  int format = ChoosePixelFormat(hDevice, &pfd);
  SetPixelFormat(hDevice, format, &pfd);

  HGLRC hContext = wglCreateContext(hDevice);
  if (!hContext) {
    MessageBoxA(NULL, "Failed to create temporary context", "Error", MB_OK);
    ExitProcess(0);
  }
  wglMakeCurrent(hDevice, hContext);

  void *handle = (void *)LoadLibraryA("opengl32.dll");
  if (!handle) {
    MessageBoxA(NULL, "Failed to load opengl32.dll", "Error", MB_OK);
    ExitProcess(0);
  }

  //GET_PROC_ADDRESS(wglCreateContextAttribsARB);
  //GET_PROC_ADDRESS(wglChoosePixelFormatARB);

  GET_PROC_ADDRESS(glGetString);
  const char* version = (const char *)glGetString(GL_VERSION);
  WriteConsoleA(kStdOut, version, sizeof(version) - 1, nullptr, nullptr);

  GET_PROC_ADDRESS(glClearColor);
  GET_PROC_ADDRESS(glClear);

  while (!GetAsyncKeyState(VK_ESCAPE)) {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //wglSwapLayerBuffers(hDevice, WGL_SWAP_MAIN_PLANE);
    SwapBuffers(hDevice);
  }

  {
    const char message[] = "Bye!\n";
    WriteConsoleA(kStdOut, message, sizeof(message) - 1, nullptr, nullptr);
  }
  ExitProcess(0);
}