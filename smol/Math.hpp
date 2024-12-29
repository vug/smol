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

inline constexpr float sin(float angle) {
  float result;
  __asm {
        fld angle  // Load angle onto FPU stack
        fsin  // Compute sine
        fstp result  // Store result and pop FPU stack
  }
  return result;
}

inline constexpr float cos(float angle) {
  float result;
  __asm {
        fld angle  // Load angle onto FPU stack
        fcos  // Compute cosine
        fstp result  // Store result and pop FPU stack
  }
  return result;
}

inline constexpr float tan(float angle) {
  float result;
  __asm {
        fld angle  // Load angle onto FPU stack
        fptan  // Compute tangent, pushes 1.0 and then tan(angle)
        fstp result  // Pop the 1.0 that fptan pushes
        fstp result  // Store actual result and pop FPU stack
  }
  return result;
}

inline constexpr float atan(float value) {
  float result;
  __asm {
        fld value  // Load value onto FPU stack
        fld1  // Load 1.0
        fpatan  // Compute arctangent (takes ratio y/x, here it's value/1.0)
        fstp result  // Store result and pop FPU stack
  }
  return result;
}

inline constexpr float atan2(float y, float x) {
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

  constexpr inline Vec3& normalize() {
    float len = sqrt(x * x + y * y + z * z);
    x /= len;
    y /= len;
    z /= len;
    return *this;
  }
};

struct Mat4 {
  float m[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
};

inline constexpr float dot(Vec3 u, Vec3 v) {
  return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline constexpr Vec3 cross(Vec3 u, Vec3 v) {
  return Vec3{u.y * v.z - u.z * v.y, -(u.x * v.z - u.z * v.x),
              u.x * v.y - u.y * v.x};
}

inline constexpr Vec3 operator-(const Vec3& u, const Vec3& v) {
  return Vec3{u.x - v.x, u.y - v.y, u.z - v.z};
}


// from https://github.com/HandmadeMath/HandmadeMath HMM_LookAt_RH
inline constexpr Mat4 lookAt(Vec3 eye, Vec3 target, Vec3 up) {
  Mat4 view;

  const Vec3 zForward = (target - eye).normalize();
  const Vec3 xRight = cross(zForward, up).normalize();
  const Vec3 yUp = cross(xRight, zForward);

  view.m[0][0] = xRight.x;
  view.m[1][0] = xRight.y;
  view.m[2][0] = xRight.z;
  view.m[3][0] = -dot(xRight, eye);

  view.m[0][1] = yUp.x;
  view.m[1][1] = yUp.y;
  view.m[2][1] = yUp.z;
  view.m[3][1] = -dot(yUp, eye);

  view.m[0][2] = -zForward.x;
  view.m[1][2] = -zForward.y;
  view.m[2][2] = -zForward.z;
  view.m[3][2] = dot(zForward, eye);
  
  view.m[0][3] = 0.0f;
  view.m[1][3] = 0.0f;
  view.m[2][3] = 0.0f;
  view.m[3][3] = 1.0f;

  return view;
}

// from https://github.com/HandmadeMath/HandmadeMath HMM_Perspective_RH_ZO
inline constexpr Mat4 perspective([[maybe_unused]] float fovY, [[maybe_unused]] float aspect, [[maybe_unused]] float nearZ, [[maybe_unused]] float farZ) {
  Mat4 proj = {};

  float cotangent = 1.0f / tan(fovY / 2.0f);
  proj.m[0][0] = cotangent / aspect;
  proj.m[1][1] = cotangent;
  proj.m[2][3] = -1.0f;

  proj.m[2][2] = (nearZ + farZ) / (nearZ - farZ);
  proj.m[3][2] = (2.0f * nearZ * farZ) / (nearZ - farZ);
  proj.m[3][3] = 0;

  return proj;
}

}  // namespace smol