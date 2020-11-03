//
// Created by Vladimir Shubarin on 20.10.2020.
//
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
  spdlog::info("Keyboard callback");
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

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  spdlog::info("App stated!");
  Window window({800, 600});
  Shader lShader("../resources/shaders/basic_w_layout.glsl");  ///< use this shader when you want to use layouts
  Shader uShader("../resources/shaders/basic_w_uniforms.glsl");///< use this shader when you want to use uniforms
  VertexArray vertexArray;
  VertexArray vertexArrayPoints;
  VertexBuffer vertexBuffer({
	  Vertex({
				 -1.0,
				 -1.0,
				 1.0,
			 },
			 {1.0, 0.0, 0.0}),
	  Vertex({
				 1.0,
				 -1.0,
				 1.0,
			 },
			 {
				 0.0,
				 1.0,
				 0.0,
			 }),
	  Vertex({
				 1.0,
				 1.0,
				 1.0,
			 },
			 {
				 0.0,
				 0.0,
				 1.0,
			 }),
	  Vertex({
				 -1.0,
				 1.0,
				 1.0,
			 },
			 {
				 1.0,
				 1.0,
				 1.0,
			 }),
	  Vertex({
				 -1.0,
				 -1.0,
				 -1.0,
			 },
			 {
				 1.0,
				 0.0,
				 0.0,
			 }),
	  Vertex({
				 1.0,
				 -1.0,
				 -1.0,
			 },
			 {
				 0.0,
				 1.0,
				 0.0,
			 }),
	  Vertex({
				 1.0,
				 1.0,
				 -1.0,
			 },
			 {
				 0.0,
				 0.0,
				 1.0,
			 }),
	  Vertex({-1.0, 1.0, -1.0}, {1.0, 1.0, 1.0}),
  });
  VertexBufferLayout layout;
  ColorBuffer colorBuffer(vertexBuffer.getVertices());
  layout.push<float>(3);///< number of params for each vertex
  vertexArray.addBuffer(vertexBuffer, layout);
  vertexArray.addBuffer(colorBuffer, layout, 1);
  IndexBuffer index_buffer0({0, 1, 2,
							 2, 3, 0,
							 // right
							 1, 5, 6,
							 6, 2, 1,
							 // back
							 7, 6, 5,
							 5, 4, 7,
							 // left
							 4, 0, 3,
							 3, 7, 4,
							 // bottom
							 4, 5, 1,
							 1, 0, 4,
							 // top
							 3, 2, 6,
							 6, 7, 3});
  float r         = 0.0f;
  float increment = 0.05f;

  glm::mat4 projection = glm::perspective(
	  glm::radians(90.f),// Вертикальное поле зрения в радианах. Обычно между 90&deg; (очень широкое) и 30&deg; (узкое)
	  4.0f / 3.0f,       // Отношение сторон. Зависит от размеров вашего окна. Заметьте, что 4/3 == 800/600 == 1280/960
	  0.1f,              // Ближняя плоскость отсечения. Должна быть больше 0.
	  100.0f             // Дальняя плоскость отсечения.
  );
  // Или, для ортокамеры
  glm::mat4 view = glm::lookAt(
	  glm::vec3(3, 2, 2),// Камера находится в мировых координатах (4,3,3)
	  glm::vec3(0, 0, 0),// И направлена в начало координат
	  glm::vec3(0, 1, 0) // "Голова" находится сверху
  );
  // Матрица модели : единичная матрица (Модель находится в начале координат)
  glm::mat4 model = glm::mat4(1.0f);// Индивидуально для каждой модели

  glm::mat4 MVPmatrix = projection * view * model;// Запомните! В обратном порядке!
  /* Loop until the user closes the window */
  glfwSetKeyCallback(window.getWindow(), handleKeyboard);
  // Включить тест глубины
  glEnable(GL_DEPTH_TEST);
  // Фрагмент будет выводиться только в том, случае, если он находится ближе к камере, чем предыдущий
  glDepthFunc(GL_LESS);
  while (!glfwWindowShouldClose(window.getWindow())) {
	window.updateFpsCounter();
	Renderer::clear();
	const float radius = 2.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;
	float camY = tan(glfwGetTime()) * camZ + camX;
	view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	MVPmatrix = projection * view * model;// Запомните! В обратном порядке!
	lShader.bind();
	lShader.setUniformMat4f("u_MVP", MVPmatrix);

	vertexArray.bind();
	Renderer::draw(&vertexArray, &index_buffer0, &lShader);
	switch (selected_optionY) {
	  case 0:
		glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		break;
	  case 3:
		glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		break;
	  case 2:
		glCall(glPointSize(10));
		glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_POINT));
		break;
	  case 1:
		glCall(glEnable(GL_DEPTH_TEST));
		glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
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
