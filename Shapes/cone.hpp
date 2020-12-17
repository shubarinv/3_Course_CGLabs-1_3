//
// Created by Vladimir Shubarin on 16.11.2020.
//

#ifndef CGLABS__CONE_HPP_
#define CGLABS__CONE_HPP_
#include "../object.hpp"
class Cone : public Object {

  static std::vector<Vertex> getCoordsForVertices(glm::vec3 position, double radius, int sides, glm::vec3 color = {0.5, 0, 0}) {
	std::vector<Vertex> vertices;
	auto xe = position.x;
	auto ze = position.z + radius;
	vertices.push_back(Vertex({xe, position.y, ze}, color));
	double alpha = M_PI / 2;
	for (int i = 0; i < sides - 1; i++) {
	  alpha += 2 * M_PI / sides;
	  auto xr = position.x + radius * cos(alpha);
	  auto zr = position.z + radius * sin(alpha);
	  xe = xr;
	  ze = zr;
	  vertices.push_back(Vertex({xe, position.y, ze}, color));
	}
	return vertices;
  }
 public:
  Cone(glm::vec3 position, double radius, double height, int sides, glm::vec3 _color = {.1, 0.5, 0.21}) {
	color = _color;
	std::vector<Vertex> vertices = getCoordsForVertices(position, radius, sides, color);
	vertices.push_back(Vertex({position.x, position.y + height, position.z}, color));
	vertexBuffer = new VertexBuffer(vertices);
	colorBuffer = new ColorBuffer(vertexBuffer->getVertices());
	std::vector<unsigned int> indices;
	for (int i = 0; i < vertices.size() - 1; i++) {
	  indices.push_back(i);
	  indices.push_back(vertices.size() - 1);
	  if (i + 1 != vertices.size() - 1)
		indices.push_back(i + 1);
	  else
		indices.push_back(0);
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

#endif //CGLABS__CONE_HPP_
