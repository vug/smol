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

inline float smolSin(float angle) {
  float result;
  __asm {
        fld angle  // Load angle onto FPU stack
        fsin  // Compute sine
        fstp result  // Store result and pop FPU stack
  }
  return result;
}

inline float smolCos(float angle) {
  float result;
  __asm {
        fld angle  // Load angle onto FPU stack
        fcos  // Compute cosine
        fstp result  // Store result and pop FPU stack
  }
  return result;
}

inline float smolTan(float angle) {
  float result;
  __asm {
        fld angle  // Load angle onto FPU stack
        fptan  // Compute tangent, pushes 1.0 and then tan(angle)
        fstp result  // Pop the 1.0 that fptan pushes
        fstp result  // Store actual result and pop FPU stack
  }
  return result;
}

inline float smolAtan(float value) {
  float result;
  __asm {
        fld value  // Load value onto FPU stack
        fld1  // Load 1.0
        fpatan  // Compute arctangent (takes ratio y/x, here it's value/1.0)
        fstp result  // Store result and pop FPU stack
  }
  return result;
}

inline float smolAtan2(float y, float x) {
  float result;
  __asm {
        fld y  // Load y onto FPU stack
        fld x  // Load x onto FPU stack
        fpatan  // Compute arctangent(y/x)
        fstp result  // Store result and pop FPU stack
  }
  return result;
}

}  // namespace smol