//
// Created by Vladimir Shubarin on 12.10.2020.
//

#ifndef CGLABS__SHADER_HPP_
#define CGLABS__SHADER_HPP_

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "renderer.hpp"
class Shader {
  struct ShaderProgramSource {
	std::string vertexShader{};
	std::string fragmentShader{};
  };

 public:
  explicit Shader(const std::string &_filepath) {
	filepath   = _filepath;
	source     = parseShader();
	rendererID = createShader();
	spdlog::info("Created shader with id: {}", rendererID);
  }
  ~Shader() {
	glCall(glDeleteProgram(rendererID));
	spdlog::info("destroyed shader with id: {}", rendererID);
  }
  [[maybe_unused]] void bind() const {
	glCall(glUseProgram(rendererID));
  }
  [[maybe_unused]] static void unbind() {
	glCall(glUseProgram(0));
  }
  void setUniform4f(const std::string &name, glm::vec4 vec4) {
	glCall(glUniform4f(getUniformLocation(name), vec4.x, vec4.y, vec4.z, vec4.w));
  }

 private:
  ShaderProgramSource source;
  std::unordered_map<std::string, int> uniformLocationCache;
  [[nodiscard]] int getUniformLocation(const std::string &name) {
	if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
	  return uniformLocationCache[name];
	}
	glCall(int location = glGetUniformLocation(rendererID, "u_Color"));
	if (location == -1) {
	  spdlog::warn("Uniform with name {} does not exist", name);
	}

	uniformLocationCache[name] = location;
	return location;
  }
  unsigned int rendererID{};
  std::string filepath{};
  ShaderProgramSource parseShader() {
	spdlog::info("Parsing shader at: {}", filepath.c_str());
	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss[2];
	enum class shaderType {
	  NONE     = -1,
	  VERTEX   = 0,
	  FRAGMENT = 1
	};
	shaderType type = shaderType::NONE;
	while (std::getline(stream, line)) {
	  if (line.find("#shader") != std::string::npos) {
		if (line.find("vertex") != std::string::npos) {
		  type = shaderType::VERTEX;
		} else if (line.find("fragment") != std::string::npos) {
		  type = shaderType::FRAGMENT;
		}
	  } else {
		ss[(int)type] << line << "\n";
	  }
	}
	spdlog::info("Shader parsed successfully");
	return {ss[0].str(), ss[1].str()};
  }
  static unsigned int compileShader(int type, std::string &source) {
	spdlog::info("Trying to compile {}", (type == GL_VERTEX_SHADER ? "VertexShader " : "FragmentShader "));
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
	  int length;
	  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
	  std::string error;
	  char buf[length];
	  error = "Application::CompileShader() failed to compile ";
	  error += (type == GL_VERTEX_SHADER ? "VertexShader " : "FragmentShader ");
	  error += buf;
	  glGetShaderInfoLog(id, length, &length, buf);
	  spdlog::critical(error);
	  throw std::runtime_error(error);
	  glDeleteShader(id);
	}
	spdlog::info("shader compiled successfully");
	return id;
  }
  unsigned int createShader() {
	unsigned int program = glCreateProgram();
	unsigned int vShader = compileShader(GL_VERTEX_SHADER, source.vertexShader);
	unsigned int fShader = compileShader(GL_FRAGMENT_SHADER, source.fragmentShader);
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	return program;
  }
};

#endif//CGLABS__SHADER_HPP_
