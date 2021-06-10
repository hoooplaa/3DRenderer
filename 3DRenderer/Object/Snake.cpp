#include "Snake.h"

Snake::Snake() {

};

Snake::~Snake() {

};

void Snake::Initialize() {
	Entity::Initialize();

};

void Snake::Destroy() {

	Entity::Destroy();
};

void Snake::Update(const float in_dt) {
	Entity::Update(in_dt);

	m_movementTimer += in_dt;
};

void Snake::Draw(std::shared_ptr<Scene> in_scene) {
	Entity::Draw(in_scene);

};