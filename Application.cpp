#define GL_SILENCE_DEPRECATION
#include <spdlog/spdlog.h>

#include "index_buffer.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "vertex.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
#include "window.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  spdlog::info("App stated!");
  Window window({640, 480});

  VertexArray vertexArray;
  VertexBuffer vertexBuffer({Vertex({-0.5f, -0.5f, 0}),
							 Vertex({0.5f, -0.5f, 0}),
							 Vertex({0.5f, 0.5f, 0}),
							 Vertex({-0.5f, 0.5f, 0})});
  VertexBufferLayout layout;
  layout.push<float>(3);///< number of params for each vertex
  vertexArray.addBuffer(vertexBuffer, layout);

  IndexBuffer index_buffer({0, 1, 2, 2, 3, 0});

  Shader shader("../resources/shaders/basic.glsl");

  shader.setUniform4f("u_Color", {0.3f, 0.4f, 0.7f, 1.0f});

  float r         = 0.0f;
  float increment = 0.05f;
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window.getWindow())) {

	Renderer::clear();
	shader.bind();
	shader.setUniform4f("u_Color", {r, 0.4f, 0.7f, 1.0f});
	Renderer::draw(&vertexArray, &index_buffer, &shader);
	if (r > 1.0f)
	  increment = -0.05f;
	else if (r < 0.0f)
	  increment = 0.05f;
	r += increment;
	/* Swap front and back buffers */
	glfwSwapBuffers(window.getWindow());

	/* Poll for and process events */
	glfwPollEvents();
  }
  window.destroy();
  spdlog::info("Program finished");
  return 0;
}
