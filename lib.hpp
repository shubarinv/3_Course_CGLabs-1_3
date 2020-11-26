//
// Created by Vladimir Shubarin on 13.10.2020.
//

#ifndef CGLABS__LIB_HPP_
#define CGLABS__LIB_HPP_
#if defined (__APPLE__)
#define ASSERT(X) \
  if (!(X)) __builtin_debugtrap();
#endif

#if defined (__WIN32__)
#define ASSERT(X) \
  if (!(X)) __debugbreak()
#endif

#define glCall(x)  \
  glClearErrors(); \
  x;               \
  ASSERT(glLogCall(#x, __FILE__, __LINE__))

#if defined (__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <spdlog/spdlog.h>
#endif
#if defined (__WIN32__)
#include "spdlogDummy.hpp"
#endif

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

bool isWindows() {
#if defined (__WIN32__)
  return true;
#endif
  return false;
}
bool isMac() {
#if defined (__APPLE__)
    return true;
#endif
    return false;

}
bool isLinux() {
#if defined (__unix__)
    return true;
#endif
    return false;
}

bool glLogCall(const char *function, const char *file, int line) {
  while (GLenum error = glGetError()) {
	spdlog::error("OpenGL error: {} in file {} in function {} at line {}", (error), file, function, line);
  }
  return true;
}

void glClearErrors() {
  while (glGetError() != GL_NO_ERROR);
}


#endif//CGLABS__LIB_HPP_
