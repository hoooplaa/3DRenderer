#pragma once

#include "Object.h"
#include <iostream>

#include <SFML/Graphics.hpp>

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
	void Draw(std::shared_ptr<sf::RenderWindow> window);

	virtual void Move(const tVector3& in_vec);

	void SetGame(std::shared_ptr<Game> in_game) { m_game = in_game; }

protected:
	tString m_name;

	std::shared_ptr<Game> m_game;
};