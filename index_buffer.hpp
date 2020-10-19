//
// Created by Vladimir Shubarin on 12.10.2020.
//

#ifndef CGLABS__INDEX_BUFFER_HPP_
#define CGLABS__INDEX_BUFFER_HPP_

#include "buffer.hpp"
#include "lib.hpp"
class IndexBuffer : public Buffer {
 private:
  unsigned int length{};

 public:
  [[nodiscard]] unsigned int getLength() const {
	return length;
  }

 public:
  explicit IndexBuffer(std::vector<unsigned int> indices) {
	glCall(glGenBuffers(1, &rendererID));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));
	length = indices.size();
	spdlog::info("IndexBuffer created rendererID: {}", rendererID);
  }
  ~IndexBuffer() {
	glCall(glDeleteBuffers(1, &rendererID));
	spdlog::info("IndexBuffer destroyed rendererID: {}", rendererID);
  }
  void bind() const override {
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
  }
  [[maybe_unused]] static void unbind() {
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  }
};

#endif//CGLABS__INDEX_BUFFER_HPP_
