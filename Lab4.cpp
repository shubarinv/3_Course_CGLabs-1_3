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
  if ((key == GLFW_KEY_Q && action == GLFW_PRESS) && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
	spdlog::info("Got quit command, destroying window");
	glfwDestroyWindow(window);
	spdlog::info("Quiting...");
	glfwTerminate();
	exit(0);
  }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  spdlog::info("App stated!");
  Window window({800, 600});
  //Shader lShader("../resources/shaders/basic_w_layout.glsl");  ///< use this shader when you want to use layouts
  //Shader uShader("../resources/shaders/basic_w_uniforms.glsl");///< use this shader when you want to use uniforms
  Shader tShader("../resources/shaders/basic_w_texture.glsl");///< use this shader when you want to use uniforms

  glfwSetKeyCallback(window.getWindow(), handleKeyboard);
  Texture test("../resources/textures/noTex.png");
  test.bind();
  tShader.bind();
  tShader.setUniform1i("u_Texture", 0);
  Object testObj;
  float positions[] = {
	  -0.5f, -0.5f, 0.0f, 0.0f,
	  0.5f, -0.5f, 1.0f, 0.0f,
	  0.5f, 0.5f, 1.0f, 1.0f,
	  -0.5f, -0.5f, 0.0f, 1.0f
  };
  std::vector<unsigned int> indices = {
	  0, 1, 2,
	  2, 3, 0
  };

  testObj.setVertexBuffer(positions, sizeof(positions) / sizeof(positions[0]));
  testObj.setIndexBuffer(indices);
  testObj.setLayoutLength(2);
  testObj.setTimesToPushLayout(2);
  testObj.init();
  while (!glfwWindowShouldClose(window.getWindow())) {
	window.updateFpsCounter();
	Renderer::clear();

	Renderer::draw(&testObj, &tShader);

	/* Swap front and back buffers */
	glfwSwapBuffers(window.getWindow());

	/* Poll for and process events */
	glfwPollEvents();
  }
  window.destroy();
  spdlog::info("Program finished");
  return 0;
}