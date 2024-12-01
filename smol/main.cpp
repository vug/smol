#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#ifdef _DEBUG
#include <print>
#endif
#include <glad/gl.h>

void SetPixelFormatDescriptor(HDC hdc) {
  PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
      PFD_TYPE_RGBA,
      32,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      0,
      24,
      8,
      0,
      PFD_MAIN_PLANE,
      0,
      0,
      0,
      0};
  int format = ChoosePixelFormat(hdc, &pfd);
  SetPixelFormat(hdc, format, &pfd);
}

void CreateOpenGLContext(HWND hWnd) {
  HDC hdc = GetDC(hWnd);

  // Set pixel format for the window's device context.
  SetPixelFormatDescriptor(hdc);

  // Create a legacy OpenGL context for bootstrapping.
  HGLRC tempContext = wglCreateContext(hdc);
  if (!tempContext) {
    MessageBoxA(NULL, "Failed to create temporary context", "Error", MB_OK);
    ExitProcess(0);
  }

  // Make the temporary context current.
  wglMakeCurrent(hdc, tempContext);

  // Load GLAD functions using the internal loader.
  const int version = gladLoaderLoadGL();
  if (!version) {
    MessageBoxA(NULL, "Failed to initialize GLAD", "Error", MB_OK);
    ExitProcess(0);
  } else {
#ifdef _DEBUG
    std::println("version {}.{}", GLAD_VERSION_MAJOR(version),  GLAD_VERSION_MINOR(version));
#endif
  }
}


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

  CreateOpenGLContext(hWindow);

  while (!GetAsyncKeyState(VK_ESCAPE)) {
  }

  {
    const char message[] = "Bye!\n";
    WriteConsoleA(kStdOut, message, sizeof(message) - 1, nullptr, nullptr);
  }
  ExitProcess(0);
}