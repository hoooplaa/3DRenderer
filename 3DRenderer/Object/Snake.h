#pragma once

#include "Entity.h"

class Snake : public Entity {
public:
	Snake();
	~Snake();
	void Initialize();
	void Destroy();

	void Update(const float in_dt);
	void Draw(std::shared_ptr<Scene> in_scene);

	void ResetMovementTimer() { m_movementTimer = 0.0f; }
	bool CanMove() const { return m_movementTimer >= 1000.0f / m_movementSpeed; }

	tVector3 GetDirection() const { return m_direction; }
	void SetDirection(const tVector3& in_dir) { m_direction = in_dir; }
	void SetSpeed(float in_speed) { m_movementSpeed = in_speed; }

private:
	float m_movementTimer = 0.0f;
	float m_movementSpeed = 5;

	tVector3 m_direction = { 0, 0, 0 };
};