#pragma once

#include <iostream>
#include <memory>

#include "../Renderer/Scene.h"
#include "Shapes.h"
#include "../Math/Math.h"

class Game;

class Entity : public Object {
public:
	using tVector3 = Math::Vector3;
	using tString = std::string;
	using tSprite = sf::Sprite;
	using tTexture = sf::Texture;

	Entity();
	~Entity();
	void Initialize();
	void Destroy();

	void Update(const float in_dt);
	void Draw(std::shared_ptr<Scene> in_scene);

	virtual void Move(const tVector3& in_vec);

	tVector3 GetPosition() const { return m_shape->GetPosition(); }
	void SetPosition(const tVector3& in_pos) { m_shape->SetPosition(in_pos); }
	void SetGame(std::shared_ptr<Game> in_game) { m_game = in_game; }

protected:
	std::shared_ptr<Shape> m_shape;
	tString m_name;

	std::shared_ptr<Game> m_game;
};