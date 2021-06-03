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
	bool CanMove() const { return m_canMove; }

private:
	float m_movementTimer = 0.0f;
	bool m_canMove = true;
};