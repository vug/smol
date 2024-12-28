#ifdef _DEBUG
#include <print>
#endif

#include "Math.hpp"
#include "OpenGl.hpp"
#include "Windows.hpp"

#define uint32_t unsigned int
#include "assets/vert_col_v.hpp"
#include "assets/vert_col_f.hpp"

using namespace smol;

struct Mesh {
  Vec3* positions{};
  uint32_t numVertices{};
  uint32_t* indices{};
  uint32_t numIndices{};
  Vec3* normals{};
  Vec3* colors{};
};

constexpr Mesh generateTetrahedron(const float size = 1.0f) {
  Mesh mesh;

  // Calculate vertices using a regular tetrahedron centered at origin
  const float a = size * 1.0f / smol::sqrt(3.0f);
  const float b = size * smol::sqrt(8.0f / 9.0f);
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

#ifdef _DEBUG
  std::println("version: {}", (const char*)glGetString(GL_VERSION));
#endif

  Mesh tetrahedron = generateTetrahedron(1.0f);
  tetrahedron.colors = new Vec3[]{
      {1, 0, 0},
      {0, 1, 0},
      {0, 0, 1},
      {1, 1, 0},
  };
  GLuint buffers[4];
  glCreateBuffers(4, buffers);
  auto [vbPosition, vbNormal, vbColor, ib] = buffers;
  const GLsizeiptr bufferSizeBytes = tetrahedron.numVertices * sizeof(Vec3);
  glNamedBufferStorage(vbPosition, bufferSizeBytes, tetrahedron.positions, GL_DYNAMIC_STORAGE_BIT);
  glNamedBufferStorage(vbColor, bufferSizeBytes, tetrahedron.colors, GL_DYNAMIC_STORAGE_BIT);
  const GLsizeiptr indexSizeBytes = tetrahedron.numIndices * sizeof(uint32_t);
  glNamedBufferStorage(ib, indexSizeBytes, tetrahedron.indices, 0);

  const GLuint vertexShader = createShaderFromSPIRVHeader(GL_VERTEX_SHADER, vertex_shader_bin, sizeof(vertex_shader_bin));
  const GLuint fragmentShader = createShaderFromSPIRVHeader(GL_FRAGMENT_SHADER, fragment_shader_bin, sizeof(fragment_shader_bin));
  const GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  uint32_t vertexArray;
  const uint32_t posAttr = 0;
  const uint32_t colAttr = 1;
  glCreateVertexArrays(1, &vertexArray);
  glEnableVertexArrayAttrib(vertexArray, posAttr);  // position
  glEnableVertexArrayAttrib(vertexArray, colAttr);  // color
  glVertexArrayAttribFormat(vertexArray, posAttr, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribFormat(vertexArray, colAttr, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayVertexBuffer(vertexArray, posAttr, vbPosition, 0, sizeof(Vec3));
  glVertexArrayVertexBuffer(vertexArray, colAttr, vbColor, 0, sizeof(Vec3));
  glVertexArrayElementBuffer(vertexArray, ib);
  glVertexArrayAttribBinding(vertexArray, posAttr, 0);
  glVertexArrayAttribBinding(vertexArray, colAttr, 1);

  while (!windows.getKeyState(0x1B /*VK_ESCAPE*/)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(vertexArray);
    glDrawElements(GL_TRIANGLES, tetrahedron.numIndices, GL_UNSIGNED_INT, 0);

    windows.swapBuffers();
  }

  windows.exit();
}