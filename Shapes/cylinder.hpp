//
// Created by Vladimir Shubarin on 18.11.2020.
//

#ifndef CGLABS__CYLINDER_HPP_
#define CGLABS__CYLINDER_HPP_
#include "../object.hpp"

class Cylinder : public Object {

  static std::vector<Vertex> getCoordsForVertices(glm::vec3 position,
												  double radius,
												  int sides,
												  double height,
												  glm::vec3 color = {0.5, 0, 0}) {
	std::vector<Vertex> vertices;
	auto xe = position.x;
	auto ze = position.z + radius;
	spdlog::info("x: {} z: {}", xe, ze);
	double alpha = M_PI / 2;
	for (int i = 0; i < sides; i++) {
	  alpha += 2 * M_PI / sides;
	  auto xr = position.x + radius * cos(alpha);
	  auto zr = position.z + radius * sin(alpha);
	  xe = xr;
	  ze = zr;
	  vertices.push_back(Vertex({xe, position.y, ze}, color));
	  vertices.push_back(Vertex({xe, position.y + height, ze}, color));
	}
	return vertices;
  }
 public:
  Cylinder(glm::vec3 position, double radius, double height, int sides, glm::vec3 _color = {.1, 0.5, 0.21}) {
	color = _color;
	std::vector<Vertex> vertices = getCoordsForVertices(position, radius, sides, height, color);
	vertexBuffer = new VertexBuffer(vertices);
	colorBuffer = new ColorBuffer(vertexBuffer->getVertices());
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < vertices.size(); i++) {
	  indices.push_back(i);
	  if (i == vertices.size() - 1) {
		indices.push_back(0);
	  } else if (indices.size() % 3 == 0) {
		indices.push_back(i);
	  }
	}
	for (unsigned int i = 1; i < vertices.size(); i++) {
	  indices.push_back(i);
	  if (i == vertices.size() - 1) {
		indices.push_back(vertices.size() - 1);
		indices.push_back(0);
		indices.push_back(1);
	  } else if (indices.size() % 3 == 0) {
		indices.push_back(i);
	  }
	}
	indexBuffer = new IndexBuffer(indices);
	bufferLayout = new VertexBufferLayout();
	bufferLayout->push<float>(3);
	vertexArray = new VertexArray;
	vertexArray->addBuffer(*vertexBuffer, *bufferLayout);
	vertexArray->addBuffer(*colorBuffer, *bufferLayout, 1);
	bInitialized = true;
  }
};

#endif //CGLABS__CYLINDER_HPP_
