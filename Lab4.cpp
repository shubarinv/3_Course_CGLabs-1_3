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

  //Shader lShader("../resources/shaders/basic_w_layout.glsl");  ///< use this shader when you want to use layouts
  //Shader uShader("../resources/shaders/basic_w_uniforms.glsl");///< use this shader when you want to use uniforms
  Shader tShader("../resources/shaders/basic_w_texture.glsl");///< use this shader when you want to use uniforms

  glfwSetKeyCallback(window.getWindow(), handleKeyboard);

  glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  glCall(glEnable(GL_BLEND));

  Texture test("../resources/textures/Unknown.png");
  test.bind();
  tShader.bind();
  tShader.setUniform1i("u_Texture", 0);
  float positions[] = {
	  -0.5f, -0.5f,
	  0.5f, -0.5f,
	  0.5f, 0.5f,
	  -0.5f, 0.5f,
  };
  float texCoords[] = {
	  0.0f, 0.0f,
	  1.0f, 0.0f,
	  1.0f, 1.0f,
	  0.0f, 1.0f
  };
  unsigned int indices[] = {
	  0, 1, 2,
	  2, 3, 0
  };

  VertexArray va;
  VertexBuffer vb(positions, 8 * sizeof(float));
  VertexBuffer vbTex(texCoords, 8 * sizeof(float));
  VertexBufferLayout layout;
  layout.push<float>(2);
  va.addBuffer(vb, layout);
  va.addBuffer(vbTex, layout, 1);

  IndexBuffer ib(indices, 6);
  while (!glfwWindowShouldClose(window.getWindow())) {

	window.updateFpsCounter();
	Renderer::clear();

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