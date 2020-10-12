//
// Created by Vladimir Shubarin on 12.10.2020.
//

#ifndef CGLABS__RENDERER_HPP_
#define CGLABS__RENDERER_HPP_
#define ASSERT(X) \
  if (!(X)) __builtin_debugtrap()
#define glCall(x)  \
  glClearErrors(); \
  x;               \
  ASSERT(glLogCall(#x, __FILE__, __LINE__))

#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <OpenGL/opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

#include <string>

bool glLogCall(const char *function, const char *file, int line) {
  while (GLenum error = glGetError()) {
	spdlog::error("OpenGL error: {} in file {} in function {} at line {}", gluErrorString(error), file, function, line);
  }
  return true;
}

void glClearErrors() {
  while (glGetError() != GL_NO_ERROR)
	;
}
class Renderer {
};

#endif//CGLABS__RENDERER_HPP_
