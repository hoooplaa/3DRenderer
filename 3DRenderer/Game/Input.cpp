#include "Input.h"

void InputHandler::ClearKeyboardInputs() {

	keyUpPressed = false;
	keyLeftPressed = false;
	keyDownPressed = false;
	keyRightPressed = false;
	keySpacePressed = false;

	keyLShiftPressed = false;
}

void InputHandler::CheckKeyboardInputs() {

	ClearKeyboardInputs();

	keyUpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	keyLeftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	keyDownPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	keyRightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	keySpacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

	keyLShiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
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