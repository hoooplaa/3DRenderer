#include "Entity.h"

Entity::Entity() {
}

Entity::~Entity() {}

void Entity::Initialize() {
	Object::Initialize();

	m_shape = Object::SpawnWithSetup<Shape>(shared_from_this(), {});
}

void Entity::Destroy() {

	Object::Destroy();
}

void Entity::Update(const float in_dt) {
	m_shape->Update(in_dt);
}

void Entity::Draw(std::shared_ptr<Scene> in_scene) {
	m_shape->Draw(in_scene);
}

void Entity::Move(const tVector3& in_vec) {
	m_shape->Move(in_vec);
}