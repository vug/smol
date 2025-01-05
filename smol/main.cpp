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

struct UniformData {
  Mat4 view;
  Mat4 projection;
};

struct Mesh {
  Vec3* positions{};
  uint32_t numVertices{};
  uint32_t* indices{};
  uint32_t numIndices{};
  Vec3* normals{};
  Vec3* colors{};
};

constexpr Mesh generateQuad(const float size = 1.0f,
                            const Vec3& offset = {0, 0, 0}) {
  Mesh mesh;
  mesh.positions =
      new Vec3[]{{-0.5f * size + offset.x, -0.5f * size + offset.y, offset.z},
                 {0.5f * size + offset.x, -0.5f * size + offset.y, offset.z},
                 {0.5f * size + offset.x, 0.5f * size + offset.y, offset.z},
                 {-0.5f * size + offset.x, 0.5f * size + offset.y, offset.z}};
  mesh.numVertices = 4;
  mesh.indices = new uint32_t[]{0, 1, 2, 0, 2, 3};
  mesh.numIndices = 6;

  mesh.colors = new Vec3[]{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 0, 1}};
  mesh.normals = new Vec3[]{{0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}};
  return mesh;
}

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

  mesh.colors = new Vec3[]{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 0, 1}};

  return mesh;
}

constexpr Mesh generateTetrahedron2(const float size = 1.0f) {
  Mesh mesh;

  const float a = size * 1.0f / smol::sqrt(3.0f);
  const float b = size * smol::sqrt(8.0f / 9.0f);
  const float c = size * -1.0f / 3.0f;

  const Vec3 v0{0, a, 0};
  const Vec3 v1{-size / 2, c, -b / 3};
  const Vec3 v2{size / 2, c, -b / 3};
  const Vec3 v3{0, c, 2 * b / 3};
  mesh.positions = new Vec3[]{
    v0, v2, v1, // Front face
    v0, v3, v2, // Right face
    v0, v1, v3, // Left face
    v1, v2, v3, // Bottom face
  };
  mesh.numVertices = 12;

  const Vec3 c0{1, 0, 0};
  const Vec3 c1{0, 1, 0};
  const Vec3 c2{0, 0, 1};
  const Vec3 c3{1, 0, 01};
  mesh.colors = new Vec3[]{
      c0, c2, c1,  // Front face
      c0, c3, c2,  // Right face
      c0, c1, c3,  // Left face
      c1, c2, c3,  // Bottom face
  };

  mesh.indices = new uint32_t[12];
  for (uint32_t ix = 0; ix < 12; ix++) {
    mesh.indices[ix] = ix;
  }
  mesh.numIndices = 12;

  return mesh;
}

void iterateIcoSphere(const Vec3* inPositions, const uint32_t inNumPositions, Vec3* outPositions, uint32_t& outNumPositions) {
  const uint32_t inNumTriangles = inNumPositions / 3;
  outNumPositions = inNumPositions * 4;
  outPositions = new Vec3[outNumPositions];

  for (uint32_t inTriIx = 0; inTriIx < inNumTriangles; ++inTriIx) {
    uint32_t vertIx = inTriIx * 3;
    const Vec3& v0 = inPositions[vertIx + 0];
    const Vec3& v1 = inPositions[vertIx + 1];
    const Vec3& v2 = inPositions[vertIx + 2];

    const Vec3 m01 = (v0 + v1).scale(0.5f);
    const Vec3 m20 = (v0 + v2).scale(0.5f);
    const Vec3 m12 = (v1 + v2).scale(0.5f);

    const uint32_t outTriIx = inTriIx * 4;
    const uint32_t outVertIx = outTriIx * 3;
    // out triangle 0
    outPositions[outVertIx +  0] = v0;
    outPositions[outVertIx +  1] = m01;
    outPositions[outVertIx +  2] = m20;
    // out triangle 1
    outPositions[outVertIx +  3] = m01;
    outPositions[outVertIx +  4] = v1;
    outPositions[outVertIx +  5] = m12;
    // out triangle 2
    outPositions[outVertIx +  6] = m20;
    outPositions[outVertIx +  7] = m12;
    outPositions[outVertIx +  8] = v2;
    // out triangle 3
    outPositions[outVertIx +  9] = m01;
    outPositions[outVertIx + 10] = m12;
    outPositions[outVertIx + 11] = m20;
  }
}

int main() {
  Windows windows(1024, 768);

#ifdef _DEBUG
  std::println("version: {}", (const char*)glGetString(GL_VERSION));
#endif

  //Mesh mesh = generateQuad(1.0f, {0, 0, 0});    // Center quad
  Mesh mesh = generateTetrahedron2();
  GLuint buffers[5];
  glCreateBuffers(5, buffers);
  auto [vbPosition, vbNormal, vbColor, ib, ub] = buffers;
  const GLsizeiptr bufferSizeBytes = mesh.numVertices * sizeof(Vec3);
  glNamedBufferStorage(vbPosition, bufferSizeBytes, mesh.positions, GL_DYNAMIC_STORAGE_BIT);
  glNamedBufferStorage(vbColor, bufferSizeBytes, mesh.colors, GL_DYNAMIC_STORAGE_BIT);
  const GLsizeiptr indexSizeBytes = mesh.numIndices * sizeof(uint32_t);
  glNamedBufferStorage(ib, indexSizeBytes, mesh.indices, 0);
  glNamedBufferStorage(ub, sizeof(UniformData), nullptr, GL_DYNAMIC_STORAGE_BIT);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, ub);

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

  UniformData uniforms;
  uniforms.view = Mat4{};
  uniforms.view = lookAt({1, 4, 2}, {0, 0, 0}, {0, 1, 0});
  uniforms.projection = Mat4{};
  const float fovY = 45.0f * 3.14159f / 180.0f;  // Convert to radians
  const float aspect = 1024.f / 768.f;
  uniforms.projection = perspective(fovY, aspect, 0.1f, 100.0f);

  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  while (!windows.getKeyState(0x1B /*VK_ESCAPE*/)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 1024, 768);

    glUseProgram(program);
    glNamedBufferSubData(ub, 0, sizeof(UniformData), &uniforms);
    glBindVertexArray(vertexArray);
    glDrawElements(GL_TRIANGLES, mesh.numIndices, GL_UNSIGNED_INT, 0);

    windows.swapBuffers();
  }

  windows.exit();
}