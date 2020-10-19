//
// Created by Vladimir Shubarin on 10/13/20.
//

#ifndef CGLABS__VERTEX_HPP_
#define CGLABS__VERTEX_HPP_
class Vertex {
  glm::vec3 position{};
  bool front = true;

 public:
  [[maybe_unused]] [[nodiscard]] bool isFront() const {
	return front;
  }

 public:
  [[maybe_unused]] [[nodiscard]] const glm::vec3 &getPosition() const {
	return position;
  }
  [[maybe_unused]] [[nodiscard]] const glm::vec3 &getColor() const {
	return color;
  }
  explicit Vertex(glm::vec3 _pos = {0, 0, 0}, bool _front = true, glm::vec3 _color = {0, 0, 0}) {
	setPosition(_pos);
	setColor(_color);
	front = _front;
  }

 private:
  glm::vec3 color{};
  void setPosition(glm::vec3 _newPosition) {
	if (_newPosition.x < -1 || _newPosition.x > 1 || _newPosition.y < -1 || _newPosition.y > 1 || _newPosition.z < -1 || _newPosition.z > 1) {
	  spdlog::error("Invalid location!");
	  throw std::runtime_error("Invalid location for vertex");
	}
	position = _newPosition;
  }
  void setColor(glm::vec3 _color) {
	if (_color.x < -1 || _color.x > 1 || _color.y < -1 || _color.y > 1 || _color.z < -1 || _color.z > 1) {
	  spdlog::error("Invalid color!");
	}
	color = _color;
  }
  bool operator==(const Vertex &b) const {
	if (position.x == b.position.x && position.y == b.position.y && position.z == b.position.z) {
	  return true;
	} else
	  return false;
  }
};

#endif//CGLABS__VERTEX_HPP_
