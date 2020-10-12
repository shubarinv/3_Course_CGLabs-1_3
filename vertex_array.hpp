//
// Created by Vladimir Shubarin on 12.10.2020.
//

#ifndef CGLABS__VERTEX_ARRAY_HPP_
#define CGLABS__VERTEX_ARRAY_HPP_

#include "vertex_buffer.hpp"
class VertexArray {
 public:
  void AddBuffer(VertexBuffer buffer);
  void addLayout(BufferLayout layout) {
  }
};

#endif//CGLABS__VERTEX_ARRAY_HPP_
