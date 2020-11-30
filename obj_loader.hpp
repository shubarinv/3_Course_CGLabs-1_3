//
// Created by Vladimir Shubarin on 29.11.2020.
//

#ifndef CGLABS__OBJ_LOADER_HPP_
#define CGLABS__OBJ_LOADER_HPP_
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "Libs/tiny_obj_loader.h"

class objLoader {
 public:
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::vector<float> loadObj(std::string filename) {
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str());

	if (!err.empty()) { // `err` may contain warning message.
	  std::cerr << err << std::endl;
	}

	if (!ret) {
	  exit(1);
	}

// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
	  // Loop over faces(polygon)
	  size_t index_offset = 0;
	  for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
		int fv = shapes[s].mesh.num_face_vertices[f];

		// Loop over vertices in the face.
		for (size_t v = 0; v < fv; v++) {
		  // access to vertex
		  tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
		  float vx = attrib.vertices[3 * idx.vertex_index + 0];
		  float vy = attrib.vertices[3 * idx.vertex_index + 1];
		  float vz = attrib.vertices[3 * idx.vertex_index + 2];
		  float nx = attrib.normals[3 * idx.normal_index + 0];
		  float ny = attrib.normals[3 * idx.normal_index + 1];
		  float nz = attrib.normals[3 * idx.normal_index + 2];
		  float tx = attrib.texcoords[2 * idx.texcoord_index + 0];
		  float ty = attrib.texcoords[2 * idx.texcoord_index + 1];
		}
		index_offset += fv;

		// per-face material
		shapes[s].mesh.material_ids[f];
	  }
	}
	return attrib.vertices;
  }

};

#endif //CGLABS__OBJ_LOADER_HPP_
