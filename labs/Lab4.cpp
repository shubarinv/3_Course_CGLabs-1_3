//
// Created by Vladimir Shubarin on 20.11.2020.
//

#define GL_SILENCE_DEPRECATION

#include "../application.hpp"
#include "../camera.hpp"
#include "../renderer.hpp"
#include "../shader.hpp"
#include "../texture.hpp"
#include "../window.hpp"
int selected_optionX = 0;
int selected_optionY = 0;
void programQuit(int key, int action, Application *app) {
  app->close();
  LOG_S(INFO) << "Quiting...";
}
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

  Shader lShader("../resources/shaders/basic_w_layout.glsl");
  lShader.bind();
  Camera camera(app.getWindow()->getWindowSize());
//003
  camera.moveTo({0,0,3});
  camera.lookAt({0, 0, 0});

  Object testObj;

  testObj.setVertexBuffer({
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
  testObj.setIndexBuffer({0, 1, 2, 2, 3, 0,///< то в каком порядке будут отрисовываться вершины куба
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

  testObj.init();


  while (!app.shouldClose) {
	camera.setModel(glm::rotate(camera.getModel(), 0.006f, {0, 1, 1}));
	lShader.setUniformMat4f("u_MVP", camera.getMVP());
	Renderer::clear();

	Renderer::draw(&testObj, &lShader);
	
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
	/* Swap front and back buffers */
	glCall(glfwSwapBuffers(app.getWindow()->getGLFWWindow()));

	/* Poll for and process events */
	glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}