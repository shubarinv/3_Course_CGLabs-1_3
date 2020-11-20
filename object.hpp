//
// Created by Vladimir Shubarin on 16.11.2020.
//

#ifndef CGLABS__OBJECT_HPP_
#define CGLABS__OBJECT_HPP_

#include <utility>

#include "Buffers/index_buffer.hpp"
#include "renderer.hpp"
#include "Buffers/color_buffer.hpp"
class Object {
 protected:
  glm::vec3 color{};
  IndexBuffer *indexBuffer{};
  VertexBuffer *vertexBuffer{};
  VertexArray *vertexArray{};
  ColorBuffer *colorBuffer{};
  VertexBufferLayout *bufferLayout{};
  unsigned int layoutLength{3};
  int timesToPushLayout = {1};
  bool bInitialized{false};
 public:
  [[nodiscard]] IndexBuffer *getIndexBuffer() const {
	if (bInitialized)
	  return indexBuffer;
	else throw std::runtime_error("Object is not initialized!");
  }
  [[nodiscard]] VertexArray *getVertexArray() const {
	if (bInitialized)
	  return vertexArray;
	else throw std::runtime_error("Object is not initialized!");
  }
  void setIndexBuffer(IndexBuffer *_indexBuffer) {
	indexBuffer = _indexBuffer;
  }
  void setIndexBuffer(std::vector<unsigned int> _indices) {
	indexBuffer = new IndexBuffer(std::move(_indices));
  }
  void setIndexBuffer(const std::vector<Vertex> &_vertices) {
	indexBuffer = new IndexBuffer(_vertices);
  }
  void setVertexBuffer(VertexBuffer _vertexBuffer) {
	vertexBuffer = &_vertexBuffer;
  }
  void setVertexBuffer(const void *data, unsigned int size) {
	vertexBuffer = new VertexBuffer(data, size);
  }
  void setVertexBuffer(const std::vector<Vertex> &vertices) {
	colorBuffer = new ColorBuffer(vertices);
	vertexBuffer = new VertexBuffer(vertices);
  }
  void setLayoutLength(unsigned int length) {
	layoutLength = length;
  }
  void setTimesToPushLayout(int times) {
	timesToPushLayout = times;
  }

//todo  void setColorBuffer(const std::vector<glm::vec3> &_colors) {}
  void setColorBuffer(const std::vector<Vertex> &_verticesWithColors) {
	colorBuffer = new ColorBuffer(_verticesWithColors);
  }
  void setColor(glm::vec3 _color) {
	color = _color;
  }
  Object() {
	bInitialized = false;
  }
  void init() {
	if (vertexBuffer == nullptr) {
	  throw std::runtime_error("Object init failed");
	}
	bufferLayout = new VertexBufferLayout();
	for (int i = 0; i < timesToPushLayout; i++) {
	  bufferLayout->push<float>(layoutLength);
	}
	vertexArray = new VertexArray();
	vertexArray->addBuffer(*vertexBuffer, *bufferLayout);
	if (colorBuffer != nullptr) {
	  vertexArray->addBuffer(*colorBuffer, *bufferLayout, 1);
	}
	bInitialized = true;
  }

};

#endif //CGLABS__OBJECT_HPP_
