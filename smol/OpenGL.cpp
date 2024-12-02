#include "OpenGL.hpp"

#define DEFINE_FUNC_PTR_TYPE(method) FnPtrT(method) method{};

DEFINE_FUNC_PTR_TYPE(wglCreateContextAttribsARB);
DEFINE_FUNC_PTR_TYPE(wglChoosePixelFormatARB);
//
DEFINE_FUNC_PTR_TYPE(glClearColor);
DEFINE_FUNC_PTR_TYPE(glClear);
DEFINE_FUNC_PTR_TYPE(glGetString);
DEFINE_FUNC_PTR_TYPE(glCreateShader);
DEFINE_FUNC_PTR_TYPE(glShaderSource);
DEFINE_FUNC_PTR_TYPE(glCompileShader);
DEFINE_FUNC_PTR_TYPE(glGetShaderiv);
DEFINE_FUNC_PTR_TYPE(glCreateProgram);
DEFINE_FUNC_PTR_TYPE(glAttachShader);
DEFINE_FUNC_PTR_TYPE(glLinkProgram);
DEFINE_FUNC_PTR_TYPE(glGetProgramiv);
DEFINE_FUNC_PTR_TYPE(glGetShaderInfoLog);
DEFINE_FUNC_PTR_TYPE(glGetProgramInfoLog);
DEFINE_FUNC_PTR_TYPE(glGenVertexArrays);
DEFINE_FUNC_PTR_TYPE(glUseProgram);
DEFINE_FUNC_PTR_TYPE(glBindVertexArray);
DEFINE_FUNC_PTR_TYPE(glDrawArrays);
DEFINE_FUNC_PTR_TYPE(glCreateBuffers);
DEFINE_FUNC_PTR_TYPE(glDeleteBuffers);
DEFINE_FUNC_PTR_TYPE(glBindBuffer);
DEFINE_FUNC_PTR_TYPE(glBufferData);
DEFINE_FUNC_PTR_TYPE(glCreateVertexArrays);
DEFINE_FUNC_PTR_TYPE(glDeleteVertexArrays);
DEFINE_FUNC_PTR_TYPE(glEnableVertexAttribArray);
DEFINE_FUNC_PTR_TYPE(glVertexAttribPointer);
DEFINE_FUNC_PTR_TYPE(glDrawElements);
DEFINE_FUNC_PTR_TYPE(glEnable);
DEFINE_FUNC_PTR_TYPE(glDepthFunc);
DEFINE_FUNC_PTR_TYPE(glGetUniformLocation);
DEFINE_FUNC_PTR_TYPE(glUniform1f);
DEFINE_FUNC_PTR_TYPE(glUniformMatrix4fv);
DEFINE_FUNC_PTR_TYPE(glBindBufferBase);
DEFINE_FUNC_PTR_TYPE(glGenTextures);
DEFINE_FUNC_PTR_TYPE(glBindTexture);
DEFINE_FUNC_PTR_TYPE(glTexImage2D);
DEFINE_FUNC_PTR_TYPE(glGenFramebuffers);
DEFINE_FUNC_PTR_TYPE(glBindFramebuffer);
DEFINE_FUNC_PTR_TYPE(glFramebufferTexture2D);
DEFINE_FUNC_PTR_TYPE(glViewport);
DEFINE_FUNC_PTR_TYPE(glReadPixels);
DEFINE_FUNC_PTR_TYPE(glCheckFramebufferStatus);
DEFINE_FUNC_PTR_TYPE(glBufferSubData);
DEFINE_FUNC_PTR_TYPE(glScissor);

//#define GET_PROC_ADDRESS(method)                           \
//  method = (FnPtrT(method))GetAnyGLFuncAddress(#method);   \
//  if (!method) {                                           \
//    fatal("Failed to initialize OpenGL function #method"); \
//  }