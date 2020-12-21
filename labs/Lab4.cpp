//
// Created by Vladimir Shubarin on 20.11.2020.
//

#define GL_SILENCE_DEPRECATION

#include "../application.hpp"
#include "../camera.hpp"
#include "../light.hpp"
#include "../renderer.hpp"

std::vector<glm::vec3> getCoordsForVertices(double xc, double yc, double size, int n) {
  std::vector<glm::vec3> vertices;
  auto xe = xc + size;
  auto ye = yc;
  vertices.emplace_back(xe, yc, ye);
  double alpha = 0;
  for (int i = 0; i < n - 1; i++) {
	alpha += 2 * M_PI / n;
	auto xr = xc + size * cos(alpha);
	auto yr = yc + size * sin(alpha);
	xe = xr;
	ye = yr;
	vertices.emplace_back(xe, yc, ye);
  }
  return vertices;
}
int selected_optionX = 0;
int selected_optionY = 0;
void programQuit([[maybe_unused]] int key, [[maybe_unused]] int action, Application *app) {
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

  Shader lShader("../playground/lighting.glsl");
  Shader normalShader("../resources/shaders/basic_w_layout.glsl");
  lShader.bind();
  Camera camera(app.getWindow()->getWindowSize());
  camera.moveTo({0, 0, 1.5});
  camera.lookAt({0, 0, 0});
  Light light(-1);
  Object testObj;

  testObj.setVertexBuffer({
	  ///< Координаты для куба и его цвет
	  Vertex({-0.5f, -0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({0.5f, -0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({0.5f, 0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({0.5f, 0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, 0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, -0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, -0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({0.5f, -0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({0.5f, 0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({0.5f, 0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, 0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, -0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, 0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, 0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, -0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, -0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, -0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, 0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({0.5f, 0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({0.5f, 0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({0.5f, -0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({0.5f, -0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({0.5f, -0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({0.5f, 0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, -0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({0.5f, -0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({0.5f, -0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({0.5f, -0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, -0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, -0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, 0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({0.5f, 0.5f, -0.5f}, {0, 1, 0}),
	  Vertex({0.5f, 0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({0.5f, 0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, 0.5f, 0.5f}, {0, 1, 0}),
	  Vertex({-0.5f, 0.5f, -0.5f}, {0, 1, 0}),
  });
  testObj.setIndexBuffer({0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35});

  testObj.init();
  auto objVertices = testObj.getVertexBuffer()->getVertices();
  auto objNormals = testObj.getNormals();
  std::vector<Vertex> normalsVertices;
  for (int i = 0; i < objVertices.size(); i++) {
	normalsVertices.push_back(Vertex({objVertices[i].getPosition()}, {1, 0, 0}));
	normalsVertices.push_back(Vertex({objVertices[i].getPosition() + glm::vec3(0.2, 0.2, 0.2) * objNormals[i]}, {1, 0, 0}));
  }
  std::vector<unsigned int> normalsIndices;
  normalsIndices.reserve(normalsVertices.size());
  for (int i = 0; i < normalsVertices.size(); ++i) {
	normalsIndices.emplace_back(i);
  }
  Object normals;
  normals.setVertexBuffer(normalsVertices);
  normals.setIndexBuffer(normalsIndices);
  normals.init(false);
  std::vector<glm::vec3> cameraPositions = getCoordsForVertices(0, 0, 2, 600);/// координаты для точек гиперболойды
  int cameraPosition{0};
  while (!app.shouldClose) {
	camera.setModel(glm::rotate(camera.getModel(), 0.006f, {0, 1, 0}));
	lShader.bind();
	lShader.setUniformMat4f("model", camera.getModel());
	lShader.setUniformMat4f("view", camera.getView());
	lShader.setUniformMat4f("projection", camera.getProjection());
	lShader.setUniform3f("lightPos", cameraPositions[cameraPosition]);
	lShader.setUniform3f("lightColor", {0, 1, 1});
	normalShader.bind();
	normalShader.setUniformMat4f("u_MVP", camera.getMVP());
	Renderer::clear();

	Renderer::draw(&testObj, &lShader);
	Renderer::draw(&normals, &normalShader, GL_LINES);
	cameraPosition++;
	if (cameraPosition >= cameraPositions.size()) {
	  cameraPosition = 0;
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
	/* Swap front and back buffers */
	glCall(glfwSwapBuffers(app.getWindow()->getGLFWWindow()));

	/* Poll for and process events */
	glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}