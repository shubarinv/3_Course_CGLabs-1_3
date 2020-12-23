//
// Created by Vladimir Shubarin on 22.12.2020.
//

#ifndef CGLABS__MESH_HPP_
#define CGLABS__MESH_HPP_
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Shaders/VertexColor.h>

#include "vertex.hpp"
class Mesh {
  GL::Mesh glMesh{};
  GL::Buffer vertexBuffer;
  GL::Buffer indexBuffer;

 public:
  explicit Mesh(std::vector<Vertex> _vertices) {
	glMesh.setPrimitive(GL::MeshPrimitive::Triangles);

	std::vector<Vertex::triangleVertex> vertices;
	vertices.reserve(_vertices.size());
	for (auto& vertex : _vertices) {
	  vertices.push_back(vertex.getStruct());
	}
	vertexBuffer.setData(vertices, GL::BufferUsage::StaticDraw);
	glMesh.setCount(vertices.size()).addVertexBuffer(std::move(vertexBuffer), 0, Shaders::VertexColor3D::Position{}, Shaders::VertexColor3D::Color3{});
  }
  void draw(Shaders::VertexColor3D* shader) {
	shader->draw(glMesh);
  }

  void setIndexBuffer(const std::vector<unsigned int>& indices) {
	indexBuffer.setData(indices, GL::BufferUsage::StaticDraw);
	glMesh.setIndexBuffer(indexBuffer, 0, GL::MeshIndexType::UnsignedInt, 0, indices.size() - 1).setCount(indices.size());
  }
};

#endif//CGLABS__MESH_HPP_
