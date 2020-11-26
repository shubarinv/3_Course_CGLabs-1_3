//
// Created by Vladimir Shubarin on 13.10.2020.
//

#ifndef CGLABS__LIB_HPP_
#define CGLABS__LIB_HPP_
#define ASSERT(X) \
  if (!(X)) __builtin_debugtrap()
#define glCall(x)  \
  glClearErrors(); \
  x;               \
  ASSERT(glLogCall(#x, __FILE__, __LINE__))

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#if defined (__APPLE__)
#include <OpenGL/glu.h>
#endif
#if defined (__WIN32__)
#include <GL/glu.h>
#endif
#include <OpenGL/opengl.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

bool glLogCall(const char *function, const char *file, int line) {
  while (GLenum error = glGetError()) {
	spdlog::error("OpenGL error: {} in file {} in function {} at line {}", gluErrorString(error), file, function, line);
  }
  return true;
}

void glClearErrors() {
  while (glGetError() != GL_NO_ERROR);
}

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
bool is
#endif//CGLABS__LIB_HPP_
