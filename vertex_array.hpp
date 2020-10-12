//
// Created by Vladimir Shubarin on 12.10.2020.
//

#ifndef CGLABS__VERTEX_ARRAY_HPP_
#define CGLABS__VERTEX_ARRAY_HPP_

#include "vertex_buffer.hpp"
#include "vertex_buffer_layout.hpp"
class VertexArray {
 private:
  unsigned int rendererID{};

 public:
  VertexArray() {
	glCall(glGenVertexArrays(1, &rendererID));
	spdlog::info("VertexArray created rendererID: {}", rendererID);
  }
  ~VertexArray() {
	glCall(glDeleteVertexArrays(1, &rendererID));
	spdlog::info("VertexArray destroyed rendererID: {}", rendererID);
  }
  void bind() const {
	glCall(glBindVertexArray(rendererID));
  }
  static void unbind() {
	glCall(glBindVertexArray(0));
  }
  void addBuffer(const VertexBuffer &buffer, const VertexBufferLayout &layout) const {
	bind();
	buffer.bind();
	const auto &elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
	  const auto &element = elements[i];
	  glCall(glEnableVertexAttribArray(i));
	  glCall(glVertexAttribPointer(i, element.length, element.type, element.normalized,
								   layout.getStride(), (const void *)offset));
	  offset += element.length * VertexBufferElement::getSize(element.type);
	  glCall(glEnableVertexAttribArray(0));
	}
  }
  void addLayout(VertexBufferElement layout) {
  }
};

#endif//CGLABS__VERTEX_ARRAY_HPP_
