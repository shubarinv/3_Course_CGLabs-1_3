//
// Created by Vladimir Shubarin on 20.11.2020.
//

#define GL_SILENCE_DEPRECATION

#include "window.hpp"
#include "shader.hpp"
#include "renderer.hpp"
#include "texture.hpp"
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

  Shader tShader("../resources/shaders/basic_w_texture.glsl");///< use this shader when you want to use textures

  glfwSetKeyCallback(window.getWindow(), handleKeyboard);

  glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  glCall(glEnable(GL_BLEND));
  glCall(glEnable(GL_CULL_FACE));
  glCall(glEnable(GL_MULTISAMPLE));
  glCall(glEnable(GL_DEPTH_TEST));
  glCall(glDepthFunc(GL_LESS));

  Texture test("../resources/textures/Unknown.png");
  test.bind();
  tShader.bind();
  tShader.setUniform1i("u_Texture", 0);
  float positions[] = {
	  -1.0, -1.0, 1.0,
	  1.0, -1.0, 1.0,
	  1.0, 1.0, 1.0,
	  -1.0, 1.0, 1.0,
	  -1.0, -1.0, -1.0,
	  1.0, -1.0, -1.0,
	  1.0, 1.0, -1.0,
	  -1.0, 1.0, -1.0,
  };
  float texCoords[] = {
	  0.0f, 0.0f, 0.0f,
	  1.0f, 0.0f, 0.0f,
	  1.0f, 1.0f, 0.0f,
	  0.0f, 1.0f, 0.0f,
  };
  unsigned int indices[] = {
	  0, 1, 2, 2, 3, 0,
	  // right
	  1, 5, 6, 6, 2, 1,
	  // back
	  7, 6, 5, 5, 4, 7,
	  // left
	  4, 0, 3, 3, 7, 4,
	  // bottom
	  4, 5, 1, 1, 0, 4,
	  // top
	  3, 2, 6, 6, 7, 3
  };

  VertexArray va;
  VertexBuffer vb(positions, 8 * 3 * sizeof(float));
  VertexBuffer vbTex(texCoords, 3 * 3 * 5 * sizeof(float));
  VertexBufferLayout layout;
  layout.push<float>(3);
  va.addBuffer(vb, layout);

  va.addBuffer(vbTex, layout, 1);

  IndexBuffer ib(indices, 6 * 6);

  glm::mat4 projection = glm::perspective(
	  glm::radians(90.f),// Вертикальное поле зрения в радианах. Обычно между 90&deg; (очень широкое) и 30&deg; (узкое)
	  4.0f / 3.0f,       // Отношение сторон. Зависит от размеров вашего окна. Заметьте, что 4/3 == 800/600 == 1280/960
	  0.1f,              // Ближняя плоскость отсечения. Должна быть больше 0.
	  100.0f             // Дальняя плоскость отсечения.
  );
  // Или, для ортокамеры
  glm::mat4 view = glm::lookAt(
	  glm::vec3(0, 0, 4),// Камера находится в мировых координатах (4,3,3)
	  glm::vec3(0, 0, 0),// И направлена в начало координат
	  glm::vec3(0, 1, 0) // "Голова" находится сверху
  );
  // Матрица модели : единичная матрица (Модель находится в начале координат)
  glm::mat4 model = glm::mat4(1.0f);// Индивидуально для каждой модели

  glm::mat4 MVPmatrix = projection * view * model;// Запомните! В обратном порядке!
  tShader.bind();
  tShader.setUniformMat4f("u_MVP", MVPmatrix);

  while (!glfwWindowShouldClose(window.getWindow())) {

	window.updateFpsCounter();
	Renderer::clear();

	model = glm::rotate(model, 0.006f, {1, 0, 0});
	MVPmatrix = projection * view * model;// Запомните! В обратном порядке!
	tShader.setUniformMat4f("u_MVP", MVPmatrix);
	Renderer::draw(&va, &ib, &tShader);

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