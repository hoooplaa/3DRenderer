#include "Input.h"

//namespace InputHandler {
	void InputHandler::ClearKeyboardInputs() {

		keyWPressed = false;
		keySPressed = false;
		keyDPressed = false;
		keyAPressed = false;
		keySpacePressed = false;

		keyUpArrowPressed = false;
		keyDownArrowPressed = false;
		keyRightArrowPressed = false;
		keyLeftArrowPressed = false;

		keyLShiftPressed = false;
		keyRShiftPressed = false;
		keyCtrlPressed = false;
	}

	void InputHandler::CheckKeyboardInputs() {

		ClearKeyboardInputs();

		keyWPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
		keyAPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		keySPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
		keyDPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		keySpacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

		keyUpArrowPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
		keyDownArrowPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
		keyRightArrowPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
		keyLeftArrowPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);

		keyLShiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
		keyRShiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
		keyCtrlPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
	}

	void InputHandler::ClearMouseInputs() {

		mouseLeftPressed = false;
		mouseRightPressed = false;
	}

	void InputHandler::CheckMouseInputs() {
		ClearMouseInputs();

		mouseRightPressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);
		mouseLeftPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	}
//}