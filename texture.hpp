//
// Created by Vladimir Shubarin on 20.11.2020.
//

#ifndef CGLABS__TEXTURE_HPP_
#define CGLABS__TEXTURE_HPP_
#include <utility>
#include "Libs/lodepng.hpp"
#include "functions.hpp"
class Texture : public Buffer {
 private:
  unsigned int rendererID{};
  std::string filepath{};
  std::vector<unsigned char> localBuffer{};
  unsigned int width{0}, height{0};
 public:
  explicit Texture(std::string _filepath) {
	filepath = std::move(_filepath);
	glCall(glGenTextures(1, &rendererID));
	glCall(glBindTexture(GL_TEXTURE_2D, rendererID));
	load();
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	glCall(glGenerateMipmap(GL_TEXTURE_2D));
	glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer.data()));
	unbind();
  }
  ~Texture() {
	glCall(glDeleteTextures(1, &rendererID));
  }
  void bind(unsigned int slot = 0) const {
	glCall(glActiveTexture(GL_TEXTURE0 + slot));
	glCall(glBindTexture(GL_TEXTURE_2D, rendererID));
  }
  [[maybe_unused]] static void unbind() {
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
  }
  [[nodiscard]] unsigned int getWidth() const { return width; }
  [[nodiscard]] unsigned int getHeight() const { return height; }

  void load() {
	unsigned error = lodepng::decode(localBuffer, width, height, filepath);
	// If there's an error, display it.
	if (error != 0) {
	  LOG_S(INFO) << "Texture load error" << error << " : " << lodepng_error_text(error);
	  throw;
	}
	LOG_S(INFO) << "Texture loaded successfully!";
  }

  std::vector<float> generateTextureCoords(unsigned int size) {
	float texCoordsPreset[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f};
	std::vector<float> textureCoords;
	short presetNum{0};
	for (int i = 0; i < size; i++) {
	  if (presetNum > 11)presetNum = 0;
	  textureCoords.push_back(texCoordsPreset[presetNum]);
	  textureCoords.push_back(texCoordsPreset[presetNum + 1]);
	  presetNum += 2;
	}

	return textureCoords;
  }
};

#endif //CGLABS__TEXTURE_HPP_
