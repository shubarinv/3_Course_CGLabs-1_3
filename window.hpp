//
// Created by Vladimir Shubarin on 02.12.2020.
//

#ifndef CGLABS__WINDOW_HPP_
#define CGLABS__WINDOW_HPP_
#include "functions.hpp"
class Window {
 private:
  GLFWwindow *window;
 public:
  [[nodiscard]] GLFWwindow *getGLFWWindow() const {
	return window;
  }
 public:
  Window(glm::vec2 size) {
	glfwSetErrorCallback(glfwErrorHandler);
	if (!glfwInit()) {
	  PLOGF << "GLFW INIT FAILED";
	}
	PLOGV << "GLFW init - OK";

	if (isMac()) {
	  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);// Highest available version for macOS
	  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);// Highest available version for macOS

	  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	  PLOGV << "System: MacOS";
	}

	window = glfwCreateWindow(size.x, size.y, "UNSET", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (window == nullptr) {
	  PLOGF << "GLFW was unable to create window";
	  glfwTerminate();
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	  PLOGF << "GLAD init Failed";
	}
	PLOGV << "Status: Using OpenGL v" << glGetString(GL_VERSION);
	PLOGV << "Renderer: " << glGetString(GL_RENDERER); /* get renderer string */
	GLint maxShaderTextures;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxShaderTextures);
	GLint maxTotalTextures;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTotalTextures);
	PLOGV << "Number of textures that can be accessed by the fragment shader: " << maxShaderTextures;
	PLOGV << "Total number of textures that can be used " << maxTotalTextures;
	glEnable(GL_MULTISAMPLE);
	PLOGV << "Init DONE!";
  }
  static void glfwErrorHandler(int error, const char *message) {
	PLOGE << "GLFW error: " << error << " " << message;
  }
  ~Window() {
	glfwDestroyWindow(window);
	PLOGV << "GLFW window destroyed";
	PLOGV << "Window(" << this << ") destroyed";
  }
};

#endif //CGLABS__WINDOW_HPP_
