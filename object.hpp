//
// Created by Vladimir Shubarin on 16.11.2020.
//

#ifndef CGLABS__OBJECT_HPP_
#define CGLABS__OBJECT_HPP_

#include <utility>

#include "Buffers/index_buffer.hpp"
#include "renderer.hpp"
#include "Buffers/color_buffer.hpp"
#include "texture.hpp"

class Object {
 protected:
  glm::vec3 color{}; ///< @brief If you are not planning to use texture, you can set obj color.
  IndexBuffer *indexBuffer{}; ///< @brief Holds in which order should vertices be drawn.
  VertexBuffer *vertexBuffer{}; ///< @brief Holds data about vertices location.
  VertexArray *vertexArray{}; ///< @brief Holds all data about vertices (Location, Color/Texture, order).
  ColorBuffer *colorBuffer{}; ///< @brief Holds data about vertices colors.
  Texture *texture{}; /// @brief Holds data about texture.
  VertexBufferLayout *bufferLayout{}; ///<@brief Specifies amount of params per each vertex and their type.
  unsigned int layoutLength{3}; ///<@brief Specifies amount of params per each vertex.
  int timesToPushLayout = {1}; ///@brief corresponds with amount of layouts shader has.
  bool bInitialized{false};
  bool bOptimized{true}; ///< @Whether vertices are being reused or not.
 public:
  /**
   * @brief returns current contents of indexBuffer
   * @warning init() method should be called before calling this method
   * @throws runtime_error if object was not initialized
   **/
  [[nodiscard]] IndexBuffer *getIndexBuffer() const {
	if (bInitialized)
	  return indexBuffer;
	else throw std::runtime_error("Object is not initialized!");
  }
  /**
   * @brief returns current contents of vertexArray
   * @warning init() method should be called before calling this method
   * @throws runtime_error if object was not initialized
   **/
  [[nodiscard]] VertexArray *getVertexArray() const {
	if (bInitialized)
	  return vertexArray;
	else throw std::runtime_error("Object is not initialized!");
  }

  /**
   * @brief Allows you to set indexBuffer by referencing another indexBuffer.
   * @param _indexBuffer reference to indexBuffer
   **/
  [[maybe_unused]] void setIndexBuffer(IndexBuffer *_indexBuffer) {
	indexBuffer = _indexBuffer;
  }
  /**
   * @brief Allows you to set indexBuffer by passing a vector of unsigned ints.
   * @param _indices vector of unsigned ints.
   * @example obj.setIndexBuffer({0,1,2,2,3,0});
   **/
  [[maybe_unused]] void setIndexBuffer(std::vector<unsigned int> _indices) {
	indexBuffer = new IndexBuffer(std::move(_indices));
  }
  /**
   * @brief Allows you to set indexBuffer by passing a vector of Vertex. Will automatically generate indexBuffer assuming that you are
   * drawing triangles.
   * @param _vertices vector of Vertex.
   **/
  [[maybe_unused]] void setIndexBuffer(const std::vector<Vertex> &_vertices) {
	indexBuffer = new IndexBuffer(_vertices);
  }
  /**
  * @brief Allows you to set VertexBuffer by passing another VertexBuffer.
  * @param _vertexBuffer vector of Vertex.
  **/
  [[maybe_unused]] void setVertexBuffer(VertexBuffer _vertexBuffer) {
	vertexBuffer = &_vertexBuffer;
  }

  /**
   * @brief Allows you to set VertexBuffer by raw data and its size.
   * @param data reference to data
   * @param size of data
   * @example float vertices[]{0,1,0,0.1,0.2}; obj.setVertexBuffer(&vertices,sizeof(vertices));
   **/
  [[maybe_unused]] void setVertexBuffer(const void *data, unsigned int size) {
	vertexBuffer = new VertexBuffer(data, size);
  }

  /**
   * @brief Allows you to set VertexBuffer by passing a vector of Vertex.
   * @param vertices vector of vertices
   **/
  [[maybe_unused]] void setVertexBuffer(const std::vector<Vertex> &vertices) {
	colorBuffer = new ColorBuffer(vertices);
	vertexBuffer = new VertexBuffer(vertices);
  }
  /**
   * @brief Sets size of layoutLength (Amount of params per vertex).
   * @param length Amount of params per vertex
   */
  [[maybe_unused]] void setLayoutLength(unsigned int length) {
	layoutLength = length;
  }
  /**
   * @brief Sets size of layoutLength (Amount of layouts in shader).
   * @param length Amount of layouts in shader
   */
  [[maybe_unused]] void setTimesToPushLayout(int times) {
	timesToPushLayout = times;
  }

/**
 * @brief Sets colorBuffer with data from vector of Vertices
 * @param _verticesWithColors  vector of Vertices that have defined colors
 */
  [[maybe_unused]] void setColorBuffer(const std::vector<Vertex> &_verticesWithColors) {
	colorBuffer = new ColorBuffer(_verticesWithColors);
  }
  /**
   * @brief Sets color of whole object
   * @param _color vec3 of color you want the object to be
   */
  [[maybe_unused]] void setColor(glm::vec3 _color) {
	color = _color;
  }

  void setTexture(std::string _filepathToTexture) {
	texture = new Texture(std::move(_filepathToTexture));
	if (bOptimized) {
	  complicate();
	}
  }

  void complicate() {
	if (vertexBuffer == nullptr)throw std::runtime_error("VertexBuffer is null, set it before setting texture");
	if (indexBuffer == nullptr)throw std::runtime_error("IndexBuffer is null, set it before setting texture");
	std::vector<Vertex> tmp;
	for (int i = 0; i < indexBuffer->getLength(); i++) {
	  tmp.push_back(vertexBuffer->getVertices()[indexBuffer->getIndices()[i]]);
	}
	setVertexBuffer(tmp);
	bOptimized = false;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < tmp.size(); i++) {
	  indices.push_back(i);
	}
	setIndexBuffer(indices);
  }

  Object() {
	bInitialized = false;
  }
  /**
   * @brief initializes object with data that have been passed to it previously.
   */
  [[maybe_unused]] void init() {
	if (vertexBuffer == nullptr) {
	  throw std::runtime_error("Object init failed");
	}
	if (texture == nullptr) {
	  bufferLayout = new VertexBufferLayout();
	  for (int i = 0; i < timesToPushLayout; i++) {
		bufferLayout->push<float>(layoutLength);
	  }
	  vertexArray = new VertexArray();
	  vertexArray->addBuffer(*vertexBuffer, *bufferLayout);
	  if (colorBuffer != nullptr) {
		vertexArray->addBuffer(*colorBuffer, *bufferLayout, 1);
	  }
	} else {
	  auto *verticesLayout = new VertexBufferLayout();
	  auto *textureCoordsLayout = new VertexBufferLayout();
	  verticesLayout->push<float>(3);
	  textureCoordsLayout->push<float>(2);
	  vertexArray = new VertexArray();
	  vertexArray->addBuffer(*vertexBuffer, *verticesLayout);
	  auto texCoords = texture->generateTextureCoords(indexBuffer->getLength());
	  auto *textureVertexBuffer = new VertexBuffer(texCoords.data(), texCoords.size() * sizeof(float));
	  vertexArray->addBuffer(*textureVertexBuffer, *textureCoordsLayout, 1);
	}
	bInitialized = true;
  }
  void draw() {
	if (texture != nullptr) texture->bind();
  }

};

#endif //CGLABS__OBJECT_HPP_
