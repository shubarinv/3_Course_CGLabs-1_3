#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <spdlog/spdlog.h>
#include <sstream>
#define ASSERT(X) \
  if (!(X)) __builtin_debugtrap()
#define glCall(x)  \
  glClearErrors(); \
  x;               \
  ASSERT(glLogCall(#x, __FILE__, __LINE__))

struct ShaderProgramSource {
  std::string vertexShader;
  std::string fragmentShader;
};
static void glClearErrors() {
  while (glGetError() != GL_NO_ERROR)
	;
}
static bool glLogCall(const char *function, const char *file, int line) {
  while (GLenum error = glGetError()) {
	spdlog::error("OpenGL error: {} in file {} in function {} at line {}", gluErrorString(error), file, function, line);
  }
  return true;
}
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
  GLuint vertexArrayID;
  glCall(glGenVertexArrays(1, &vertexArrayID));
  glCall(glBindVertexArray(vertexArrayID));
  glCall(glGenBuffers(1, &buffer));
  glCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
  glCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

  glCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *)nullptr));
  glCall(glEnableVertexAttribArray(0));

  unsigned int ibo;//Index buffer object
  glCall(glGenBuffers(1, &ibo));
  glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
  glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

  ShaderProgramSource source = ParseShader("../resources/shaders/basic.glsl");

  unsigned int shader = createShader(source.vertexShader, source.fragmentShader);
  glCall(glUseProgram(shader));

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
	/* Render here */

	glCall(glClear(GL_COLOR_BUFFER_BIT));
	glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
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
