//
// Created by Vladimir Shubarin on 29.11.2020.
//

#ifndef CGLABS__C_MAP_TEXTURE_HPP_
#define CGLABS__C_MAP_TEXTURE_HPP_


class [[deprecated("DOES NOT WORK. NEEDS REWRITE")]] CMapTexture : public Buffer {
 private:
  unsigned int rendererID{};
  std::string filepath{};
  std::vector<unsigned char> localBuffer{};
  unsigned int width{0}, height{0};
 public:
  explicit CMapTexture(std::string _filepath, bool oneForAll = false) {
	filepath = std::move(_filepath);
	glCall(glGenTextures(1, &rendererID));
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, rendererID));

	if (oneForAll) {
	  for (unsigned int i = 0; i < 6; i++) {
		load(filepath);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, localBuffer.data());
	  }
	} else {
	  std::vector<std::string> pathToFaces = generateFilePaths();
	  for (unsigned int i = 0; i < pathToFaces.size(); i++) {
		load(pathToFaces[i].c_str());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, width, height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, localBuffer.data());
	  }
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	unbind();
  }
  ~CMapTexture() {
	glCall(glDeleteTextures(1, &rendererID));
  }
  void bind(unsigned int slot = 0) const {
	glCall(glActiveTexture(GL_TEXTURE0 + slot));
	glCall(glBindTexture(GL_TEXTURE_CUBE_MAP, rendererID));
  }
  [[maybe_unused]] static void unbind() {
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
  }
  [[maybe_unused]] [[nodiscard]] unsigned int getWidth() const { return width; }
  [[maybe_unused]] [[nodiscard]] unsigned int getHeight() const { return height; }

  void load(std::string _filepath) {
	unsigned error = lodepng::decode(localBuffer, width, height, _filepath);
	// If there's an error, display it.
	if (error != 0) {
	  LOG_S(ERROR) <<"Texture ("<<_filepath<<") load error "<< error<<": "<<lodepng_error_text(error);
	  throw;
	}
	LOG_S(INFO) <<"Texture loaded successfully!";
  }
  std::vector<std::string> generateFilePaths() {
	std::vector<std::string> paths;
	paths.push_back(filepath + "_PX"); // right
	paths.push_back(filepath + "_NX"); // left
	paths.push_back(filepath + "_PY"); // top
	paths.push_back(filepath + "_NY"); // bottom
	paths.push_back(filepath + "_PZ"); // back
	paths.push_back(filepath + "_NZ"); // front
	return paths;
  }
};

#endif //CGLABS__C_MAP_TEXTURE_HPP_
