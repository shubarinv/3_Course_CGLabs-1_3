//
// Created by Vladimir Shubarin on 20.12.2020.
//

#ifndef CGLABS__LIGHT_HPP_
#define CGLABS__LIGHT_HPP_

#include "shader.hpp"
class Light {
  Shader *shader{};
  glm::vec3 position{-1, -1, -1}, color{-1, -1, -1}, direction{-1, -1, -1};
  double intensity{-1};

 public:
  explicit Light(double _intensity) {
	intensity = _intensity;
  }
  Light(glm::vec3 _position, double _intensity, glm::vec3 _color) {
	position = _position;
	color = _color;
	intensity = _intensity;
  }
  Light(glm::vec3 _position, double _intensity, glm::vec3 _color, glm::vec3 _direction) {
	position = _position;
	color = _color;
	intensity = _intensity;
	direction = _direction;
  }
  void setShader(Shader *_shader) {
	shader = _shader;
  }
  void setShader(std::string _shader) {
	shader = new Shader(_shader);
  }
  void draw() {
	if (shader == nullptr) {
	  throw std::runtime_error("Shader for light was not set!");
	}
	shader->bind();
	if (intensity != -1) {
	}
	if (position != glm::vec3(-1, -1, -1)) {
	}
	if (color != glm::vec3(-1, -1, -1)) {
	}
	if (direction != glm::vec3(-1, -1, -1)) {
	}
	shader->unbind();
  }
};

#endif//CGLABS__LIGHT_HPP_
