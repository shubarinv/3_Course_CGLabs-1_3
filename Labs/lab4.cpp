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
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Primitives/Cone.h>
#include <Magnum/Primitives/UVSphere.h>

#include "../functions.hpp"
#include "../lights.hpp"

class lab4 : public Platform::Application {
  typedef Magnum::Platform::GlfwApplication::KeyEvent::Key Key;

 public:
  explicit lab4(const Arguments& arguments) : Platform::Application{arguments, Configuration{}.setTitle("Lab 4")} {

	logInit(arguments.argc, arguments.argv);
	setWindowSize({640, 640});
	GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
	GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
	lightCoords = getCoordsForLight(0, 0, 4, 400);

	Trade::MeshData sphere = Primitives::uvSphereSolid(20, 20);
	Trade::MeshData pyramid = Primitives::coneSolid(20, 20, 1.f);

	GL::Buffer vertices_sphere;
	vertices_sphere.setData(MeshTools::interleave(sphere.positions3DAsArray(), sphere.normalsAsArray()));
	std::pair<Containers::Array<char>, MeshIndexType> compressed_sphere = MeshTools::compressIndices(sphere.indicesAsArray());
	GL::Buffer indices_sphere;
	indices_sphere.setData(compressed_sphere.first);

	GL::Buffer vertices_pyramid;
	vertices_pyramid.setData(MeshTools::interleave(pyramid.positions3DAsArray(), pyramid.normalsAsArray()));
	std::pair<Containers::Array<char>, MeshIndexType> compressed_pyramid = MeshTools::compressIndices(pyramid.indicesAsArray());
	GL::Buffer indices_pyramid;
	indices_pyramid.setData(compressed_pyramid.first);

	mesh_sphere.setPrimitive(sphere.primitive())
		.setCount(sphere.indexCount())
		.addVertexBuffer(std::move(vertices_sphere), 0, Shaders::Phong::Position{}, Shaders::Phong::Normal{})
		.setIndexBuffer(std::move(indices_sphere), 0, compressed_sphere.second);

	mesh_pyramid.setPrimitive(pyramid.primitive())
		.setCount(pyramid.indexCount())
		.addVertexBuffer(std::move(vertices_pyramid), 0, Shaders::Phong::Position{}, Shaders::Phong::Normal{})
		.setIndexBuffer(std::move(indices_pyramid), 0, compressed_pyramid.second);

	transformation =
		Matrix4::rotationX(Math::Deg(30.0f)) * Matrix4::rotationY(Math::Deg(40.0f));
	projection =
		Matrix4::perspectiveProjection(
			Math::Deg(35.0f), Vector2{windowSize()}.aspectRatio(), 0.01f, 100.0f)
		* Matrix4::translation(Vector3::zAxis(-10.0f));
	color = Color3::fromHsv({Math::Deg(35.0f), 1.0f, 1.0f});

	lights.addLight({{0, 0, 10}, {0.8, 0.8, 0.8}, "t1_1"});
	lights.addLight({{10, 0, 0}, {0.8, 0, 0.8}, "t3_1"});
	lights.addLight({{-10, 0, 10}, {0.8, 0.8, 0}, "t3_2"});
	shader = Shaders::Phong({}, lights.countEnabledLights());
  }

  /// Variables and functions
 private:
  void drawEvent() override {
	GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

	if (selectedTask == 0) {
	  lights.lookForTheLight("t1_1")->enable();
	  lights.lookForTheLight("t3_1")->disable();
	  lights.lookForTheLight("t3_2")->disable();
	  shader.draw(mesh_sphere);
	}
	if (selectedTask == 1) {
	  lights.lookForTheLight("t1_1")->enable();
	  lights.lookForTheLight("t1_1")->color = {0.8f, 0.8f, 0.8f};
	  lights.lookForTheLight("t3_1")->disable();
	  lights.lookForTheLight("t3_2")->disable();
	  shader.draw(mesh_sphere);
	}
	if (selectedTask == 2) {
	  lights.lookForTheLight("t1_1")->disable();
	  lights.lookForTheLight("t3_1")->enable();
	  lights.lookForTheLight("t3_2")->enable();
	  shader.draw(mesh_pyramid);
	}

	if (shader.lightCount() != lights.countEnabledLights()) {
	  shader = Shaders::Phong({}, lights.countEnabledLights());
	}

	shader.setLightPositions(lights.getPositions())
		.setLightColors(lights.getColors())
		.setDiffuseColor({1, 1, 1})
		.setAmbientColor({0.1, 0.1, 0.1})
		.setTransformationMatrix(transformation)
		.setNormalMatrix(transformation.normalMatrix())
		.setProjectionMatrix(projection);
	currentLightPosition++;
	if (currentLightPosition > lightCoords.size()) {
	  currentLightPosition = 0;
	}
	swapBuffers();
	redraw();
  }

