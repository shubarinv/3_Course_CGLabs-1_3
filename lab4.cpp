//
// Created by Vladimir Shubarin on 22.12.2020.
//
#include <Magnum/GL/DebugOutput.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/Platform/GlfwApplication.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/MeshData.h>

using namespace Magnum;

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/Platform/GlfwApplication.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/Trade/MeshData.h>

#include "functions.hpp"

class lab4 : public Platform::Application {
  typedef Magnum::Platform::GlfwApplication::KeyEvent::Key Key;

 public:
  explicit lab4(const Arguments& arguments) : Platform::Application{arguments, Configuration{}.setTitle("Lab 2")} {

	logInit(arguments.argc, arguments.argv);
	GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
	GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
	setWindowSize({640, 640});

	Trade::MeshData cube = Primitives::cubeSolid();

	GL::Buffer vertices;
	vertices.setData(MeshTools::interleave(cube.positions3DAsArray(), cube.normalsAsArray()));
	std::pair<Containers::Array<char>, MeshIndexType> compressed = MeshTools::compressIndices(cube.indicesAsArray());
	GL::Buffer indices;
	indices.setData(compressed.first);

	_mesh.setPrimitive(cube.primitive())
		.setCount(cube.indexCount())
		.addVertexBuffer(std::move(vertices), 0, Shaders::Phong::Position{}, Shaders::Phong::Normal{})
		.setIndexBuffer(std::move(indices), 0, compressed.second);
	_transformation =
		Matrix4::rotationX(Math::Deg(30.0f)) * Matrix4::rotationY(Math::Deg(40.0f));
	_projection =
		Matrix4::perspectiveProjection(
			Math::Deg(35.0f), Vector2{windowSize()}.aspectRatio(), 0.01f, 100.0f)
		* Matrix4::translation(Vector3::zAxis(-10.0f));
	_color = Color3::fromHsv({Math::Deg(35.0f), 1.0f, 1.0f});
  }

  /// Variables and functions
 private:
  void drawEvent() override {
	GL::defaultFramebuffer.clear(
		GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

	_shader.setLightPositions({{7.0f, 5.0f, 2.5f}})
		.setDiffuseColor(_color)
		.setAmbientColor(Color3::fromHsv({_color.hue(), 1.0f, 0.3f}))
		.setTransformationMatrix(_transformation)
		.setNormalMatrix(_transformation.normalMatrix())
		.setProjectionMatrix(_projection)
		.draw(_mesh);

	swapBuffers();
  }

  GL::Mesh _mesh;
  Shaders::Phong _shader;

  Matrix4 _transformation, _projection;
  Color3 _color;
  std::vector<std::vector<unsigned int>> indices;
};

MAGNUM_APPLICATION_MAIN(lab4)