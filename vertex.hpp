//
// Created by Vladimir Shubarin on 03.12.2020.
//

#ifndef CGLABS__VERTEX_HPP_
#define CGLABS__VERTEX_HPP_
#include "functions.hpp"
class Vertex {
  glm::vec3 position{};
  glm::vec3 color{};
  glm::vec3 normal{};
 public:
  Vertex() {
	PLOGV << "Created Vertex(" << this << ") with unset params";
  }

  [[maybe_unused]] explicit Vertex(glm::vec3 _position) {
	PLOGV << "Created Vertex(" << this << ") with position x:" << _position.x << " y:" << _position.y << " z:" << _position.z;

	position = _position;
  }
  [[maybe_unused]] Vertex(glm::vec3 _position, glm::vec3 _color) {
	PLOGV << "Created Vertex(" << this << ") with position x:" << _position.x << " y:" << _position.y << " z:" << _position.z <<
		  " and color r:" << _color.r << " g:" << _color.g << " b:" << _color.b;
	position = _position;
	setColor(_color);
  }

  [[maybe_unused]] void setPosition(const glm::vec3 &_position) {
	Vertex::position = _position;
  }
  [[maybe_unused]] void setColor(glm::vec3 _color) {
	if (_color.r > 1 || _color.g > 1 || _color.b > 1 || _color.r < 0 || _color.g < 0 || _color.b < 0) {
	  PLOGE << "Vertex(" << this << " Attempted to set color that exceeds allowed range. (this call will be ignored)";
	} else {
	  Vertex::color = _color;
	}
  }
  [[maybe_unused]] void setNormal(const glm::vec3 &_normal) {
	Vertex::normal = _normal;
  }
  ~Vertex() {
	PLOGV << "Destroyed Vertex(" << this << ")";
  }
};

#endif //CGLABS__VERTEX_HPP_
