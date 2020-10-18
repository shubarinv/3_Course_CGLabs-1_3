#define GL_SILENCE_DEPRECATION
#include <spdlog/spdlog.h>

#include "index_buffer.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "vertex.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
#include "window.hpp"

int selected_optionX = 0;
int selected_optionY = 0;
void handleKeyboard(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
  spdlog::info("Keyboard callback ");
  if ((key == GLFW_KEY_Q && action == GLFW_PRESS) && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
	spdlog::info("Got quit command, destroying window");
	glfwDestroyWindow(window);
	spdlog::info("Quiting...");
	glfwTerminate();
	exit(0);
  }
  if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
	selected_optionY--;
  }
  if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
	selected_optionY++;
  }
  if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
	selected_optionX--;
  }
  if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
	selected_optionX++;
  }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  spdlog::info("App stated!");
  Window window({640, 640});

  VertexArray vertexArray;
  VertexBuffer vertexBuffer({
	  Vertex({0.0f, 0.84853f, 0}),
	  Vertex({-0.6f, 0.6f, 0}),
	  Vertex({-0.84853f, 0, 0}),
	  Vertex({-0.6f, -0.6f, 0}),
	  Vertex({0.f, -0.84853f, 0}),
	  Vertex({0.6f, -0.6f, 0}),
	  Vertex({0.84853f, 0.0f, 0}),
	  Vertex({0.6f, 0.6f, 0}),

  });
  VertexBufferLayout layout;
  layout.push<float>(3);///< number of params for each vertex
  vertexArray.addBuffer(vertexBuffer, layout);

  IndexBuffer index_buffer0({0, 1, 2, 3, 2, 4, 4, 2, 5, 5, 2, 6, 6, 2, 7, 7, 2, 0});
  IndexBuffer index_buffer1({0, 2, 4, 4, 6, 0, 1, 3, 5, 5, 7, 1, 0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 0});
  IndexBuffer index_buffer2({0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 0});
  IndexBuffer index_buffer3({1, 3, 5});
  IndexBuffer index_buffer4({1, 2, 6, 6, 7, 1});
  IndexBuffer index_buffer5({0, 1, 2, 3, 2, 4, 4, 2, 5, 5, 0, 2});
  IndexBuffer index_buffer6({2, 3, 6, 6, 7, 2});
  IndexBuffer index_buffer7({1, 3, 5, 5, 7, 1});
  IndexBuffer index_buffer8({0, 1, 4, 4, 7, 0});
  IndexBuffer index_buffer9({0, 2, 4, 4, 6, 0});

  Shader shader("../resources/shaders/basic.glsl");

  float r         = 0.0f;
  float increment = 0.05f;
  /* Loop until the user closes the window */
  glfwSetKeyCallback(window.getWindow(), handleKeyboard);
  while (!glfwWindowShouldClose(window.getWindow())) {
	window.updateFpsCounter();
	Renderer::clear();
	shader.bind();
	shader.setUniform4f("u_Color", {r, 0.4f, 0.7f, 1.0f});
	switch (selected_optionX) {
	  case 0:
		Renderer::draw(&vertexArray, &index_buffer0, &shader);
		break;
	  case 1:
		Renderer::draw(&vertexArray, &index_buffer1, &shader);
		break;
	  case 2:
		Renderer::draw(&vertexArray, &index_buffer2, &shader, GL_LINES);
		break;
	  case 3:
		Renderer::draw(&vertexArray, &index_buffer3, &shader);
		break;
	  case 4:
		Renderer::draw(&vertexArray, &index_buffer4, &shader);
		break;
	  case 5:
		Renderer::draw(&vertexArray, &index_buffer5, &shader);
		break;
	  case 6:
		Renderer::draw(&vertexArray, &index_buffer6, &shader);
		break;
	  case 7:
		Renderer::draw(&vertexArray, &index_buffer7, &shader);
		break;
	  case 8:
		Renderer::draw(&vertexArray, &index_buffer8, &shader);
		break;
	  case 9:
		Renderer::draw(&vertexArray, &index_buffer9, &shader);
		break;
	  default:
		break;
	}
	switch (selected_optionY) {
	  case 0:
		glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		break;
	  case 1:
		glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		break;
	  case 2:
		glCall(glPointSize(10));
		glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_POINT));
		break;
	  default:
		break;
	}

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
