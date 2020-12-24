//
// Created by Vladimir Shubarin on 22.12.2020.
//
#include <Magnum/GL/DebugOutput.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Platform/GlfwApplication.h>

using namespace Magnum;

#include "functions.hpp"
#include "mesh.hpp"

class lab2 : public Platform::Application {
  typedef Magnum::Platform::GlfwApplication::KeyEvent::Key Key;

 public:
  explicit lab2(const Arguments &arguments) : Platform::Application{arguments, Configuration{}.setTitle("Lab 2")} {
	logInit(arguments.argc, arguments.argv);
	setWindowSize({640, 640});

	mesh = new Mesh{{
		Vertex({0.0f, 0.84853f, 0}, {0.5, 1, 0}),
		Vertex({-0.6f, 0.6f, 0}, {0.5, 1, 0}),
		Vertex({-0.84853f, 0, 0}, {0.5, 1, 0}),
		Vertex({-0.6f, -0.6f, 0}, {0.5, 1, 0}),
		Vertex({0.f, -0.84853f, 0}, {0.5, 1, 0}),
		Vertex({0.6f, -0.6f, 0}, {0.5, 1, 0}),
		Vertex({0.84853f, 0.0f, 0}, {0.5, 1, 0}),
		Vertex({0.6f, 0.6f, 0}, {0.5, 1, 0}),
	}};

	indices.push_back({0, 1, 2, 3, 2, 4, 4, 2, 5, 5, 2, 6, 6, 2, 7, 7, 2, 0});
	indices.push_back({0, 2, 4, 4, 6, 0, 1, 3, 5, 5, 7, 1, 0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 0});
	indices.push_back({0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 0});
	indices.push_back({1, 3, 5});
	indices.push_back({1, 2, 6, 6, 7, 1});
	indices.push_back({0, 1, 2, 3, 2, 4, 4, 2, 5, 5, 0, 2});
	indices.push_back({2, 3, 6, 6, 7, 2});
	indices.push_back({1, 3, 5, 5, 7, 1});
	indices.push_back({0, 1, 4, 4, 7, 0});
	indices.push_back({0, 2, 4, 4, 6, 0});
  }

  /// Variables and functions
 private:
  Mesh *mesh;
  Shaders::VertexColor3D shader;
  unsigned int selectedTask{0};
  std::vector<std::vector<unsigned int>> indices;

  ///Draw function
  void drawEvent() override {
	GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
	mesh->setIndexBuffer(indices[selectedTask]);
	mesh->draw(&shader);

	swapBuffers();
	redraw();
  }

  /// Input handling
 private:
  void keyPressEvent(KeyEvent &event) override {
	LOG_S(INFO) << "(Keyboard)::PressedKey: " << event.keyName();
	if (event.key() == Key::Esc) {
	  quit();
	}
	if (event.key() == Key::Left) {
	  selectedTask--;
	  if (selectedTask == -1) { selectedTask = indices.size() - 1; }
	  LOG_S(INFO) << "Selected task: " << selectedTask;
	}
	if (event.key() == Key::Right) {
	  selectedTask++;
	  if (selectedTask > indices.size() - 1) {
		selectedTask = 0;
	  }
	  LOG_S(INFO) << "Selected task: " << selectedTask;
	}
  }

  void quit() {
	LOG_S(INFO) << "Goodbye!";
	this->exit();
  }
};

MAGNUM_APPLICATION_MAIN(lab2)