//
// Created by Vladimir Shubarin on 10/13/20.
//

#ifndef CGLABS__VERTEX_HPP_
#define CGLABS__VERTEX_HPP_
class Vertex {
  glm::vec3 position{};///< @brief coords of Vertex

 public:
  /**
   * @returns returns position of the Vertex
   */
  [[maybe_unused]] [[nodiscard]] const glm::vec3 &getPosition() const {
	return position;
  }
  /**
   * @returns returns color of the vertex
   */
  [[maybe_unused]] [[nodiscard]] const glm::vec3 &getColor() const {
	return color;
  }
  /**
   * @param _pos vec3 position of vertex
   * @param _color vec3 color of vertex
   */
  explicit Vertex(glm::vec3 _pos = {0, 0, 0}, glm::vec3 _color = {0, 0, 0}) {
	setPosition(_pos);
	setColor(_color);
  }

  glm::vec3 normal{};
 private:
  glm::vec3 color{};
  /**
   * @brief sets position for the vertex
   * @param _newPosition new position for the vertex
   */
  void setPosition(glm::vec3 _newPosition) {
	position = _newPosition;
  }
  /**
   * @brief sets color for vertex
   * @param _color vec3 color for vertex
   */
  void setColor(glm::vec3 _color) {
	if (_color.x < -1 || _color.x > 1 || _color.y < -1 || _color.y > 1 || _color.z < -1 || _color.z > 1) {
	  LOG_S(ERROR) << "Invalid color!";
	}
	color = _color;
  }

  /**
   * @brief Compares two vertices
   */
  bool operator==(const Vertex &b) const {
	if (position.x == b.position.x && position.y == b.position.y && position.z == b.position.z) {
	  return true;
	} else
	  return false;
  }
};

#endif//CGLABS__VERTEX_HPP_
