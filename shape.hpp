//
// Created by Vladimir Shubarin on 16.11.2020.
//

#ifndef CGLABS__SHAPE_HPP_
#define CGLABS__SHAPE_HPP_

#include "index_buffer.hpp"
#include "renderer.hpp"
class Shape {
 protected:
  glm::vec3 color;
  IndexBuffer *indexBuffer{};
  VertexBuffer *vertexBuffer{};
  VertexArray *vertexArray{};
  ColorBuffer *colorBuffer{};
  VertexBufferLayout *bufferLayout{};
 public:
  [[nodiscard]] IndexBuffer *getIndexBuffer() const {
	return indexBuffer;
  }
  [[nodiscard]] VertexArray *getVertexArray() const {
	return vertexArray;
  }
};

#endif //CGLABS__SHAPE_HPP_
