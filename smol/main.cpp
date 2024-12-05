#ifdef _DEBUG
#include <print>
#endif
#include "OpenGl.hpp"
#include "Windows.hpp"

#define uint32_t unsigned int
#include "assets/tri_v.hpp"
#include "assets/tri_f.hpp"

void checkOpenGlError() {
#ifdef _DEBUG
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    std::println("OpenGL error {}: ", err);
  }
#endif
}


GLuint createShaderFromSPIRVHeader(GLenum shaderType, const unsigned int* spirvCode, size_t spirvCodeSize) {
  GLuint shader = glCreateShader(shaderType);
  glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, spirvCode, spirvCodeSize);
  glSpecializeShader(shader, "main", 0, nullptr, nullptr);
  return shader;
}

int main() {
  Windows windows(1024, 768);
  // TODO(vug): call init from constructor
  windows.initOpenGlFunctions();

  GLuint vertexShader = createShaderFromSPIRVHeader(GL_VERTEX_SHADER, vertex_shader_bin, sizeof(vertex_shader_bin));
  GLuint fragmentShader = createShaderFromSPIRVHeader(GL_FRAGMENT_SHADER, fragment_shader_bin, sizeof(fragment_shader_bin));
  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  uint32_t vertexArray;
  glGenVertexArrays(1, &vertexArray);

  // TODO(vug): do the rotating triangle example for the repo

  while (!windows.getKeyState(0x1B /*VK_ESCAPE*/)) {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    windows.swapBuffers();
  }

  windows.exit();
}