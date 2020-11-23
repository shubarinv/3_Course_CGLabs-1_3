//
// Created by Vladimir Shubarin on 12.10.2020.
//

#ifndef CGLABS__RENDERER_HPP_
#define CGLABS__RENDERER_HPP_

#include <string>

#include "Buffers/index_buffer.hpp"
#include "shader.hpp"
#include "Buffers/vertex_array.hpp"
#include "object.hpp"

class Renderer {
 public:
  static void draw(VertexArray *vertexArray, IndexBuffer *indexBuffer, Shader *shader, GLuint mode = GL_TRIANGLES) {
	shader->bind();
	vertexArray->bind();
	indexBuffer->bind();
	glCall(glDrawElements(mode, indexBuffer->getLength(), GL_UNSIGNED_INT, nullptr));
  }
  static void draw(Object *object, Shader *shader, GLuint mode = GL_TRIANGLES) {
	shader->bind();
	object->getVertexArray()->bind();
	object->getIndexBuffer()->bind();
	glCall(glDrawElements(mode, object->getIndexBuffer()->getLength(), GL_UNSIGNED_INT, nullptr));
  }
  static void clear(glm::vec4 clearColor = {0, 0, 0, 0}) {
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
};

#endif//CGLABS__RENDERER_HPP_
