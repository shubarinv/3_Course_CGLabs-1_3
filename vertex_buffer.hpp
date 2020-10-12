//
// Created by Vladimir Shubarin on 12.10.2020.
//

#ifndef CGLABS__VERTEX_BUFFER_HPP_
#define CGLABS__VERTEX_BUFFER_HPP_

#include "renderer.hpp"
class VertexBuffer {
 private:
  unsigned int rendererID{};

 public:
  VertexBuffer(const void *data, unsigned int size) {
	glCall(glGenBuffers(1, &rendererID));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
  }
  ~VertexBuffer() {
	glCall(glDeleteBuffers(1, &rendererID));
  }
  void bind() const {
	glCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
  }
  static void unbind() {
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  }
};

#endif//CGLABS__VERTEX_BUFFER_HPP_
