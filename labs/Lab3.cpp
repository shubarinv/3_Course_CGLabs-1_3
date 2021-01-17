
#define GL_SILENCE_DEPRECATION

//#include <cmath>

#include "../Buffers/color_buffer.hpp"
#include "../Buffers/index_buffer.hpp"
#include "../Buffers/vertex_array.hpp"
#include "../Shapes/cone.hpp"
#include "../Shapes/cylinder.hpp"
#include "../application.hpp"
#include "../camera.hpp"
#include "../renderer.hpp"

int selected_optionX = 0;
int selected_optionY = 0;

/**
 * @brief Вызывается при нажатии на ESC
 * @param key
 * @param action
 * @param app Ссылка на экземпляр приложения которое надо завершить
 */
void programQuit([[maybe_unused]] int key, [[maybe_unused]] int action, Application *app) {
  LOG_S(INFO) << "Quiting...";
  app->close();
}

/**
 * @brief Меняет режим отрисовки
 * @param key
 * @param action
 * @param app Ссылка на экземпляр приложения которое которое вызвало функцию
 */
void changeDrawMode(int key, int action, [[maybe_unused]] Application *app) {
  if (action == GLFW_PRESS) {
	if (key == GLFW_KEY_UP) {
	  selected_optionY--;
	} else if (key == GLFW_KEY_DOWN) {
	  selected_optionY++;
	} else {
	  LOG_S(ERROR) << "How the hell did we end up here?";
	}
	LOG_S(INFO) << "Draw mode is now " << selected_optionY;
  }
}
/**
 * @brief меняет номер задания
 * @param key
 * @param action
 * @param app Ссылка на экземпляр приложения которое которое вызвало функцию
 */
