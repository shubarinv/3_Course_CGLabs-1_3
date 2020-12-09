//
// Created by Vladimir Shubarin on 03.12.2020.
//

#ifndef CGLABS__APPLICATION_HPP_
#define CGLABS__APPLICATION_HPP_

#include <map>
#include "window.hpp"

class Application {
  Window *window{};
  typedef void (*functionType)(int, int, Application *);
  std::map<int, functionType> keyPressCallbacks;
 public:
  [[nodiscard]] Window *getWindow() const {
	return window;
  }

  bool shouldClose{false};
  void init() {
	logInit();
	window = new Window({800,600});
	setOpenGLFlags();
	glfwSetKeyCallback(window->getGLFWWindow(), keyCallback);

	glfwSetWindowUserPointer(window->getGLFWWindow(), this);
  }

  void registerKeyCallback(int key, functionType func) {
	auto posOfPreviouslyRegisteredKeyPressCallbacks = keyPressCallbacks.find(key);
	if (posOfPreviouslyRegisteredKeyPressCallbacks != keyPressCallbacks.end()) {
	  PLOGV << "Call back for \"" << glfwGetKeyName(key, 0) << "\" was already registered";
	}
	keyPressCallbacks.emplace(key, func);
  }

  [[maybe_unused]] void unregisterKeyCallback(int key) {
	auto posOfPreviouslyRegisteredKeyPressCallbacks = keyPressCallbacks.find(key);
	if (posOfPreviouslyRegisteredKeyPressCallbacks != keyPressCallbacks.end()) {
	  keyPressCallbacks.erase(posOfPreviouslyRegisteredKeyPressCallbacks);
	}
  }

  [[maybe_unused]] static void setOpenGLFlags() {
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  void close() {
	shouldClose = true;
	//PLOGV << "Destroying all meshes!";
	//std::vector<Mesh>().swap(meshes);
	delete (window);
  }

  ~ Application() {
	PLOGV << "Application destroyed";
	delete (window);
  }

 private:
  void handleKeyboard([[maybe_unused]] GLFWwindow *_window, int key, int scancode, int action, [[maybe_unused]] int mods) {
	PLOGV << "KeyCallback: " << glfwGetKeyName(key, scancode);
	auto requiredCallback = keyPressCallbacks.find(key);
	if (requiredCallback != keyPressCallbacks.end()) {
	  PLOGV << "Calling registered callback";
	  requiredCallback->second(key, action, this);
	} else {
	  PLOGV << "No registered callback for " << glfwGetKeyName(key, scancode);
	}
  }

  static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	auto *app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
	app->handleKeyboard(window, key, scancode, action, mods);
  }

};

#endif //CGLABS__APPLICATION_HPP_
