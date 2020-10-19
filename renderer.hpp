//
// Created by Vladimir Shubarin on 12.10.2020.
//

#ifndef CGLABS__RENDERER_HPP_
#define CGLABS__RENDERER_HPP_

#include <string>

#include "index_buffer.hpp"
#include "shader.hpp"
#include "vertex_array.hpp"

class Renderer {
 public:
  static void draw(VertexArray *vertexArray, IndexBuffer *indexBuffer, Shader *shader, GLuint mode = GL_TRIANGLES, bool isUnif = true) {
	shader->bind();
	vertexArray->bind();
	if (isUnif) {
	  indexBuffer->bind();
	  glCall(glDrawElements(mode, indexBuffer->getLength(), GL_UNSIGNED_INT, nullptr));
	} else {
	  glDrawArrays(mode, 0, vertexArray->getOffset());
	}
  }
  static void clear(glm::vec4 clearColor = {0, 0, 0, 0}) {
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
};

#endif//CGLABS__RENDERER_HPP_
