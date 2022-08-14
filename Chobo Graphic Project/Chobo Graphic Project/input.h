#pragma once
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

enum class Keyboard {
	Enter, Escape, Space, Left, Up, Right, Down,
	A, B, C, D, E, F, G, H, I, J,
	K, L, M, N, O, P, Q, R, S, T,
	U, V, W, X, Y, Z,
};

class Input
{
public:
	Input() = default;

	void update_prev_pos();
	glm::dvec2 get_mouse_pos();
	glm::dvec2 get_mouse_previous_pos();
	float get_mouse_scroll_value();

	bool IsLeftMouseDown() const;
	bool IsLeftMouseClicked() const;
	bool IsleftMouseReleased() const;

	bool IsRightMouseDown() const;
	bool IsRightMouseClicked() const;
	bool IsRightMouseReleased() const;

	bool IsKeyPressed(Keyboard key) const;
	bool IsKeyDown(Keyboard key) const;
	bool IsKeyReleased(Keyboard key) const;

private:
	glm::dvec2 mousePrevPos = {};
	int ModKey(Keyboard key) const;
};

