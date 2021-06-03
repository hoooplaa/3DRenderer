#include "Game.h"

// Make inputs a switch statment w hashing

// Add camera class

// Make matrix/vectors templates

// Actually understand functions in math.h

// dont forget about w (automatically initializing it to 1 and not when u initialize the cube, might be ap orblem)

// Figure out how to handle shapes with textures

// Add convention to math functions

// Add depth buffer

// Make input class static or singleton

// Change projection matrix on window resize

// Add tic setting

// Learn Rodrigues Rotation Formula

// Think of a better way to do the inputs

// Add normal functions and projection functions to math

// Clean up the headers

// Think of way to only write one create shape function that works for all shapes

// Actually use spawn w setup function

// Add snake create function

Game::Game() {

};

Game::~Game() {

};

void Game::Initialize() {
	Object::Initialize();

	m_scene = CreateScene(tVector2(1200, 800), "Scene");

	assert(m_robotoBoldFont.loadFromFile("C://Users//Alex Haurin//source//repos//3DRenderer//3DRenderer//Assets//Fonts//Roboto-Bold.ttf") && "Couldn't load roboto bold font");
	assert(m_robotoBoldFont.loadFromFile("C://Users//Alex Haurin//source//repos//3DRenderer//3DRenderer//Assets//Fonts//Roboto-Light.ttf") && "Couldn't load roboto light font");
	m_fpsText.setFont(m_robotoBoldFont);
	m_fpsText.setPosition(sf::Vector2f(10, m_scene->m_window->getSize().y - 50));

	// Populate scene
	std::shared_ptr<Cuboid> board = CreateGameCuboid(tVector3(0, 0, 0), 10, 1, 10);
	m_snake = std::make_shared<Snake>();
	m_snake->Initialize();

	m_shapes.push_back(board);
	m_entities.push_back(m_snake);

	//m_comanderCodyHelmet = m_renderer.LoadFromObj("Assets/Objects/CodyHelmet.obj");
	//m_comanderCodyHelmet.mesh = m_scene->m_renderer->LoadFromObj("C://Users//Alex Haurin//source//repos//3DRenderer//3DRenderer//Assets//Objects//Fox.obj", true);
}

void Game::Destroy() {
	Object::Destroy();
};

void Game::Run() {

	SetFrameRate(10000);
	while (m_scene->GetWindow()->isOpen()) {
		auto startTime = std::chrono::high_resolution_clock::now();
		HandleEvents();
		Update(dt);
		Draw();
		auto endTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = startTime - endTime;

		if (deltaTime < m_targetTime) {
			auto remainingTime = m_targetTime - deltaTime - std::chrono::milliseconds(5);
			if (remainingTime > std::chrono::milliseconds(0)) {
				std::this_thread::sleep_for(remainingTime);

				while ((std::chrono::high_resolution_clock::now() - startTime) < m_targetTime) {}
			}
		}
		numOfFrames += 1;
		totalTime += dt;
		dt = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - startTime).count();
		//std::cout << dt << std::endl;
	}
};

void Game::HandleEvents() {
	sf::Event event;

	while (m_scene->GetWindow()->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			m_scene->GetWindow()->close();
			Destroy();
			break;
		case sf::Event::Resized:
			break;
		case sf::Event::KeyPressed:
			//m_inputBool = true;
			m_inputState.CheckKeyboardInputs();
			break;
		case sf::Event::KeyReleased:
			m_inputState.CheckKeyboardInputs();
			break;
		case sf::Event::MouseButtonPressed:
			m_inputState.CheckMouseInputs();
			break;
		case sf::Event::MouseButtonReleased:
			m_inputState.CheckMouseInputs();
			break;
		}
	}
}

void Game::Update(const double in_dt) {
	m_scene->Update(in_dt);
	
	for (auto& shape : m_shapes) { shape->Update(in_dt); }
	for (auto& entity : m_entities) { entity->Update(in_dt); }

	m_mousePos = sf::Vector2f(sf::Mouse::getPosition(*m_scene->GetWindow()));
	auto mouseMovement = m_mousePos - m_pastMousePos;

	m_scene->RotateCameraYaw(mouseMovement.x / 1000);
	m_scene->RotateCameraPitch(mouseMovement.y / 1000);

	auto cameraDir = Normalize(m_scene->m_cameraDir);
	auto cameraPos = m_scene->m_cameraPos;

	// Camera movement
	if (m_inputState.keySpacePressed) { // up/down
		m_scene->MoveCamera(tVector3(0, 0.03 * in_dt, 0));
	}
	if (m_inputState.keyLShiftPressed) {
		m_scene->MoveCamera(tVector3(0, -0.03 * in_dt, 0));
	}
	if (m_inputState.keyWPressed) { // forward/back
		m_scene->MoveCamera(Vector_Mul(cameraDir, 0.03 * in_dt));
	}
	if (m_inputState.keySPressed) {
		m_scene->MoveCamera(Vector_Mul(cameraDir, -0.03 * in_dt));
	}
	if (m_inputState.keyAPressed) { // strafing
		m_scene->MoveCamera(Vector_Mul(tVector3(cameraDir.z, 0, -cameraDir.x), 0.03 * in_dt));
	}
	if (m_inputState.keyDPressed) {
		m_scene->MoveCamera(Vector_Mul(tVector3(cameraDir.z, 0, -cameraDir.x), -0.03 * in_dt));
	}
	// Snake movement
	if (m_inputState.keyUpArrowPressed) { m_inputState.lastKeyPressed = sf::Keyboard::Key::Up; }
	if (m_inputState.keyDownArrowPressed) { m_inputState.lastKeyPressed = sf::Keyboard::Key::Down; }
	if (m_inputState.keyRightArrowPressed) { m_inputState.lastKeyPressed = sf::Keyboard::Key::Right; }
	if (m_inputState.keyLeftArrowPressed) { m_inputState.lastKeyPressed = sf::Keyboard::Key::Left; }

	m_scene->m_renderer->UpdateScene(m_scene);

	m_pastMousePos = sf::Vector2f(sf::Mouse::getPosition(*m_scene->GetWindow()));
};

void Game::Draw() {
	m_scene->Clear(sf::Color::Black);

	for (auto& shape : m_shapes) { shape->Draw(m_scene); }
	for (auto& entity : m_entities) { entity->Draw(m_scene); }

	//m_scene->Render(m_comanderCodyHelmet);

	m_scene->Display();
};


std::shared_ptr<Scene> Game::CreateScene(const tVector2& in_dimensions, const std::string& in_name) {
	auto out_scene = Object::SpawnWithSetup<Scene>(shared_from_this(), {}, in_dimensions, in_name);

	return out_scene;
}

std::shared_ptr<Shape> Game::CreateGameShape() {
	auto out_shape = Object::SpawnWithSetup<Shape>(shared_from_this(), {});

	return out_shape;
}

std::shared_ptr<Cuboid> Game::CreateGameCuboid(const tVector3& in_position, float in_sideWidth, float in_sideHeight, float in_sideDepth) {
	auto out_cuboid = Object::SpawnWithSetup<Cuboid>(shared_from_this(), {}, in_position, in_sideWidth, in_sideHeight, in_sideDepth);

	return out_cuboid;
}