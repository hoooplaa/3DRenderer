#include "Scene.h"

Scene::Scene(const tVector2& in_dimensions, const std::string& in_name)
	: m_name(in_name), m_dimensions(in_dimensions) {

};

Scene::~Scene() {

};

void Scene::Initialize() {
	m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(m_dimensions.x, m_dimensions.y), m_name);

	m_renderer->Initialize(shared_from_base<Scene>());

	m_cameraYaw = 0.0f;
	m_cameraPitch = 0.0f;

	tVector3 target = { 0,0,1 };
	tMatrix4x4 mCameraRotationY = Math::MakeRotationMatrixY(m_cameraYaw);
	tMatrix4x4 mCameraRotationX = Math::MakeRotationMatrixX(m_cameraPitch);

	m_cameraDir = MultiplyMatrixVector(mCameraRotationX, target);
	m_cameraDir = MultiplyMatrixVector(mCameraRotationY, m_cameraDir);
};

void Scene::Destroy() {
	m_window->close();
	
	Object::Destroy();
};

void Scene::Update(double in_dt) {
	// Update camera dir
	tVector3 target = { 0,0,1 };
	tMatrix4x4 mCameraRotationY = Math::MakeRotationMatrixY(m_cameraYaw);
	tMatrix4x4 mCameraRotationX = Math::MakeRotationMatrixX(m_cameraPitch);

	m_cameraDir = MultiplyMatrixVector(mCameraRotationX, target);
	m_cameraDir = MultiplyMatrixVector(mCameraRotationY, m_cameraDir);
};

void Scene::Draw() {
	
}

void Scene::Clear(const sf::Color& in_color) {
	m_window->clear(in_color);
};

void Scene::Display() {
	m_window->display();
}

void Scene::Render(Shape& in_shape) {
	m_renderer->RenderShape(in_shape, shared_from_base<Scene>());
}

void Scene::RotateCameraYaw(float in_yaw) {
	// Cap yaw / pitch to avoid float overflows and loosing precision
	m_cameraYaw += in_yaw;

	m_cameraYaw = std::fmod(m_cameraYaw, MATH_PI * 2);
}

void Scene::RotateCameraPitch(float in_pitch) {
	// Cap yaw/pitch to avoid float overflows and loosing precision
	m_cameraPitch += in_pitch;

	m_cameraPitch = std::fmod(m_cameraPitch, MATH_PI * 2);
}

void Scene::MoveCamera(const tVector3& in_movement) {
	m_cameraPos = Vector_Add(m_cameraPos, in_movement);
}