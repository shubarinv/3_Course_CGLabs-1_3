//
// Created by Vladimir Shubarin on 02.12.2020.
//

#include "functions.hpp"
#include "application.hpp"
#include "shader.hpp"

void programQuit(int key, int action, Application *app) {
  app->close();
  PLOGV << "Quiting...";
}

int main(int argc, char *argv[]) {
  Application app;
  app.init();
  Application::setOpenGLFlags();
  app.registerKeyCallback(GLFW_KEY_ESCAPE, programQuit);
  glfwSetWindowUserPointer(app.getWindow()->getGLFWWindow(), &app);

  while (!app.shouldClose) {
	glClear(GL_COLOR_BUFFER_BIT);
	// 1rst attribute buffer : vertices
	glCall(glfwSwapBuffers(app.getWindow()->getGLFWWindow()));
	glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
