//
// Created by Vladimir Shubarin on 20.11.2020.
//

#define GL_SILENCE_DEPRECATION

#include "window.hpp"
#include "shader.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "obj_loader.hpp"
int selected_optionX = 0;
int selected_optionY = 0;
void handleKeyboard(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
  spdlog::info("Keyboard callback");
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
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
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  spdlog::info("App stated!");
  Window window({800, 600});
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

  auto objloader = new objLoader();
   auto loadedObj=VertexBuffer::floatToVertex(objloader->loadObj("../resources/objects/cube.obj"));
  auto object = new Object();
  object->setVertexBuffer(loadedObj);
  object->setIndexBuffer(loadedObj);
  object->setTexCoords(objloader->attrib.texcoords);
  //object->setOptimized(false);
  object->setTexture("../resources/textures/wood.png");
  object->init();
  tShader.setUniformMat4f("u_MVP", MVPmatrix);
  glfwSetKeyCallback(window.getWindow(), handleKeyboard);
  while (!glfwWindowShouldClose(window.getWindow())) {

	window.updateFpsCounter();
	Renderer::clear();

	Renderer::draw(object,&tShader);
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
	glfwSwapBuffers(window.getWindow());

	/* Poll for and process events */
	glfwPollEvents();

  }
  window.destroy();
  spdlog::info("Program finished");
  return 0;
}