#include "Entity.h"

Entity::Entity() {
}

Entity::~Entity() {}

void Entity::Initialize() {

	Object::Initialize();
}

void Entity::Destroy() {
	Object::Destroy();

}

void Entity::Update(const float in_dt) {

}

void Entity::Draw(std::shared_ptr<sf::RenderWindow> in_window) {

}

void Entity::Move(const tVector3& in_vec) {

}