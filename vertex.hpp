//
// Created by Vladimir Shubarin on 22.12.2020.
//

#ifndef CGLABS__VERTEX_HPP_
#define CGLABS__VERTEX_HPP_
using namespace Magnum;
#include <Magnum/Magnum.h>
class Vertex {

  Vector3 position{};
  Color3 color{};


 public:
  Vector3 normal{};
  Vertex(Vector3 _position, Color3 _color) {
	position = _position;
	color = _color;
  }
   [[nodiscard]] Vector3 getPosition() const {
	return position;
  }
  struct triangleVertex {
	Vector3 position;
//	Color3 color;
	Vector3 normal;
  };
  triangleVertex getStruct() {
	return {position,normal};
  }
};

#endif//CGLABS__VERTEX_HPP_
