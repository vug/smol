#include "OpenGl.hpp"
#include "Windows.hpp"

#define uint32_t unsigned int
#include "assets/tri_v.hpp"
#include "assets/tri_f.hpp"

constexpr float smolSqrt(float value) {
  float result;
  __asm {
    movss xmm0, value    ; Load the input value into the XMM0 register
    sqrtss xmm0, xmm0    ; Compute the square root of the value in XMM0
    movss result, xmm0   ; Store the result back to memory
  }
  return result;
}


struct Vec3 {
  float v[3];
};

struct Mesh {
  Vec3* positions;
  uint32_t numVertices;
  uint32_t* indices;
  uint32_t numIndices;
};

constexpr Mesh generateTetrahedron(const float size = 1.0f) {
  Mesh mesh;

  // Calculate vertices using a regular tetrahedron centered at origin
  const float a = size * 1.0f / smolSqrt(3.0f);
  const float b = size * smolSqrt(8.0f / 9.0f);
  const float c = size * -1.0f / 3.0f;
  
  mesh.positions = new Vec3[]{{0, a, 0},
                              {-size / 2, c, -b / 3},
                              {size / 2, c, -b / 3},
                              {0, c, 2 * b / 3},};
  mesh.numVertices = 4;

  // Indices for the four triangular faces
  // Counter-clockwise winding order for proper face culling
  mesh.indices = new unsigned int[]{
      0, 2, 1,  // Front face
      0, 3, 2,  // Right face
      0, 1, 3,  // Left face
      1, 2, 3,  // Bottom face
  };
  mesh.numIndices = 12;

  return mesh;
}

int main() {
  Windows windows(1024, 768);

  const Mesh tetrahedron = generateTetrahedron(1.0f);
  GLuint buffers[4];
  glCreateBuffers(4, buffers);
  auto [vbPosition, vbNormal, vbColor, ib] = buffers;
  //const GLsizeiptr bufferSizeBytes = tetrahedron.numVertices * sizeof(Vec3);
  //glNamedBufferStorage(vbPosition, bufferSizeBytes, tetrahedron.positions, 0);

  const GLuint vertexShader = createShaderFromSPIRVHeader(GL_VERTEX_SHADER, vertex_shader_bin, sizeof(vertex_shader_bin));
  const GLuint fragmentShader = createShaderFromSPIRVHeader(GL_FRAGMENT_SHADER, fragment_shader_bin, sizeof(fragment_shader_bin));
  const GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  uint32_t vertexArray;
  glCreateVertexArrays(1, &vertexArray);

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