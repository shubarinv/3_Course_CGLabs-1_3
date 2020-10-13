#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "index_buffer.hpp"
#include "shader.hpp"
#include "vertex.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
#include <spdlog/spdlog.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  spdlog::info("App stated!");
  GLFWwindow *window;
  /* Initialize the library */
  if (!glfwInit()) {
	spdlog::critical("Application::Failed to init GLFW");
	throw std::runtime_error("Failed to init GLFW");
  }
  glfwWindowHint(GLFW_SAMPLES, 4);              // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);// We want OpenGL 4.1
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);          // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// We don't want the old OpenGL
  spdlog::debug("GLFW Init - OK");
  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
  if (!window) {
	glfwTerminate();
	spdlog::critical("Application::Failed to create window");
	throw std::runtime_error("Failed to create window");
  }
  spdlog::debug("Window created successfully");

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  GLenum err = glewInit();
  if (GLEW_OK != err) {
	/* Problem: glewInit failed, something is seriously wrong. */
	spdlog::critical("Application::GLEW init failed: {}", glewGetErrorString(err));
  }
  spdlog::info("Status: Using GLEW v{}", glewGetString(GLEW_VERSION));
  spdlog::info("Status: Using OpenGL v{}", glGetString(GL_VERSION));
  std::vector<Vertex> vertices{
	  Vertex({-0.5f, -0.5f, 0}),
	  Vertex({0.5f, -0.5f, 0}),
	  Vertex({0.5f, 0.5f, 0}),
	  Vertex({-0.5f, 0.5f, 0})};
  unsigned int indices[] = {
	  0, 1, 2,
	  2, 3, 0};

  VertexArray vertexArray;
  VertexBuffer vertexBuffer(vertices);
  VertexBufferLayout layout;
  layout.push<float>(3);
  vertexArray.addBuffer(vertexBuffer, layout);

  IndexBuffer index_buffer(indices, 6);

  Shader shader("../resources/shaders/basic.glsl");
  shader.bind();
  shader.setUniform4f("u_Color", {0.3f, 0.4f, 0.7f, 1.0f});

  float r = 0.0f;
  float increment = 0.05f;
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
	/* Render here */

	glCall(glClear(GL_COLOR_BUFFER_BIT));
	shader.setUniform4f("u_Color", {r, 0.4f, 0.7f, 1.0f});
	vertexArray.bind();
	index_buffer.bind();
	glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

	if (r > 1.0f)
	  increment = -0.05f;
	else if (r < 0.0f)
	  increment = 0.05f;
	r += increment;
	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	/* Poll for and process events */
	glfwPollEvents();
  }
  glfwTerminate();
  spdlog::info("Program finished");
  return 0;
}
