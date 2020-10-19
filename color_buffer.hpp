//
// Created by Vladimir Shubarin on 19.10.2020.
//

#ifndef CGLABS__COLOR_BUFFER_HPP_
#define CGLABS__COLOR_BUFFER_HPP_

#include <vector>

#include "buffer.hpp"
#include "lib.hpp"
#include "vertex.hpp"
class ColorBuffer : public Buffer {

 public:
  explicit ColorBuffer(const std::vector<Vertex> &_vertices) {
	std::vector<GLfloat> vertices;
	for (auto &vector : _vertices) {
	  vertices.push_back(vector.getColor().r);
	  vertices.push_back(vector.getColor().g);
	  vertices.push_back(vector.getColor().b);
	}
	glCall(glGenBuffers(1, &rendererID));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	glCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW));
	spdlog::info("ColorBuffer created, rendererID: {}", rendererID);
  }

  ~ColorBuffer() {
	glCall(glDeleteBuffers(1, &rendererID));
	spdlog::info("ColorBuffer destroyed rendererID: {}", rendererID);
  }
};

#endif//CGLABS__COLOR_BUFFER_HPP_
