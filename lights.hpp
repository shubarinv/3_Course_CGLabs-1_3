//
// Created by Vladimir Shubarin on 24.12.2020.
//

#ifndef CGLABS__LIGHTS_HPP_
#define CGLABS__LIGHTS_HPP_
#include <utility>

#include "functions.hpp"
class Lights {
  struct Light {
	Light(Vec3 _position, Color4_ _color, std::string _name) {
	  position = _position;
	  color = _color;
	  name = std::move(_name);
	}
	Vec3 position;
	Color4_ color;
	std::string name;
	bool enabled = true;
	void enable() {
	  enabled = true;
	}
	void disable() {
	  enabled = false;
	}
  };
  std::vector<Light> lights;

 public:
  void addLight(Light light) {
	lights.push_back(light);
  }
  std::vector<Vec3> getPositions() {
	std::vector<Vec3> lightsPos;
	for (auto& light : lights) {
	  if (light.enabled)
		lightsPos.push_back(light.position);
	}
	return lightsPos;
  }
  std::vector<Color4_> getColors() {
	std::vector<Color4_> lightsColors;
	for (auto& light : lights) {
	  if (light.enabled)
		lightsColors.push_back(light.color);
	}
	return lightsColors;
  }
  Light* lookForTheLight(const std::string& name) {
	for (auto& light : lights) {
	  if (light.name == name) return &light;
	}
	return nullptr;
  }
  void setPosition(Light* light, Vec3 newPosition) {
	light->position = newPosition;
  }
  void setColor(Light* light, Vec3 newColor) {
	light->color = newColor;
  }
  int countEnabledLights() {
	int enabledLights=0;
	for (auto& light : lights) {
	  if (light.enabled)
		enabledLights++;
	}
	return enabledLights;
  }
};

#endif//CGLABS__LIGHTS_HPP_
