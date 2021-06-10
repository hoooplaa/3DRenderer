#include "Game.h"

/////////////////////////// Shit i gotta do /////////////////////////
// Make inputs a switch statment w hashing?

// Add camera class

// Make matrix/vectors templates

// Add depth buffer?

// Make input class static or singleton

// Change projection matrix on window resize

// Add tic setting

// Learn Rodrigues Rotation Formula

// Use/learn quarternions for camera movement/rotation

// Clean up the headers

Game::Game() {

};

Game::~Game() {

};

void Game::Initialize() {
	Object::Initialize();

	m_scene = CreateScene(tVector2(1200, 800), "Scene");

	assert(m_robotoBoldFont.loadFromFile("C://Users//Alex Haurin//source//repos//3DRenderer//3DRenderer//Assets//Fonts//Roboto-Bold.ttf") && "Couldn't load roboto bold font");

	// Populate scene
	std::shared_ptr<Cuboid> board = CreateGameCuboid(tVector3(0, 0, 0), tVector3(m_boardSize.x * m_blockSize, m_blockSize, m_boardSize.y * m_blockSize));
	m_snake = CreateSnake();

	m_shapes.push_back(board);
	m_entities.push_back(m_snake);

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

void Game::Update(const float in_dt) {
	m_scene->Update(in_dt);

	//std::cout << m_scene->m_cameraPos << std::endl;
	
	// Update entities
	for (auto& shape : m_shapes) { shape->Update(in_dt); }
	for (auto& entity : m_entities) { entity->Update(in_dt); }

	// Mouse movement
	m_mousePos = sf::Vector2f(sf::Mouse::getPosition(*m_scene->GetWindow())); // Calculate the change in mouse pos from last fram to use for camera pointing
	auto mouseMovement = m_mousePos - m_pastMousePos;
	m_pastMousePos = m_mousePos;

	// Camera movement
	m_scene->RotateCameraYaw(in_dt * (mouseMovement.x / 10000)); // Rotate camera using change in mouse movement
	m_scene->RotateCameraPitch(in_dt * (mouseMovement.y / 10000));

	auto cameraDir = Normalize(m_scene->m_cameraDir); // Calculate camera dir/pos to use in movement input
	auto cameraPos = m_scene->m_cameraPos;

	// Camera movement
	if (m_inputState.keySpacePressed) { m_scene->MoveCamera(tVector3(0, 0.03 * in_dt, 0)); } // Up/down
	else if (m_inputState.keyLShiftPressed) { m_scene->MoveCamera(tVector3(0, -0.03 * in_dt, 0)); }
	else if (m_inputState.keyWPressed) { m_scene->MoveCamera(VectorMul(cameraDir, 0.03 * in_dt)); } // Forawrd/back
	else if (m_inputState.keySPressed) { m_scene->MoveCamera(VectorMul(cameraDir, -0.03 * in_dt)); }
	else if (m_inputState.keyAPressed) { m_scene->MoveCamera(VectorMul(tVector3(cameraDir.z, 0, -cameraDir.x), 0.03 * in_dt)); } // Strafing
	else if (m_inputState.keyDPressed) { m_scene->MoveCamera(VectorMul(tVector3(cameraDir.z, 0, -cameraDir.x), -0.03 * in_dt)); }
	// Snake movement
	if (m_inputState.keyUpArrowPressed) { m_snake->SetDirection({ 0, 0, 1 }); }
	else if (m_inputState.keyDownArrowPressed) { m_snake->SetDirection({ 0, 0, -1 }); }
	else if (m_inputState.keyRightArrowPressed) { m_snake->SetDirection({ -1, 0, 0 }); }
	else if (m_inputState.keyLeftArrowPressed) { m_snake->SetDirection({ 1, 0, 0 }); }
	else if (m_inputState.keyCtrlPressed) { m_snake->SetDirection({ 0, 1, 0 }); }
	else if (m_inputState.keyRShiftPressed) { m_snake->SetDirection({ 0, -1, 0 }); }

	if (m_snake->CanMove()) { // Move snake in current dir if snake has waited long enough
		m_snake->Move(Math::VectorMul(m_snake->GetDirection(), m_blockSize));
		m_snake->ResetMovementTimer();
	}
};

void Game::Draw() {
	m_scene->Clear(sf::Color::Black);

	// Draw entities
	for (auto& shape : m_shapes) { shape->Draw(m_scene); }
	for (auto& entity : m_entities) { entity->Draw(m_scene); }

	//m_scene->Render(m_comanderCodyHelmet);

	m_scene->Display();
};


std::shared_ptr<Scene> Game::CreateScene(const tVector2& in_dimensions, const std::string& in_name) {
	auto out_scene = Object::SpawnWithSetup<Scene>(shared_from_this(), {}, in_dimensions, in_name);

	return out_scene;
}

std::shared_ptr<Snake> Game::CreateSnake() {
	auto out_snake = Object::SpawnWithSetup<Snake>(shared_from_this(), [this](Snake* in_snake) {
		in_snake->SetGame(shared_from_base<Game>());
	});

	// Quick fix to bug i'm working on
	tVector3 pos = out_snake->GetShape()->GetPosition();
	tVector3 dim = out_snake->GetShape()->GetDimensions();

	out_snake->GetShape()->mesh.tris = {
		//SOUTH																						 
		{ pos,								        { pos.x, pos.y + dim.y, pos.z },                  { pos.x + dim.x, pos.y + dim.y, pos.z } },
		{ { pos.x, pos.y, pos.z },                  { pos.x + dim.x, pos.y + dim.y, pos.z },          { pos.x + dim.x, pos.y, pos.z } },

		//EAST								    	 									  			 
		{ { pos.x + dim.x, pos.y, pos.z },          { pos.x + dim.x, pos.y + dim.y, pos.z },          { pos.x + dim.x, pos.y + dim.y, pos.z + dim.z } },
		{ { pos.x + dim.x, pos.y, pos.z },          { pos.x + dim.x, pos.y + dim.y, pos.z + dim.z },  { pos.x + dim.x, pos.y, pos.z + dim.z } },

		//North										 									  			 
		{ { pos.x + dim.x, pos.y, pos.z + dim.z },  { pos.x + dim.x, pos.y + dim.y, pos.z + dim.z },  { pos.x, pos.y + dim.y, pos.z + dim.z } },
		{ { pos.x + dim.x, pos.y, pos.z + dim.z },  { pos.x, pos.y + dim.y, pos.z + dim.z },          { pos.x, pos.y, pos.z + dim.z } },

		//WEST										 									  			 
		{ { pos.x, pos.y, pos.z + dim.z },		    { pos.x, pos.y + dim.y, pos.z + dim.z },          { pos.x, pos.y + dim.y, pos.z } },
		{ { pos.x, pos.y, pos.z + dim.z },		    { pos.x, pos.y + dim.y, pos.z },                  { pos.x, pos.y, pos.z } },

		//TOP										 									  			 
		{ { pos.x, pos.y + dim.y, pos.z },          { pos.x, pos.y + dim.y, pos.z + dim.z },          { pos.x + dim.x, pos.y + dim.y, pos.z + dim.z } },
		{ { pos.x, pos.y + dim.y, pos.z },          { pos.x + dim.x, pos.y + dim.y, pos.z + dim.z },  { pos.x + dim.x, pos.y + dim.y, pos.z } },

		//BOTTOM									 									  			 
		{ { pos.x + dim.x, pos.y, pos.z + dim.z },  { pos.x, pos.y, pos.z + dim.z },                  { pos.x, pos.y, pos.z } },
		{ { pos.x + dim.x, pos.y, pos.z + dim.z },  { pos.x, pos.y, pos.z },                          { pos.x + dim.x, pos.y, pos.z } },
	};

	return out_snake;
}

std::shared_ptr<Cuboid> Game::CreateGameCuboid(const tVector3& in_position, const tVector3& in_dimensions) {
	auto out_cuboid = Object::SpawnWithSetup<Cuboid>(shared_from_this(), {}, in_position, in_dimensions);

	// Quick fix to bug i'm working on
	tVector3 pos = out_cuboid->GetPosition();
	tVector3 dim = out_cuboid->GetDimensions();

	out_cuboid->mesh.tris = {
		//SOUTH																						 
		{ pos,								        { pos.x, pos.y + dim.y, pos.z },                  { pos.x + dim.x, pos.y + dim.y, pos.z } },
		{ { pos.x, pos.y, pos.z },                  { pos.x + dim.x, pos.y + dim.y, pos.z },          { pos.x + dim.x, pos.y, pos.z } },

		//EAST								    	 									  			 
		{ { pos.x + dim.x, pos.y, pos.z },          { pos.x + dim.x, pos.y + dim.y, pos.z },          { pos.x + dim.x, pos.y + dim.y, pos.z + dim.z } },
		{ { pos.x + dim.x, pos.y, pos.z },          { pos.x + dim.x, pos.y + dim.y, pos.z + dim.z },  { pos.x + dim.x, pos.y, pos.z + dim.z } },

		//North										 									  			 
		{ { pos.x + dim.x, pos.y, pos.z + dim.z },  { pos.x + dim.x, pos.y + dim.y, pos.z + dim.z },  { pos.x, pos.y + dim.y, pos.z + dim.z } },
		{ { pos.x + dim.x, pos.y, pos.z + dim.z },  { pos.x, pos.y + dim.y, pos.z + dim.z },          { pos.x, pos.y, pos.z + dim.z } },

		//WEST										 									  			 
		{ { pos.x, pos.y, pos.z + dim.z },		    { pos.x, pos.y + dim.y, pos.z + dim.z },          { pos.x, pos.y + dim.y, pos.z } },
		{ { pos.x, pos.y, pos.z + dim.z },		    { pos.x, pos.y + dim.y, pos.z },                  { pos.x, pos.y, pos.z } },

		//TOP										 									  			 
		{ { pos.x, pos.y + dim.y, pos.z },          { pos.x, pos.y + dim.y, pos.z + dim.z },          { pos.x + dim.x, pos.y + dim.y, pos.z + dim.z } },
		{ { pos.x, pos.y + dim.y, pos.z },          { pos.x + dim.x, pos.y + dim.y, pos.z + dim.z },  { pos.x + dim.x, pos.y + dim.y, pos.z } },

		//BOTTOM									 									  			 
		{ { pos.x + dim.x, pos.y, pos.z + dim.z },  { pos.x, pos.y, pos.z + dim.z },                  { pos.x, pos.y, pos.z } },
		{ { pos.x + dim.x, pos.y, pos.z + dim.z },  { pos.x, pos.y, pos.z },                          { pos.x + dim.x, pos.y, pos.z } },
	};

	return out_cuboid;
}