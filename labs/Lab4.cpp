//
// Created by Vladimir Shubarin on 20.11.2020.
//

#define GL_SILENCE_DEPRECATION

#include "../window.hpp"
#include "../shader.hpp"
#include "../renderer.hpp"
#include "../texture.hpp"
#include "../application.hpp"
int selected_optionX = 0;
int selected_optionY = 0;
void programQuit(int key, int action, Application *app) {
  app->close();
  PLOGV << "Quiting...";
}
void changeDrawMode(int key, int action, [[maybe_unused]] Application *app) {
  if (action == GLFW_PRESS) {
	if (key == GLFW_KEY_UP) {
	  selected_optionY--;
	} else if (key == GLFW_KEY_DOWN) {
	  selected_optionY++;
	} else {
	  PLOGE << "How the hell did we end up here?";
	}
	PLOGV << "Draw mode is now " << selected_optionY;
  }
}
void changeTask(int key, int action, [[maybe_unused]] Application *app) {
  if (action == GLFW_PRESS) {
	if (key == GLFW_KEY_LEFT) {
	  selected_optionX--;
	} else if (key == GLFW_KEY_RIGHT) {
	  selected_optionX++;
	} else {
	  PLOGE << "How the hell did we end up here?";
	}
	PLOGV << "Task is now " << selected_optionX;
  }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  Application app;
  app.init();
  app.registerKeyCallback(GLFW_KEY_ESCAPE, programQuit);
  app.registerKeyCallback(GLFW_KEY_LEFT, changeTask);
  app.registerKeyCallback(GLFW_KEY_RIGHT, changeTask);
  app.registerKeyCallback(GLFW_KEY_UP, changeDrawMode);
  app.registerKeyCallback(GLFW_KEY_DOWN, changeDrawMode);
  Application::setOpenGLFlags();

  Shader tShader("../resources/shaders/basic_w_texture.glsl");
  glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  glCall(glEnable(GL_BLEND));
  //glCall(glEnable(GL_CULL_FACE));
  glCall(glEnable(GL_MULTISAMPLE));
  glCall(glEnable(GL_DEPTH_TEST));
  glCall(glDepthFunc(GL_LESS));
  tShader.bind();
  glm::mat4 projection = glm::perspective(
	  glm::radians(90.f),// Вертикальное поле зрения в радианах. Обычно между 90&deg; (очень широкое) и 30&deg; (узкое)
	  4.0f / 3.0f,       // Отношение сторон. Зависит от размеров вашего окна. Заметьте, что 4/3 == 800/600 == 1280/960
	  0.1f,              // Ближняя плоскость отсечения. Должна быть больше 0.
	  100.0f             // Дальняя плоскость отсечения.
  );
  // Или, для ортокамеры
  glm::mat4 view = glm::lookAt(
	  glm::vec3(0, 0, 3),// Камера находится в мировых координатах (4,3,3)
	  glm::vec3(0, 0, 0),// И направлена в начало координат
	  glm::vec3(0, 1, 0) // "Голова" находится сверху
  );
  // Матрица модели : единичная матрица (Модель находится в начале координат)
  glm::mat4 model = glm::mat4(1.0f);// Индивидуально для каждой модели

  glm::mat4 MVPmatrix = projection * view * model;// Запомните! В обратном порядке!

  Object testObj;

  testObj.setVertexBuffer({
							  Vertex({-1.0, -1.0, 1.0,}),
							  Vertex({-1.0, 1.0, 1.0,}),
							  Vertex({1.0, 1.0, 1.0,}),
							  Vertex({1.0, -1.0, 1.0,}),
							  Vertex({-1.0, -1.0, -1.0,}),
							  Vertex({-1.0, 1.0, -1.0,}),
							  Vertex({1.0, 1.0, -1.0,}),
							  Vertex({1.0, -1.0, -1.0,}),
						  });
  testObj.setIndexBuffer({
							 0, 1, 2,
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

  testObj.setTexture("../resources/textures/wood.png");
  testObj.init();
  tShader.setUniform1i("u_Texture", 0);

  tShader.setUniformMat4f("u_MVP", MVPmatrix);
  while (!app.shouldClose) {
	model = glm::rotate(model, 0.006f, {0, 1, 1});
	glm::mat4 MVPmatrix = projection * view * model;// Запомните! В обратном порядке!
	tShader.setUniformMat4f("u_MVP", MVPmatrix);
	Renderer::clear();

	Renderer::draw(&testObj, &tShader);
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
	/* Swap front and back buffers */
	glCall(glfwSwapBuffers(app.getWindow()->getGLFWWindow()));

	/* Poll for and process events */
	glfwPollEvents();

  }
  glfwTerminate();
  return 0;
}