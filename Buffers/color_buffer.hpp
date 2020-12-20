//
// Created by Vladimir Shubarin on 19.10.2020.
//

#ifndef CGLABS__COLOR_BUFFER_HPP_
#define CGLABS__COLOR_BUFFER_HPP_

#include <vector>

#include "buffer.hpp"
#include "../functions.hpp"
#include "../vertex.hpp"
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
	LOG_S(INFO) << "ColorBuffer created rendererID: " << rendererID;
  }
  explicit ColorBuffer(const std::vector<glm::vec3> &_vertices) {
	std::vector<GLfloat> vertices;
	for (auto &vector : _vertices) {
	  vertices.push_back(vector.r);
	  vertices.push_back(vector.g);
	  vertices.push_back(vector.b);
	}
	glCall(glGenBuffers(1, &rendererID));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
	glCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW));
	LOG_S(INFO) << "ColorBuffer created rendererID: " << rendererID;
  }

  ~ColorBuffer() {
	glCall(glDeleteBuffers(1, &rendererID));
	LOG_S(INFO) << "ColorBuffer destroyed rendererID: " << rendererID;
  }
  void bind() const override {
	glCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
  }
  [[maybe_unused]] static void unbind() {
	glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  }
};

#endif//CGLABS__COLOR_BUFFER_HPP_
