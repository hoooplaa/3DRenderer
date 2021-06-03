#pragma once

#include <SFML/Graphics.hpp>

class InputHandler {
public:
	void ClearKeyboardInputs();
	void CheckKeyboardInputs();

	void ClearMouseInputs();
	void CheckMouseInputs();

	bool keyWPressed = false;
	bool keySPressed = false;
	bool keyDPressed = false;
	bool keyAPressed = false;
	bool keySpacePressed = false;

	bool keyUpArrowPressed = false;
	bool keyDownArrowPressed = false;
	bool keyRightArrowPressed = false;
	bool keyLeftArrowPressed = false;

	bool keyLShiftPressed = false;

	bool mouseLeftPressed = false;
	bool mouseRightPressed = false;

	sf::Keyboard::Key lastKeyPressed;
};