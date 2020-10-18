//
// Created by Vladimir Shubarin on 17.10.2020.
//

#ifndef CGLABS__WINDOW_HPP_
#define CGLABS__WINDOW_HPP_
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "lib.hpp"

class Window {
  GLFWwindow* glWindow;

 public:
  void destroy() {
	glfwDestroyWindow(glWindow);
	glfwTerminate();
  }
  bool getShouldClose() {
	glfwWindowShouldClose(glWindow);
  }

  explicit Window(glm::vec2 size) {
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
	glWindow = glfwCreateWindow(size.x, size.y, "Hello World", nullptr, nullptr);
	if (!glWindow) {
	  glfwTerminate();
	  spdlog::critical("Application::Failed to create window");
	  throw std::runtime_error("Failed to create window");
	}
	spdlog::debug("Window created successfully");

	/* Make the window's context current */
	glfwMakeContextCurrent(glWindow);
	GLenum err = glewInit();
	if (GLEW_OK != err) {
	  /* Problem: glewInit failed, something is seriously wrong. */
	  spdlog::critical("Application::GLEW init failed: {}", glewGetErrorString(err));
	}
	spdlog::info("Status: Using GLEW v{}", glewGetString(GLEW_VERSION));
	spdlog::info("Status: Using OpenGL v{}", glGetString(GL_VERSION));
  }
  GLFWwindow* getWindow() {
	return glWindow;
  }
};

#endif//CGLABS__WINDOW_HPP_
