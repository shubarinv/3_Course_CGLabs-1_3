//
// Created by Vladimir Shubarin on 12.10.2020.
//

#ifndef CGLABS__INDEX_BUFFER_HPP_
#define CGLABS__INDEX_BUFFER_HPP_

#include "renderer.hpp"
class IndexBuffer {
 private:
  unsigned int rendererID{};
  unsigned int length{};

 public:
  [[nodiscard]] unsigned int getLength() const {
	return length;
  }

 public:
  IndexBuffer(const unsigned int *data, unsigned int _length) {
	glCall(glGenBuffers(1, &rendererID));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _length * sizeof(unsigned int), data, GL_STATIC_DRAW));
  }
  ~IndexBuffer() {
	glCall(glDeleteBuffers(1, &rendererID));
  }
  void bind() const {
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
  }
  static void unbind() {
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  }
};

#endif//CGLABS__INDEX_BUFFER_HPP_
