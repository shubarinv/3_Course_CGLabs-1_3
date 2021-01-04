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
 public:

  void init() {
	logInit();
	window = new Window;
	setOpenGLFlags();
	glfwSetKeyCallback(window->getGLFWWindow(), keyCallback);
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

  void handleKeyboard([[maybe_unused]] GLFWwindow *_window, int key, int scancode, int action, [[maybe_unused]] int mods) {
	PLOGV << "KeyCallback: " << glfwGetKeyName(key, scancode);
	for (auto &callback:keyPressCallbacks) {
	  if (callback.first == key) {
		PLOGV << "Calling registered callback";
		callback.second(key, action, this);
	  }
	}
  }

  void close() {
	shouldClose = true;
	delete (window);
  }
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	auto *app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
	app->handleKeyboard(window, key, scancode, action, mods);
  }
  ~
  Application() {
	PLOGV << "Application destroyed";
	delete (window);
  }
};

#endif //CGLABS__APPLICATION_HPP_
