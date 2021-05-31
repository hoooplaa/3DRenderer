#pragma once

#include <SFML/Graphics.hpp>

class InputHandler {
public:
	void ClearKeyboardInputs();
	void CheckKeyboardInputs();

	void ClearMouseInputs();
	void CheckMouseInputs();

	bool keyRightPressed = false;
	bool keyLeftPressed = false;
	bool keyUpPressed = false;
	bool keyDownPressed = false;
	bool keySpacePressed = false;

	bool keyLShiftPressed = false;

	bool mouseLeftPressed = false;
	bool mouseRightPressed = false;
};