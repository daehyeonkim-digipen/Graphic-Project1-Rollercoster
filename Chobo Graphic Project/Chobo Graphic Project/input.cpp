#include <GL/glew.h>
#include "OpenGL.h"

void Input::update_prev_pos()
{
	mousePrevPos = get_mouse_pos();
}

glm::dvec2 Input::get_mouse_pos()
{
	return Vec2(ImGui::GetIO().MousePos.x, OpenGL::window_size().y - ImGui::GetIO().MousePos.y);
}

glm::dvec2 Input::get_mouse_previous_pos()
{
	return mousePrevPos;
}

float Input::get_mouse_scroll_value()
{
	return ImGui::GetIO().MouseWheel;
}

bool Input::IsLeftMouseDown() const {
	return ImGui::IsMouseDown(0);
}

bool Input::IsLeftMouseClicked() const {
	return ImGui::IsMouseClicked(0);
}

bool Input::IsleftMouseReleased() const {
	return ImGui::IsMouseReleased(0);
}

bool Input::IsRightMouseDown() const {
	return ImGui::IsMouseDown(1);
}

bool Input::IsRightMouseClicked() const {
	return ImGui::IsMouseClicked(1);
}

bool Input::IsRightMouseReleased() const {
	return ImGui::IsMouseReleased(1);
}

int Input::ModKey(Keyboard key) const {
	if (key >= Keyboard::A && key <= Keyboard::Z) {
		int offset = static_cast<int>(key) - static_cast<int>(Keyboard::A);
		return 'A' + offset;
	}
	switch (key) {
	case Keyboard::Enter:
		return ImGui::GetKeyIndex(ImGuiKey_Enter);
		break;
	case Keyboard::Escape:
		return ImGui::GetKeyIndex(ImGuiKey_Escape);
		break;
	case Keyboard::Space:
		return ImGui::GetKeyIndex(ImGuiKey_Space);
		break;
	case Keyboard::Left:
		return ImGui::GetKeyIndex(ImGuiKey_LeftArrow);
		break;
	case Keyboard::Up:
		return ImGui::GetKeyIndex(ImGuiKey_UpArrow);
		break;
	case Keyboard::Right:
		return ImGui::GetKeyIndex(ImGuiKey_RightArrow);
		break;
	case Keyboard::Down:
		return ImGui::GetKeyIndex(ImGuiKey_DownArrow);
		break;
	}
}

bool Input::IsKeyPressed(Keyboard key) const {
	return ImGui::IsKeyPressed(ModKey(key));
}

bool Input::IsKeyDown(Keyboard key) const {
	return ImGui::IsKeyDown(ModKey(key));
}

bool Input::IsKeyReleased(Keyboard key) const {
	return ImGui::IsKeyReleased(ModKey(key));
}