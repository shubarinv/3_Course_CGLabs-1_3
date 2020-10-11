#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>
static unsigned int compileShader(int type, std::string &source) {
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

  float positions[6] = {
	  -0.8f, -0.8f,
	  0.0f, 0.8f,
	  0.8f, -0.8f};
  unsigned int buffer;
  GLuint vertexArrayID;
  glGenVertexArrays(1, &vertexArrayID);
  glBindVertexArray(vertexArrayID);
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void *)nullptr);
  glEnableVertexAttribArray(0);
  std::string vertexShader =
	  "#version 410 core\n"
	  "\n"
	  "layout(location=0)in vec4 position;\n"
	  "\n"
	  "void main()\n"
	  "{\n"
	  "gl_Position =position;\n"
	  "}\n";
  std::string fragmentShader =
	  "#version 410 core\n"
	  "\n"
	  "layout(location=0)out vec4 color;\n"
	  "\n"
	  "void main()\n"
	  "{\n"
	  "color =vec4(1.0,0.0,0.0,1.0);\n"
	  "}\n";
  unsigned int shader = createShader(vertexShader, fragmentShader);
  glUseProgram(shader);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	/* Poll for and process events */
	glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
