#define GL_SILENCE_DEPRECATION
#include <spdlog/spdlog.h>

#include <random>

#include "color_buffer.hpp"
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
	spdlog::info("selected_optionY is now {}", selected_optionY);
  }
  if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {

	selected_optionY++;
	spdlog::info("selected_optionY is now {}", selected_optionY);
  }
  if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
	selected_optionX--;
	spdlog::info("selected_optionX is now {}", selected_optionX);
  }
  if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
	selected_optionX++;
	spdlog::info("selected_optionX is now {}", selected_optionX);
  }
}
template<typename Numeric, typename Generator = std::mt19937>
Numeric random(Numeric from, Numeric to) {
  thread_local static Generator gen(std::random_device{}());

  using dist_type = typename std::conditional<
	  std::is_integral<Numeric>::value, std::uniform_int_distribution<Numeric>, std::uniform_real_distribution<Numeric> >::type;

  thread_local static dist_type dist;

  return dist(gen, typename dist_type::param_type{from, to});
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  spdlog::info("App stated!");
  Window window({640, 640});

  VertexArray vertexArray;
  VertexArray vertexArrayPoints;
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
  std::vector<Vertex> points;
  points.reserve(4);
  for (int i = 0; i < 4; ++i) {
	points.emplace_back(Vertex({random<double>(-1, 1), random<double>(-1, 1), 0}, true, {random<double>(0, 1), random<double>(0, 1), random<double>(0, 1)}));
  }
  VertexBuffer vbPoints(points);
  VertexBufferLayout layout;
  VertexBufferLayout layoutPoints;
  //ColorBuffer colorBuffer(points);

  layout.push<float>(3);      ///< number of params for each vertex
  layoutPoints.push<float>(3);///< number of params for each vertex
  vertexArray.addBuffer(vertexBuffer, layout);
  vertexArrayPoints.addBuffer(vbPoints, layout);
  // vertexArrayPoints.addBuffer(colorBuffer, layoutPoints,0);
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
  std::vector<unsigned int> tmp;
  for (unsigned int i = 0; i < points.size(); i++) {
	tmp.emplace_back(i);
  }
  IndexBuffer index_buffer10(tmp);

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
	vertexArray.bind();
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
	  case 10:
		vertexArrayPoints.bind();
		glPointSize(4);
		Renderer::draw(&vertexArrayPoints, &index_buffer10, &shader, GL_POINTS);
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
