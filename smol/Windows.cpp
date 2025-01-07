
#include "OpenGL.hpp"
#include "Windows.hpp"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

extern "C" {
  int _fltused = 0;
}

#define GET_PROC_ADDRESS(method) \
  method = reinterpret_cast<FnPtrT(method)>(getAnyGLFuncAddress(#method));

// To use RenderDoc uncomment below line:
// #define EXPLICITLY_SET_GL_VERSION

Windows::Windows(int width, int height)
    : kStdOut(GetStdHandle(STD_OUTPUT_HANDLE)),
      kWidth(width),
      kHeight(height),
      hWindow([this]() {
        // By default STATIC window will have title also drawn inside the
        // window. But that'd be erased when window is cleared
        const HWND hWindow = CreateWindowA(
            "STATIC", "Smol", WS_OVERLAPPED | CS_OWNDC, CW_USEDEFAULT,
            CW_USEDEFAULT,
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

#if defined(EXPLICITLY_SET_GL_VERSION)
        GET_PROC_ADDRESS(wglCreateContextAttribsARB);
        wglDeleteContext(hContext);
        int attribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 6,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0  // End of attributes list
        };
        hContext = wglCreateContextAttribsARB(hDevice, nullptr, attribs);
        wglMakeCurrent(hDevice, hContext);
#endif

        return hContext;
      }()),
      openglLib(LoadLibraryA("opengl32.dll")) {
  initOpenGlFunctions();
}

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

void Windows::initOpenGlFunctions() const {
  GET_PROC_ADDRESS(glGetError);
  GET_PROC_ADDRESS(glClearColor);
  GET_PROC_ADDRESS(glClear);
  GET_PROC_ADDRESS(glGetString);
  GET_PROC_ADDRESS(glCreateShader);
  GET_PROC_ADDRESS(glShaderBinary);
  GET_PROC_ADDRESS(glSpecializeShader);
  GET_PROC_ADDRESS(glCreateProgram);
  GET_PROC_ADDRESS(glAttachShader);
  GET_PROC_ADDRESS(glLinkProgram);
  GET_PROC_ADDRESS(glCreateVertexArrays);
  //GET_PROC_ADDRESS(glGenVertexArrays);
  GET_PROC_ADDRESS(glUseProgram);
  GET_PROC_ADDRESS(glBindVertexArray);
  GET_PROC_ADDRESS(glDrawArrays);
  GET_PROC_ADDRESS(glCreateBuffers);
  GET_PROC_ADDRESS(glNamedBufferStorage);
  GET_PROC_ADDRESS(glEnableVertexArrayAttrib);
  GET_PROC_ADDRESS(glVertexArrayAttribFormat);
  GET_PROC_ADDRESS(glVertexArrayVertexBuffer);
  GET_PROC_ADDRESS(glVertexArrayAttribBinding);
  GET_PROC_ADDRESS(glVertexArrayElementBuffer);
  GET_PROC_ADDRESS(glDrawElements);
  GET_PROC_ADDRESS(glBindBufferBase);
  GET_PROC_ADDRESS(glNamedBufferSubData);
  GET_PROC_ADDRESS(glViewport);
  GET_PROC_ADDRESS(glEnable);
  GET_PROC_ADDRESS(glDisable);
  //GET_PROC_ADDRESS(glShaderSource);
  //GET_PROC_ADDRESS(glCompileShader);
  //GET_PROC_ADDRESS(glGetShaderiv);
  //GET_PROC_ADDRESS(glGetProgramiv);
  //GET_PROC_ADDRESS(glGetShaderInfoLog);
  //GET_PROC_ADDRESS(glGetProgramInfoLog);
  //GET_PROC_ADDRESS(glDeleteBuffers);
  //GET_PROC_ADDRESS(glBindBuffer);
  //GET_PROC_ADDRESS(glBufferData);
  //GET_PROC_ADDRESS(glCreateVertexArrays);
  //GET_PROC_ADDRESS(glDeleteVertexArrays);
  //GET_PROC_ADDRESS(glEnableVertexAttribArray);
  //GET_PROC_ADDRESS(glVertexAttribPointer);
  //GET_PROC_ADDRESS(glDepthFunc);
  //GET_PROC_ADDRESS(glGetUniformLocation);
  //GET_PROC_ADDRESS(glUniform1f);
  //GET_PROC_ADDRESS(glUniformMatrix4fv);
  //GET_PROC_ADDRESS(glGenTextures);
  //GET_PROC_ADDRESS(glBindTexture);
  //GET_PROC_ADDRESS(glTexImage2D);
  //GET_PROC_ADDRESS(glGenFramebuffers);
  //GET_PROC_ADDRESS(glBindFramebuffer);
  //GET_PROC_ADDRESS(glFramebufferTexture2D);
  //GET_PROC_ADDRESS(glReadPixels);
  //GET_PROC_ADDRESS(glCheckFramebufferStatus);
  //GET_PROC_ADDRESS(glBufferSubData);
  //GET_PROC_ADDRESS(glScissor);
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

void *operator new[](size_t size) {
  HANDLE heap = GetProcessHeap();
  void *ptr = HeapAlloc(heap, HEAP_ZERO_MEMORY, size);
  return ptr;
}

void operator delete[](void *ptr) noexcept {
  HANDLE heap = GetProcessHeap();
  HeapFree(heap, 0, ptr);
}