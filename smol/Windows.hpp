#pragma once

struct HWND__;
typedef struct HWND__* HWND;
struct HDC__;
typedef struct HDC__* HDC;
struct HGLRC__;
typedef struct HGLRC__* HGLRC;
struct HINSTANCE__;
typedef struct HINSTANCE__* HINSTANCE;


class Windows {
 public:
  Windows(int width, int height);
  void logToConsole(const char* message) const;
  void* getAnyGLFuncAddress(const char* name) const;
  void initOpenGlFunctions() const;
  short getKeyState(int keyCode) const;
  void swapBuffers() const;
  void exit() const;

  void* kStdOut;
  const int kWidth = 1024;
  const int kHeight = 768;
  const HWND hWindow;
  const HDC hDevice;
  const HGLRC hContext;
  const HINSTANCE openglLib;
};