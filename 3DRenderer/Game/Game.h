#pragma once

#include <time.h>
#include <chrono>
#include <thread>
#include <cassert>
#include <thread>

#include <SFML/Graphics.hpp>

#include "../Game/Input.h"
#include "../Renderer/Scene.h"
#include "../Object/Snake.h"

class Game : public Object {
public:
	using tVector3 = Math::Vector3;
	using tVector2 = sf::Vector2f;
	using tMatrix4x4 = Math::Matrix4x4;

	Game();
	~Game();

	void Initialize();
	void Destroy();

	void Run();
	void HandleEvents();
	void Update(const double dt);
	void Draw();

	std::shared_ptr<Scene> CreateScene(const tVector2& in_dimensions, const std::string& in_name);
	std::shared_ptr<Shape> CreateGameShape();
	std::shared_ptr<Cuboid> CreateGameCuboid(const tVector3& in_position, float in_sideWidth, float in_sideHeight, float in_sideDepth);

	double GetFrameRate() const { return m_frameRate; }
	void   SetFrameRate(double in_frameRate) {
		m_frameRate = in_frameRate;
		m_targetTime = std::chrono::milliseconds((int)(1000.0f / in_frameRate));
	}

private:
	double dt = 1.0;
	std::chrono::milliseconds m_targetTime = std::chrono::milliseconds(0);
	double m_frameRate = 60.0;
	float m_theta = 0.0f;

	std::shared_ptr<Scene> m_scene;
	InputHandler m_inputState;
	
	std::vector<std::shared_ptr<Shape>> m_shapes;
	std::vector<std::shared_ptr<Entity>> m_entities;

	std::shared_ptr<Snake> m_snake;
	std::shared_ptr<Shape> m_comanderCodyHelmet;

	sf::Text m_fpsText;
	sf::Font m_robotoBoldFont;
	sf::Font m_robotoLightFont;

	sf::Vector2f m_mousePos;
	sf::Vector2f m_pastMousePos;

	//Benchmarking
public:
	int numOfFrames = 0;
	float totalTime = 0.0f;
};