//
// Created by Vladimir Shubarin on 20.12.2020.
//

#ifndef CGLABS__LIGHT_HPP_
#define CGLABS__LIGHT_HPP_

class Light {
  glm::vec3 position{}, color{}, direction{};
  double intensity{};
  explicit Light(double _intensity) {
	intensity = _intensity;
  }
  Light(glm::vec3 _position, double _intensity, glm::vec3 _color{1,1,1}) {
	position = _position;
	color = _color;
	intensity = _intensity;
  }
  Light(glm::vec3 _position, double _intensity, glm::vec3 _color{1,1,1},glm::vec3 _direction) {
	position = _position;
	color = _color;
	intensity = _intensity;
	direction=_direction;
  }

};

#endif//CGLABS__LIGHT_HPP_