void changeTask(int key, int action, [[maybe_unused]] Application *app) {
  if (action == GLFW_PRESS) {
	if (key == GLFW_KEY_LEFT) {
	  selected_optionX--;
	} else if (key == GLFW_KEY_RIGHT) {
	  selected_optionX++;
	} else {
	  LOG_S(ERROR) << "How the hell did we end up here?";
	}
	LOG_S(INFO) << "Task is now " << selected_optionX;
  }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  Application app;
  app.init({720, 480}, argc, argv);
  app.registerKeyCallback(GLFW_KEY_ESCAPE, programQuit);
  app.registerKeyCallback(GLFW_KEY_LEFT, changeTask);
  app.registerKeyCallback(GLFW_KEY_RIGHT, changeTask);
  app.registerKeyCallback(GLFW_KEY_UP, changeDrawMode);
  app.registerKeyCallback(GLFW_KEY_DOWN, changeDrawMode);
  Application::setOpenGLFlags();

  Shader lShader("../resources/shaders/basic_w_layout.glsl");  ///< use this shader when you want to use layouts
  Shader uShader("../resources/shaders/basic_w_uniforms.glsl");///< use this shader when you want to use uniforms
  LOG_SCOPE_F(INFO, "Objects and buffers init");
  Object objGeneral;    ///< объект используемый в 1-3 заданиях
  Object objHyperboloid;///< объект используемый в 4-7 заданиях
  Object objCube;       /// 8 задание

  //Cone cone3({0, 0.6, 0}, 0.3, 0.3, 4, {.49, 0.8, 0.5});         ///< верхушка елки
  Cone cone2({0, 0.3, 0}, 0.4, 0.5, 12, {.7, 0.9, 0.7});         ///< часть елки
  Cone cone1({0, -0.2, 0}, 0.55, 1, 12, {.6, 0.8, 0.6});         ///< часть елки
  Cone cone0({0, -0.6, 0}, 0.7, 1, 12, {.4, 0.8, 0.4});          ///< нижняя часть елки
  Cylinder trunk({0, -0.9, 0}, 0.2, 1.19, 12, {.37, 0.20, 0.21});///< ствол елки

  objGeneral.setVertexBuffer({
	  /// кординаты для объекта из 1-3 заданий
	  Vertex({0.0f, 0.84853f, 0}, {1, 0.0, 0.0}),
	  Vertex({-0.6f, 0.6f, 0}, {1, 1, 0.0}),
	  Vertex({-0.84853f, 0, 0}, {0, 1, 0}),
	  Vertex({-0.6f, -0.6f, 0}, {.0, 1, 1}),
	  Vertex({0.f, -0.84853f, 0}, {.0, 0.0, 1}),
	  Vertex({0.6f, -0.6f, 0}, {1, 0, 1}),
	  Vertex({0.84853f, 0.0f, 0}, {1, 1, 0}),
	  Vertex({0.6f, 0.6f, 0}, {0, 1, 0}),
  });

  std::vector<Vertex> tmp;/// координаты для точек гиперболойды
  for (int i = 0; i <= 200; i++) {
	double angle = 2 * 3.141 * i / 200;
	double pointX = cos(angle);
	double pointY = sin(angle);
	tmp.push_back(Vertex({pointX, 0.6, pointY}, {.6, 0.7, 0.2}));
	tmp.push_back(Vertex({cos(2 * 3.141 * (i + 50) / 200), -0.6, sin(2 * 3.141 * (i + 50) / 200)}, {.8, 0.5, 0.4}));
  }
  objHyperboloid.setVertexBuffer(tmp);///<Задаем значение буферу вершин
  objHyperboloid.setIndexBuffer(tmp); ///<Задаем порядок отрисовки вершин вершин
  objCube.setVertexBuffer({
	  ///< Координаты для куба и его цвет
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
				 1.0,
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
				 0.0,
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

  IndexBuffer index_buffer0({1, 3, 5, 5, 7, 1});                  ///< то в каком порядке будут отрисовываться вершины в 1 задании
  IndexBuffer index_buffer1({0, 2, 4, 4, 6, 0});                  ///< то в каком порядке будут отрисовываться вершины в 2 задании
  IndexBuffer index_buffer2({0, 1, 2, 3, 2, 4, 4, 2, 5, 5, 0, 2});///< то в каком порядке будут отрисовываться вершины в 3 задании

  objCube.setIndexBuffer({0, 1, 2, 2, 3, 0,///< то в каком порядке будут отрисовываться вершины куба
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

  Camera camera(app.getWindow()->getWindowSize());
  camera.moveTo({0, 0, 1});
  camera.lookAt({0, 0, 0});

  LOG_SCOPE_F(INFO, "Runtime");
  double lasttime = glfwGetTime();
  while (!app.shouldClose) {

	app.getWindow()->updateFpsCounter();
	Renderer::clear({0.12, 0.1, 0.21, 0});///< отчисляем экран указаным цветом
	if (selected_optionX < 3) {
	  camera.moveTo({0, 0, 1});
	  camera.lookAt({0, 0, 0});
	}

	lShader.bind();
	lShader.setUniformMat4f("u_MVP", camera.getMVP());

	switch (selected_optionX) {
	  case 0:
		objGeneral.setIndexBuffer(&index_buffer0);
		Renderer::draw(&objGeneral, &lShader);
		break;
	  case 1:
		objGeneral.setIndexBuffer(&index_buffer1);
		Renderer::draw(&objGeneral, &lShader);
		break;
	  case 2:
		objGeneral.setIndexBuffer(&index_buffer2);
		Renderer::draw(&objGeneral, &lShader);
		break;
	  case 3: {
		const float radius = 1.2f;
		float camX = sin(glfwGetTime()) * radius;
		float camY = cos(glfwGetTime()) * radius;
		float camZ = tan(glfwGetTime()) * camY + camX;
		camera.moveTo({camX, camY, camZ});
		Renderer::draw(&objHyperboloid, &lShader);
		break;
	  }
	  case 4:
		camera.lookAt({0, 0, 0});
		camera.moveTo({0, 0, 3});
		camera.setModel(glm::rotate(camera.getModel(), -0.01f, {0, 0, 1}));
		Renderer::draw(&objHyperboloid, &lShader);
		break;
	  case 5:
		uShader.bind();
		uShader.setUniformMat4f("u_MVP", camera.getMVP());
		uShader.setUniform4f("u_Color", {0.8, r, 0.2f, 1.0f});
		camera.moveTo({0, 0, 1});
		camera.lookAt({0, 0, 0});
		camera.setModel(glm::scale(glm::mat4(0.1f), glm::vec3(r * 0.2)));
		Renderer::draw(&objHyperboloid, &uShader);
		break;

	  case 6:
		camera.moveTo({0, 2, 3});
		camera.lookAt({0, 0, 0});
		camera.setModel(glm::mat4(1.0f));// Индивидуально для каждой модели
		Renderer::draw(&objCube, &lShader);
		break;

	  case 7:
		lShader.bind();
		camera.moveTo({0, 0, 2});
		camera.lookAt({0, 0, 0});
		camera.setModel(glm::rotate(camera.getModel(), 0.004f, {0, 1, 0}));
		Renderer::draw(&cone0, &lShader);
		Renderer::draw(&cone1, &lShader);
		Renderer::draw(&cone2, &lShader);
		//Renderer::draw(&cone3, &lShader);
		Renderer::draw(&trunk, &lShader);
		break;
	  default: break;
	}

	switch (selected_optionY) {
	  case 0:
		glCall(glDisable(GL_DEPTH_TEST));
		glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		break;
	  case 1:
		// Включить тест глубины
		glCall(glEnable(GL_DEPTH_TEST));
		// Фрагмент будет выводиться только в том, случае, если он находится ближе к камере, чем предыдущий
		glDepthFunc(GL_LESS);
		glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		break;
	  case 2:
		glCall(glPointSize(10));
		glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_POINT));
		break;

	  case 3:
		glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		break;

	  default: break;
	}

	if (r > 2.0f)
	  increment = -0.011f;
	else if (r < 0.0f)
	  increment = 0.01f;
	r += increment;

	/* Swap front and back buffers */
	glCall(glfwSwapBuffers(app.getWindow()->getGLFWWindow()));

	/* Poll for and process events */
	glfwPollEvents();
	while (glfwGetTime() < lasttime + 1.0/60) {
// TODO: Put the thread to sleep, yield, or simply do nothing
	}
	lasttime += 1.0/60;
  }
  LOG_S(INFO) << "Goodbye.";
  return 0;
}
