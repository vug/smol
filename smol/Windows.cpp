
#include "OpenGL.hpp"
#include "Windows.hpp"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

extern "C" {
  int _fltused = 0;
}

Windows::Windows(int width, int height)
    : kStdOut(GetStdHandle(STD_OUTPUT_HANDLE)),
      kWidth(width),
      kHeight(height),
      hWindow([this]() {
        // By default STATIC window will have title also drawn inside the
        // window. But that'd be erased when window is cleared
        const HWND hWindow = CreateWindowA(
            "STATIC", "Smol", WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT,
            kWidth, kHeight, nullptr, nullptr, nullptr, nullptr);
        ShowCursor(0);  // Hides mouse cursor over the window
        ShowWindow(hWindow, SW_SHOW);
        UpdateWindow(hWindow);
        return hWindow;
      }()),
      hDevice(GetDC(hWindow)),
      hContext([this]() {
        const PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32,0,0,0,0,0,0,0,0,0,0,0,0,0,24,8,0,PFD_MAIN_PLANE,0,0,0,0};
        int format = ChoosePixelFormat(hDevice, &pfd);
        SetPixelFormat(hDevice, format, &pfd);
        HGLRC hContext = wglCreateContext(hDevice);
        wglMakeCurrent(hDevice, hContext);
        return hContext;
      }()),
      openglLib(LoadLibraryA("opengl32.dll")) {}

void Windows::logToConsole(const char* message) const {
  unsigned int messageLength{};
  while (message[messageLength] != '\0' && messageLength < 1024) {
    messageLength++;
  }
  WriteConsoleA(kStdOut, message, messageLength, nullptr, nullptr);
}

void *Windows::getAnyGLFuncAddress(const char *name) const {
  void *p = (void *)wglGetProcAddress(name);
  if (p == 0 || (p == (void *)0x1) || (p == (void *)0x2) ||
      (p == (void *)0x3) || (p == (void *)-1)) {
    p = (void *)GetProcAddress(openglLib, name);
  }
  return p;
}

#define GET_PROC_ADDRESS(method) \
  method = (FnPtrT(method))getAnyGLFuncAddress(#method);

void Windows::initOpenGlFunctions() const {
  GET_PROC_ADDRESS(glGetString);
  const char *version = (const char *)glGetString(GL_VERSION);
  logToConsole(version);
  GET_PROC_ADDRESS(glClearColor);
  GET_PROC_ADDRESS(glClear);

  // GET_PROC_ADDRESS(wglCreateContextAttribsARB);
  // GET_PROC_ADDRESS(wglChoosePixelFormatARB);
}

short Windows::getKeyState(int keyCode) const {
  return GetAsyncKeyState(keyCode);
}

void Windows::swapBuffers() const { 
  SwapBuffers(hDevice); 
}

void Windows::exit() const {
  ExitProcess(0);
}