  GL::Mesh mesh_sphere;
  GL::Mesh mesh_pyramid;
  Shaders::Phong shader;
  Matrix4 transformation, projection;
  Color3 color;
  Lights lights;
  unsigned int selectedTask{0};
  std::vector<Vector3> lightCoords;
  int currentLightPosition{0};
  static std::vector<Vector3> getCoordsForLight(double xc, double yc, double size, int n) {
	std::vector<Vector3> vertices;
	auto xe = xc + size;
	auto ye = yc;
	vertices.emplace_back(xe, yc, ye);
	double alpha = 0;
	for (int i = 0; i < n - 1; i++) {
	  alpha += 2 * M_PI / n;
	  auto xr = xc + size * cos(alpha);
	  auto yr = yc + size * sin(alpha);
	  xe = xr;
	  ye = yr;
	  vertices.emplace_back(xe, yc, ye);
	}
	return vertices;
  }

  void keyPressEvent(KeyEvent& event) override {
	LOG_S(INFO) << "(Keyboard)::PressedKey: " << event.keyName();
	if (event.key() == Key::Esc) {
	  quit();
	}
	if (event.key() == Key::Left) {
	  selectedTask--;
	  if (selectedTask == -1) { selectedTask = 0; }
	  LOG_S(INFO) << "Selected task: " << selectedTask;
	}
	if (event.key() == Key::Right) {
	  selectedTask++;
	  if (selectedTask > 3) {
		selectedTask = 0;
	  }
	  LOG_S(INFO) << "Selected task: " << selectedTask;
	}
	if (event.key() == Key::W) {
	  if (selectedTask == 1) {
		lights.lookForTheLight("t1_1")->position = {lights.lookForTheLight("t1_1")->position.x(), lights.lookForTheLight("t1_1")->position.y(), lights.lookForTheLight("t1_1")->position.z() - 0.5f};
	  }
	}
	if (event.key() == Key::S) {
	  if (selectedTask == 1) {
		lights.lookForTheLight("t1_1")->position = {lights.lookForTheLight("t1_1")->position.x(), lights.lookForTheLight("t1_1")->position.y(), lights.lookForTheLight("t1_1")->position.z() + 0.5f};
	  }
	}
	if (event.key() == Key::A) {
	  if (selectedTask == 0) {
		lights.lookForTheLight("t1_1")->color = {lights.lookForTheLight("t1_1")->color.rgb().r(), 0, lights.lookForTheLight("t1_1")->color.rgb().b() - 0.05f};
	  }
	  if (selectedTask == 1) {
		lights.lookForTheLight("t1_1")->position = {lights.lookForTheLight("t1_1")->position.x() - 0.5f, lights.lookForTheLight("t1_1")->position.y(), 10};
	  }
	}
	if (event.key() == Key::D) {
	  if (selectedTask == 0) {
		lights.lookForTheLight("t1_1")->color = {lights.lookForTheLight("t1_1")->color.rgb().r(), 0, lights.lookForTheLight("t1_1")->color.rgb().b() + 0.05f};
	  }
	  if (selectedTask == 1) {
		lights.lookForTheLight("t1_1")->position = {lights.lookForTheLight("t1_1")->position.x() + 0.5f, lights.lookForTheLight("t1_1")->position.y(), 10};
	  }
	}
	if (event.key() == Key::Y) {
	  if (selectedTask == 2) {
		lights.lookForTheLight("t3_1")->color =
			{lights.lookForTheLight("t3_1")->color.rgb().r() - 0.05f,
			 0,
			 lights.lookForTheLight("t3_1")->color.rgb().b() - 0.05f};
		lights.lookForTheLight("t3_2")->color =
			{lights.lookForTheLight("t3_2")->color.rgb().r() - 0.05f,
			 lights.lookForTheLight("t3_2")->color.rgb().g() - 0.05f,
			 0};
	  }
	}
	if (event.key() == Key::P) {
	  if (selectedTask == 2) {
		lights.lookForTheLight("t3_1")->color =
			{lights.lookForTheLight("t3_1")->color.rgb().r() + 0.05f,
			 0,
			 lights.lookForTheLight("t3_1")->color.rgb().b() + 0.05f};
		lights.lookForTheLight("t3_2")->color =
			{lights.lookForTheLight("t3_2")->color.rgb().r() + 0.05f,
			 lights.lookForTheLight("t3_2")->color.rgb().g() + 0.05f,
			 0};
	  }
	}
	if (selectedTask == 2) {
	  if (lights.lookForTheLight("t3_1")->color.rgb().r() >= 1) {
		lights.lookForTheLight("t3_1")->color = {0.1, 0, 0.1};
		lights.lookForTheLight("t3_2")->color = {0.1, 0.1, 0};
	  }
	  if (lights.lookForTheLight("t3_1")->color.rgb().r() <= 0) {
		lights.lookForTheLight("t3_1")->color = {0, 0, 0};
		lights.lookForTheLight("t3_2")->color = {0, 0, 0};
	  }
	}
  }
  void quit() {
	LOG_S(INFO) << "Goodbye!";
	this->exit();
  }
};

MAGNUM_APPLICATION_MAIN(lab4)