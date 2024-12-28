#pragma once

namespace smol {

inline constexpr float sqrt(float value) {
  float result;
  __asm {
    movss xmm0, value    ; Load the input value into the XMM0 register
    sqrtss xmm0, xmm0    ; Compute the square root of the value in XMM0
    movss result, xmm0   ; Store the result back to memory
  }
  return result;
}

inline float sin(float angle) {
  float result;
  __asm {
        fld angle  // Load angle onto FPU stack
        fsin  // Compute sine
        fstp result  // Store result and pop FPU stack
  }
  return result;
}

inline float cos(float angle) {
  float result;
  __asm {
        fld angle  // Load angle onto FPU stack
        fcos  // Compute cosine
        fstp result  // Store result and pop FPU stack
  }
  return result;
}

inline float tan(float angle) {
  float result;
  __asm {
        fld angle  // Load angle onto FPU stack
        fptan  // Compute tangent, pushes 1.0 and then tan(angle)
        fstp result  // Pop the 1.0 that fptan pushes
        fstp result  // Store actual result and pop FPU stack
  }
  return result;
}

inline float atan(float value) {
  float result;
  __asm {
        fld value  // Load value onto FPU stack
        fld1  // Load 1.0
        fpatan  // Compute arctangent (takes ratio y/x, here it's value/1.0)
        fstp result  // Store result and pop FPU stack
  }
  return result;
}

inline float atan2(float y, float x) {
  float result;
  __asm {
        fld y  // Load y onto FPU stack
        fld x  // Load x onto FPU stack
        fpatan  // Compute arctangent(y/x)
        fstp result  // Store result and pop FPU stack
  }
  return result;
}

struct Vec3 {
  float x{};
  float y{};
  float z{};

  inline Vec3& normalize() {
    float len = sqrt(x * x + y * y + z * z);
    x /= len;
    y /= len;
    z /= len;
    return *this;
  }
};

struct Mat4 {
  float m[4][4];
};

inline constexpr float dot(Vec3 u, Vec3 v) {
  return u.x * v.x + u.y * u.y + u.z * u.z;
}

inline constexpr Vec3 cross(Vec3 u, Vec3 v) {
  return Vec3{u.y * v.z - u.z * v.y, -(u.x * v.z - u.z * v.x),
              u.x * v.y - u.y * v.x};
}

Vec3 operator-(const Vec3& u, const Vec3& v) {
  return Vec3{u.x - v.x, u.y - v.y, u.z - v.z};
}

Mat4 lookAt(Vec3 eye, Vec3 target, Vec3 up) {
  // Calculate camera coordinate system
  const Vec3 zaxis = (eye - target).normalize();  // Camera looks down -z
  const Vec3 xaxis = cross(up, zaxis).normalize();
  const Vec3 yaxis = cross(zaxis, xaxis);  // Already normalized

  // Create view matrix
  Mat4 view;

  // Rotation part
  view.m[0][0] = xaxis.x; view.m[0][1] = xaxis.y; view.m[0][2] = xaxis.z;
  view.m[1][0] = yaxis.x; view.m[1][1] = yaxis.y; view.m[1][2] = yaxis.z;
  view.m[2][0] = zaxis.x; view.m[2][1] = zaxis.y; view.m[2][2] = zaxis.z;

  // Translation part
  view.m[0][3] = -dot(xaxis, eye);
  view.m[1][3] = -dot(yaxis, eye);
  view.m[2][3] = -dot(zaxis, eye);

  return view;
}

Mat4 perspective(float fovY, float aspect, float nearZ, float farZ) {
  Mat4 proj;
    
  // Clear the matrix first (set all elements to 0)
  for(int i = 0; i < 4; i++)
      for(int j = 0; j < 4; j++)
          proj.m[i][j] = 0.0f;
    
  float tanHalfFovy = tan(fovY / 2);
    
  proj.m[0][0] = 1.0f / (aspect * tanHalfFovy); // Scale x
  proj.m[1][1] = 1.0f / tanHalfFovy;            // Scale y
  proj.m[2][2] = -(farZ + nearZ) / (farZ - nearZ);  // Scale and map z
  proj.m[2][3] = -(2.0f * farZ * nearZ) / (farZ - nearZ);
  proj.m[3][2] = -1.0f;  // This makes it a perspective projection
    
  return proj;
}

}  // namespace smol