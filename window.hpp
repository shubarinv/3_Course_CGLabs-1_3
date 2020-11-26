//
// Created by Vladimir Shubarin on 17.10.2020.
//

#ifndef CGLABS__WINDOW_HPP_
#define CGLABS__WINDOW_HPP_


#include <utility>
#include <iostream>

#include "lib.hpp"

class Window {
  GLFWwindow* glWindow;
  std::string title;

 public:
  void destroy() {
	glfwDestroyWindow(glWindow);
	glfwTerminate();
  }
  [[maybe_unused]] bool getShouldClose() {
	return glfwWindowShouldClose(glWindow);
  }

  explicit Window(glm::vec2 size, std::string _title = "UNSET ") {
	/* Initialize the library */
	if (!glfwInit()) {
	  throw std::runtime_error("Failed to init GLFW");
	}
	title = std::move(_title);
	glfwWindowHint(GLFW_SAMPLES, 4);              // 4x antialiasing
	if(isMac()){
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);// We want OpenGL 4.1
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	}
	else if(isWindows()){
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);// We want OpenGL 4.1
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	}
	glfwWindowHint(GLFW_FLOATING, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);          // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// We don't want the old OpenGL

	if(!isWindows()){
	    spdlog::debug("GLFW Init - OK");
	}
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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	  std::cout << "Failed to initialize OpenGL context" << std::endl;
	  throw;
	}
	spdlog::info("Status: Using OpenGL v{}", glGetString(GL_VERSION));
	GLint maxShaderTextures;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxShaderTextures);
	GLint maxTotalTextures;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTotalTextures);
	spdlog::info("Number of textures that can be accessed by the fragment shader: {}", maxShaderTextures);
	spdlog::info("Total number of textures that can be used {}", maxTotalTextures);
	glEnable(GL_MULTISAMPLE);
  }
  [[maybe_unused]] void updateFpsCounter() {
	static double previous_seconds = glfwGetTime();
	static int frame_count;
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;
	if (elapsed_seconds > 0.25) {
	  previous_seconds = current_seconds;
	  double fps       = (double)frame_count / elapsed_seconds;
	  std::string tmp  = title + "@ fps: " + std::to_string((int)round(fps));
	  glfwSetWindowTitle(glWindow, tmp.c_str());
	  frame_count = 0;
	}
	frame_count++;
  }
  GLFWwindow* getWindow() {
	return glWindow;
  }
};

#endif//CGLABS__WINDOW_HPP_
