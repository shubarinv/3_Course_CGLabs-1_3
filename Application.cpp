#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "index_buffer.hpp"
#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
#include <fstream>
#include <spdlog/spdlog.h>
#include <sstream>
struct ShaderProgramSource {
  std::string vertexShader{};
  std::string fragmentShader{};
};
static ShaderProgramSource ParseShader(const std::string &filepath) {
  spdlog::info("Parsing shader at: {}", filepath.c_str());
  std::ifstream stream(filepath);
  std::string line;
  std::stringstream ss[2];
  enum class shaderType {
	NONE = -1,
	VERTEX = 0,
	FRAGMENT = 1
  };
  shaderType type = shaderType::NONE;
  while (std::getline(stream, line)) {
	if (line.find("#shader") != std::string::npos) {
	  if (line.find("vertex") != std::string::npos) {
		type = shaderType::VERTEX;
	  } else if (line.find("fragment") != std::string::npos) {
		type = shaderType::FRAGMENT;
	  }
	} else {
	  ss[(int)type] << line << "\n";
	}
  }
  spdlog::info("Shader parsed successfully");
  return {ss[0].str(), ss[1].str()};
}
static unsigned int compileShader(int type, std::string &source) {
  spdlog::info("Trying to compile {}", (type == GL_VERTEX_SHADER ? "VertexShader " : "FragmentShader "));
  unsigned int id = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
	int length;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
	std::string error;
	char buf[length];
	error = "Application::CompileShader() failed to compile ";
	error += (type == GL_VERTEX_SHADER ? "VertexShader " : "FragmentShader ");
	error += buf;
	glGetShaderInfoLog(id, length, &length, buf);
	spdlog::critical(error);
	throw std::runtime_error(error);
	glDeleteShader(id);
  }
  spdlog::info("shader compiled successfully");
  return id;
}

static unsigned int createShader(std::string vertexShader, std::string fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vShader = compileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fShader = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
  glAttachShader(program, vShader);
  glAttachShader(program, fShader);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vShader);
  glDeleteShader(fShader);

  return program;
}

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

  float positions[] = {
	  -0.5f,
	  -0.5f,
	  0.5f,
	  -0.5f,
	  0.5f,
	  0.5f,
	  -0.5f,
	  0.5f,
  };
  unsigned int indices[] = {
	  0, 1, 2,
	  2, 3, 0};
  unsigned int buffer;

  VertexArray vertexArray;
  VertexBuffer vertexBuffer(positions, 4 * 2 * sizeof(float));
  VertexBufferLayout layout;
  layout.push<float>(2);
  vertexArray.addBuffer(vertexBuffer, layout);

  IndexBuffer index_buffer(indices, 6);

  ShaderProgramSource source = ParseShader("../resources/shaders/basic.glsl");

  unsigned int shader = createShader(source.vertexShader, source.fragmentShader);
  glCall(glUseProgram(shader));

  glCall(int location = glGetUniformLocation(shader, "u_Color"));
  ASSERT(location != -1);
  glCall(glUniform4f(location, 0.3f, 0.4f, 0.7f, 1.0f));

  float r = 0.0f;
  float increment = 0.05f;
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
	/* Render here */

	glCall(glClear(GL_COLOR_BUFFER_BIT));

	glCall(glUniform4f(location, r, 0.4f, 0.7f, 1.0f));
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
  glCall(glDeleteProgram(shader));
  glfwTerminate();
  spdlog::info("Program finished");
  return 0;
}
