//
// Created by Vladimir Shubarin on 12.10.2020.
//

#ifndef CGLABS__VERTEX_BUFFER_HPP_
#define CGLABS__VERTEX_BUFFER_HPP_

#include "lib.hpp"
#include "vertex.hpp"
class VertexBuffer : public Buffer {
 private:
  std::vector<Vertex> vertices_vec;

 public:
  [[nodiscard]] const std::vector<Vertex> &getVertices() const {
	return vertices_vec;
  }

 public:
  explicit VertexBuffer(const std::vector<Vertex> &_vertices) {
	vertices_vec = _vertices;
	std::vector<float> vertices;
	for (auto &vector : _vertices) {
	  vertices.push_back(vector.getPosition().x);
	  vertices.push_back(vector.getPosition().y);
	  vertices.push_back(vector.getPosition().z);
	}
	glCall(glGenBuffers(1, &rendererID));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	glCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW));
	spdlog::info("VertexBuffer(Vertex) created rendererID: {}", rendererID);
  }

  [[deprecated]] [[maybe_unused]] VertexBuffer(const void *data, unsigned int size) {
	glCall(glGenBuffers(1, &rendererID));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	spdlog::info("VertexBuffer(General) created rendererID: {}", rendererID);
  }

  ~VertexBuffer() {
	glCall(glDeleteBuffers(1, &rendererID));
	spdlog::info("VertexBuffer destroyed rendererID: {}", rendererID);
  }
  void bind() const override {
	glCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
  }
  [[maybe_unused]] static void unbind() {
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  }
};

#endif//CGLABS__VERTEX_BUFFER_HPP_
