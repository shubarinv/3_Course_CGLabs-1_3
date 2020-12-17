//
// Created by Vladimir Shubarin on 12.10.2020.
//

#ifndef CGLABS__INDEX_BUFFER_HPP_
#define CGLABS__INDEX_BUFFER_HPP_

#include <vector>
#include "../vertex.hpp"
#include "buffer.hpp"
#include "../functions.hpp"
class IndexBuffer : public Buffer {
 private:
  unsigned int length{};
  std::vector<unsigned int> indices;
 public:
  [[nodiscard]] const std::vector<unsigned int> &getIndices() const {
	return indices;
  }

 public:
  [[nodiscard]] unsigned int getLength() const {
	return length;
  }

 public:
  explicit IndexBuffer(std::vector<unsigned int> _indices) {
	glCall(glGenBuffers(1, &rendererID));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW));
	length  = _indices.size();
	indices = _indices;

	LOG_S(INFO) << "IndexBuffer created rendererID: " << rendererID;
  }
  [[deprecated]][[maybe_unused]] explicit IndexBuffer(const void *data, unsigned int size) {
	glCall(glGenBuffers(1, &rendererID));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), data, GL_STATIC_DRAW));
	length = size;
	LOG_S(INFO) << "IndexBuffer created rendererID: " << rendererID;
  }
  explicit IndexBuffer(const std::vector<Vertex> &vertices, bool quads = true) {
	std::vector<unsigned int> _indices;

	for (unsigned int i = 0; i < vertices.size(); i++) {

	  _indices.push_back(i);

	  if (_indices.size() % 3 == 0) {
		_indices.push_back(i);
	  }
	}
	if (quads) {
	  for (unsigned int i = 1; i < vertices.size(); i++) {

		_indices.push_back(i);

		if (_indices.size() % 3 == 0) {
		  _indices.push_back(i);
		}
	  }
	}
	indices = _indices;
	glCall(glGenBuffers(1, &rendererID));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), _indices.data(), GL_STATIC_DRAW));
	length = _indices.size();
	LOG_S(INFO) << "IndexBuffer created rendererID: " << rendererID;
  }
  ~IndexBuffer() {
	glCall(glDeleteBuffers(1, &rendererID));
	LOG_S(INFO) << "IndexBuffer destroyed rendererID: " << rendererID;
  }
  void bind() const override {
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
  }
  [[maybe_unused]] static void unbind() {
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  }
};

#endif//CGLABS__INDEX_BUFFER_HPP_
