//
// Created by Vladimir Shubarin on 22.12.2020.
//
#include <Magnum/GL/DebugOutput.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Platform/GlfwApplication.h>

using namespace Magnum;

#include "functions.hpp"
#include "input_handler.hpp"
#include "mesh.hpp"

class lab1 : public Platform::Application {
 public:
  explicit lab1(const Arguments &arguments) : Platform::Application{arguments, Configuration{}.setTitle("Lab 1")} {
	logInit(arguments.argc, arguments.argv);
	setWindowSize({640, 640});
  }

 private:
  void drawEvent() override {
	GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

	swapBuffers();
  }


};

MAGNUM_APPLICATION_MAIN(lab1)