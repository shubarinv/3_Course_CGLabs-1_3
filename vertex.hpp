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
  Vertex(Vector3 _position, Color3 _color) {
	position = _position;
	color = _color;
  }
  struct triangleVertex {
	Vector3 position;
	Color3 color;
  };
  triangleVertex getStruct() {
	return {position, color};
  }
};

#endif//CGLABS__VERTEX_HPP_
