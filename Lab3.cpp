//
// Created by Vladimir Shubarin on 20.10.2020.
//
#define GL_SILENCE_DEPRECATION
#include <spdlog/spdlog.h>

#include <cmath>
#include <random>

#include "Buffers/color_buffer.hpp"
#include "Buffers/index_buffer.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "vertex.hpp"
#include "Buffers/vertex_array.hpp"
#include "window.hpp"
#include "Shapes/cone.hpp"
#include "Shapes/cylinder.hpp"

template<typename Numeric, typename Generator = std::mt19937>
Numeric random(Numeric from, Numeric to) {
  thread_local static Generator gen(std::random_device{}());

  using dist_type = typename std::conditional<
	  std::is_integral<Numeric>::value, std::uniform_int_distribution<Numeric>, std::uniform_real_distribution<Numeric> >::type;

  thread_local static dist_type dist;

  return dist(gen, typename dist_type::param_type{from, to});
}

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

  Object objGeneral;
  Object objHyperboloid;
  Object objCube;
  Cone cone00({0, 0.6, 0}, 0.3, 0.3, 4, {.5, 0.9, 0.5});
  Cone cone0({0, 0.3, 0}, 0.4, 0.5, 4, {.4, 0.9, 0.4});
  Cone cone1({0, -0.2, 0}, 0.55, 1, 4, {.3, 0.9, 0.3});
  Cone cone2({0, -0.6, 0}, 0.7, 1, 4, {.2, 0.9, 0.2});
  Cylinder cyl1({0, -0.9, 0}, 0.2, 1.19, 4, {.37, 0.20, 0.21});

  objGeneral.setVertexBuffer({
								 Vertex({0.0f, 0.84853f, 0}, {.81, 0.33, 0.81}),
								 Vertex({-0.6f, 0.6f, 0}, {.70, 0.20, 0.2}),
								 Vertex({-0.84853f, 0, 0}, {.1, 0.4, 0.2}),
								 Vertex({-0.6f, -0.6f, 0}, {.3, 0.7, 0.9}),
								 Vertex({0.f, -0.84853f, 0}, {.0, 0.3, 0.1}),
								 Vertex({0.6f, -0.6f, 0}, {.8, 0.5, 0.8}),
								 Vertex({0.84853f, 0.0f, 0}, {.7, 0.4, 0.5}),
								 Vertex({0.6f, 0.6f, 0}, {.1, 0.5, 0.21}),
							 });

  std::vector<Vertex> tmp;
  for (int i = 0; i <= 200; i++) {
	double angle = 2 * 3.141 * i / 200;
	double pointX = cos(angle);
	double pointY = sin(angle);
	tmp.push_back(Vertex({pointX, 0.6, pointY}, {.1, 0.5, 0.21}));
	tmp.push_back(Vertex({cos(2 * 3.141 * (i + 50) / 200), -0.6, sin(2 * 3.141 * (i + 50) / 200)}, {.81, 0.33, 0.81}));
  }
  objHyperboloid.setVertexBuffer(tmp);
  objHyperboloid.setIndexBuffer(tmp);
  objCube.setVertexBuffer({
							  Vertex({-1.0, -1.0, 1.0,}, {1.0, 0.0, 0.0}),
							  Vertex({1.0, -1.0, 1.0,}, {0.0, 1.0, 0.0,}),
							  Vertex({1.0, 1.0, 1.0,}, {0.0, 0.0, 1.0,}),
							  Vertex({-1.0, 1.0, 1.0,}, {1.0, 1.0, 1.0,}),
							  Vertex({-1.0, -1.0, -1.0,}, {1.0, 0.0, 0.0,}),
							  Vertex({1.0, -1.0, -1.0,}, {0.0, 1.0, 0.0,}),
							  Vertex({1.0, 1.0, -1.0,}, {0.0, 0.0, 1.0,}),
							  Vertex({-1.0, 1.0, -1.0}, {1.0, 1.0, 1.0}),
						  });

  IndexBuffer index_buffer0({1, 3, 5, 5, 7, 1});
  IndexBuffer index_buffer1({0, 2, 4, 4, 6, 0});
  IndexBuffer index_buffer2({0, 1, 2, 3, 2, 4, 4, 2, 5, 5, 0, 2});

  objCube.setIndexBuffer({0, 1, 2, 2, 3, 0,
							 // right
						  1, 5, 6, 6, 2, 1,
							 // back
						  7, 6, 5, 5, 4, 7,
							 // left
						  4, 0, 3, 3, 7, 4,
							 // bottom
						  4, 5, 1, 1, 0, 4,
							 // top
						  3, 2, 6, 6, 7, 3});
  objGeneral.init();
  objHyperboloid.init();
  objCube.init();

  float r = 0.0f;
  float increment = 0.05f;

  glm::mat4 projection = glm::perspective(
	  glm::radians(90.f),// Вертикальное поле зрения в радианах. Обычно между 90&deg; (очень широкое) и 30&deg; (узкое)
	  4.0f / 3.0f,       // Отношение сторон. Зависит от размеров вашего окна. Заметьте, что 4/3 == 800/600 == 1280/960
	  0.1f,              // Ближняя плоскость отсечения. Должна быть больше 0.
	  100.0f             // Дальняя плоскость отсечения.
  );
  // Или, для ортокамеры
  glm::mat4 view = glm::lookAt(
	  glm::vec3(0, 0, 1),// Камера находится в мировых координатах (4,3,3)
	  glm::vec3(0, 0, 0),// И направлена в начало координат
	  glm::vec3(0, 1, 0) // "Голова" находится сверху
  );
  // Матрица модели : единичная матрица (Модель находится в начале координат)
  glm::mat4 model = glm::mat4(1.0f);// Индивидуально для каждой модели

  glm::mat4 MVPmatrix = projection * view * model;// Запомните! В обратном порядке!
  /* Loop until the user closes the window */
  glfwSetKeyCallback(window.getWindow(), handleKeyboard);

  while (!glfwWindowShouldClose(window.getWindow())) {
	window.updateFpsCounter();
	Renderer::clear();
	if (selected_optionX < 3) {
	  view = glm::lookAt(
		  glm::vec3(0, 0, 1),// Камера находится в мировых координатах (4,3,3)
		  glm::vec3(0, 0, 0),// И направлена в начало координат
		  glm::vec3(0, 1, 0) // "Голова" находится сверху
	  );
	  model = glm::mat4(1.0f);// Индивидуально для каждой модели
	  MVPmatrix = projection * view * model;// Запомните! В обратном порядке!
	}

	lShader.bind();
	lShader.setUniformMat4f("u_MVP", MVPmatrix);

	switch (selected_optionX) {
	  case 0: objGeneral.setIndexBuffer(&index_buffer0);
		Renderer::draw(&objGeneral, &lShader);
		break;
	  case 1: objGeneral.setIndexBuffer(&index_buffer1);
		Renderer::draw(&objGeneral, &lShader);
		break;
	  case 2: objGeneral.setIndexBuffer(&index_buffer2);
		Renderer::draw(&objGeneral, &lShader);
		break;
	  case 3: {
		const float radius = 1.2f;
		float camX = sin(glfwGetTime()) * radius;
		float camY = cos(glfwGetTime()) * radius;
		float camZ = tan(glfwGetTime()) * camY + camX;
		view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		MVPmatrix = projection * view * model;// Запомните! В обратном порядке!
		Renderer::draw(&objHyperboloid, &lShader);
		break;
	  }
	  case 4: view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		MVPmatrix = projection * view * model;// Запомните! В обратном порядке!
		model = glm::rotate(model, 0.001f, {0, 0, 1});
		Renderer::draw(&objHyperboloid, &lShader);
		break;
	  case 5: uShader.bind();
		uShader.setUniformMat4f("u_MVP", MVPmatrix);
		uShader.setUniform4f("u_Color", {r, 0.4f, 0.7f, 1.0f});
		view = glm::lookAt(
			glm::vec3(0, 0, 1),// Камера находится в мировых координатах (4,3,3)
			glm::vec3(0, 0, 0),// И направлена в начало координат
			glm::vec3(0, 1, 0) // "Голова" находится сверху
		);
		model = glm::scale(
			glm::mat4(1.0f),
			glm::vec3(r)
		);
		MVPmatrix = projection * view * model;// Запомните! В обратном порядке!
		Renderer::draw(&objHyperboloid, &uShader);
		break;

	  case 6:
		view = glm::lookAt(
			glm::vec3(0, 0, 3),// Камера находится в мировых координатах (4,3,3)
			glm::vec3(0, 0, 0),// И направлена в начало координат
			glm::vec3(0, 1, 0) // "Голова" находится сверху
		);
		model = glm::mat4(1.0f);// Индивидуально для каждой модели
		MVPmatrix = projection * view * model;// Запомните! В обратном порядке!
		Renderer::draw(&objCube, &lShader);
		break;

	  case 7: lShader.bind();
		view = glm::lookAt(
			glm::vec3(0, 0, 2),// Камера находится в мировых координатах (4,3,3)
			glm::vec3(0, 0, 0),// И направлена в начало координат
			glm::vec3(0, 1, 0) // "Голова" находится сверху
		);
		model = glm::rotate(model, 0.004f, {0, 1, 0});
		MVPmatrix = projection * view * model;// Запомните! В обратном порядке!
		Renderer::draw(&cone2, &lShader);
		Renderer::draw(&cone1, &lShader);
		Renderer::draw(&cone0, &lShader);
		Renderer::draw(&cone00, &lShader);
		Renderer::draw(&cyl1, &lShader);
		break;
	  default:break;
	}

	switch (selected_optionY) {
	  case 0: glCall(glDisable(GL_DEPTH_TEST));
		glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		break;
	  case 1:
		// Включить тест глубины
	  glCall(glEnable(GL_DEPTH_TEST));
		// Фрагмент будет выводиться только в том, случае, если он находится ближе к камере, чем предыдущий
		glDepthFunc(GL_LESS);
		glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		break;
	  case 2: glCall(glPointSize(10));
		glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_POINT));
		break;

	  case 3: glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		break;

	  default: break;
	}

	if (r > 2.0f)
	  increment = -0.011f;
	else if (r < 0.0f)
	  increment = 0.01f;
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
