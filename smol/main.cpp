#include "OpenGl.hpp"
#include "Windows.hpp"

#define uint32_t unsigned int
#include "assets/tri_v.hpp"
#include "assets/tri_f.hpp"

int main() {
  Windows windows(1024, 768);

  const GLuint vertexShader = createShaderFromSPIRVHeader(GL_VERTEX_SHADER, vertex_shader_bin, sizeof(vertex_shader_bin));
  const GLuint fragmentShader = createShaderFromSPIRVHeader(GL_FRAGMENT_SHADER, fragment_shader_bin, sizeof(fragment_shader_bin));
  const GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  uint32_t vertexArray;
  glGenVertexArrays(1, &vertexArray);

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