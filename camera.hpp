//
// Created by Vladimir Shubarin on 24.12.2020.
//

#ifndef CGLABS__CAMERA_HPP_
#define CGLABS__CAMERA_HPP_
#include "functions.hpp"

class Camera {
  Vector3 position{};
  Vector3 lookingAt{};
  Matrix4 projection{};
  Matrix4 transformation{};
  Matrix4 model{};
  Matrix4 view{};

 public:
  Camera() {
	projection = Magnum::Matrix4::perspectiveProjection(
		Magnum::Deg(90.f),// Вертикальное поле зрения в радианах. Обычно между 90&deg; (очень широкое) и 30&deg; (узкое)
		4.0f / 3.0f,      // Отношение сторон. Зависит от размеров вашего окна. Заметьте, что 4/3 == 800/600 == 1280/960
		0.1f,             // Ближняя плоскость отсечения. Должна быть больше 0.
		100.0f            // Дальняя плоскость отсечения.
	);
	model = Matrix4(1.0f);
  }

  explicit Camera(Vector2i windowSize) {
	projection = Magnum::Matrix4::perspectiveProjection(
		Magnum::Deg(90.f),              // Вертикальное поле зрения в радианах. Обычно между 90&deg; (очень широкое) и 30&deg; (узкое)
		windowSize.x() / windowSize.y(),// Отношение сторон. Зависит от размеров вашего окна.
		0.1f,                           // Ближняя плоскость отсечения. Должна быть больше 0.
		100.0f                          // Дальняя плоскость отсечения.
	);
	model = Matrix4(1.0f);
  }
  Camera* lookAt(Vector3 lookAt) {
	lookingAt = lookAt;
	updateView();
	return this;
  }
  Camera* moveTo(Vector3 moveTo) {
	position = moveTo;
	updateView();
	return this;
  }
  Camera* setModel(Matrix4 _model) {
	model = _model;
	return this;
  }
  Matrix4 getModel() {
	return model;
  }
  Matrix4 getProjection() {
	return projection;
  }
  Matrix4 getView() {
	view = Magnum::Matrix4::lookAt(
		position,
		lookingAt,
		Vector3(0, 1, 0));
	return view;
  }
  Matrix4 getMVP() {
	return projection * view * model;
  }
  Camera* updateView() {
	view = Magnum::Matrix4::lookAt(
		position,
		lookingAt,
		Vector3(0, 1, 0));
	return this;
  }
  Camera* transform(Math::Deg<float> x, Math::Deg<float> y, Math::Deg<float> z) {
	transformation = Matrix4::rotationX(Math::Deg(x)) * Matrix4::rotationY(Math::Deg(y)) * Matrix4::rotationZ(Math::Deg(z));
	return this;
  }
  Matrix4 getTransformation() {
	return transformation;
  }
  void setTransformation(Matrix4 _transformation) {
	transformation = _transformation;
  }
};

#endif//CGLABS__CAMERA_HPP_
