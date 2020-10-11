#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLUT/glut.h>

#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
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

  float positions[] = {
	  -0.5f, -0.5f,
	  0.5f, 0.f,
	  0.5f, 0.5f};
  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT);

	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	/* Poll for and process events */
	glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
