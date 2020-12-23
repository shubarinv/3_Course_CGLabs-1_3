//
// Created by Vladimir Shubarin on 22.12.2020.
//

#ifndef CGLABS__MESH_HPP_
#define CGLABS__MESH_HPP_
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Shaders/VertexColor.h>

#include <utility>
#include "functions.hpp"
#include "vertex.hpp"
class Mesh {
  GL::Mesh glMesh{};
  GL::Buffer vertexBuffer;
  GL::Buffer indexBuffer;
  std::vector<unsigned int> indexBuffer_vec;
  std::vector<Vertex> vertexBuffer_vec;

 public:
  explicit Mesh(std::vector<Vertex> _vertices,const std::vector<unsigned int>& indices) {
	setIndexBuffer(indices);
	vertexBuffer_vec=std::move(_vertices);
	generateNormals();
	glMesh.setPrimitive(GL::MeshPrimitive::Triangles);

	std::vector<Vertex::triangleVertex> vertices;
	vertices.reserve(vertexBuffer_vec.size());
	for (auto& vertex : vertexBuffer_vec) {
	  vertices.push_back(vertex.getStruct());
	}
	vertexBuffer.setData(vertices, GL::BufferUsage::StaticDraw);
	glMesh.setCount(vertices.size()).addVertexBuffer(std::move(vertexBuffer), 0, Shaders::Phong::Position{}, Shaders::Phong::Normal{});
  }
  void draw(Shaders::VertexColor3D* shader) {
	shader->draw(glMesh);
  }

  void setIndexBuffer(const std::vector<unsigned int>& indices) {
	indexBuffer_vec = indices;
	indexBuffer.setData(indices, GL::BufferUsage::StaticDraw);
	glMesh.setIndexBuffer(indexBuffer, 0, GL::MeshIndexType::UnsignedInt, 0, indices.size() - 1).setCount(indices.size());
  }
  void draw(Shaders::Phong* shader){
	shader->draw(glMesh);
  }

  void generateNormals() {
	for (unsigned int i = 0; i < indexBuffer_vec.size(); i += 3) {
	  unsigned int Index0 = indexBuffer_vec[i];
	  unsigned int Index1 = indexBuffer_vec[i + 1];
	  unsigned int Index2 = indexBuffer_vec[i + 2];
	  Vector3 v1 = vertexBuffer_vec[Index1].getPosition() -  vertexBuffer_vec[Index0].getPosition();
	  Vector3 v2 = vertexBuffer_vec[Index2].getPosition() -  vertexBuffer_vec[Index0].getPosition();
	  Vector3 Normal = Magnum::Math::cross(v1, v2);//glm::cross(v1, v2);//v1.Cross(v2);
	  Normal = Normal.normalized();

	 vertexBuffer_vec[Index0].normal += Normal;
	 vertexBuffer_vec[Index1].normal += Normal;
	 vertexBuffer_vec[Index2].normal += Normal;
	}

	for (auto & i : vertexBuffer_vec) {
	  i.normal =  i.normal.normalized();
	}
  }
  GL::Mesh *getRef(){
	return &glMesh;
  }
};

#endif//CGLABS__MESH_HPP_
