#include "OpenGL.h"

Camera::Camera(GLFWwindow* window_ptr, int windowWidth, int windowHeight) : window_ptr(window_ptr)
{
	Reset();
	camMove = false;
	nearPlane = 1.0f;
	farPlane = 1000.0f;
	topPlane = 0.6f * nearPlane;
	bottomPlane = -topPlane;
	aspect = 1.0f * windowWidth / windowHeight;
	rightPlane = topPlane * aspect;
	leftPlane = -rightPlane;
}

void Camera::ComputeViewProjMats()
{

	/*  Update view transform matrix */
	viewMat = LookAt(cam, cam + camFront, camUp);

	/*  Update projection matrix */
	projMat = Frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);

	/*  Update view/projection-related matrices for non-animating objects */
	magMat = Scale(Vec3(10));
	vpMat = projMat * viewMat * magMat;
}

void Camera::Reset()
{
	cam = { 0,0,20 };
	camFront = {0,0,-1};
	camUp = { 0, 1, 0 };
	yaw = 270;
	pitch = 0;
}

void Camera::Update()
{
	if (OpenGLInput.IsKeyReleased(Keyboard::Space)) {
		camMove = !camMove;
	}
	if (camMove) {
		glfwSetInputMode(window_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	} 
	else {
		glfwSetInputMode(window_ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if(!sScene.lightMode)
	{
		if (OpenGLInput.IsKeyDown(Keyboard::W)) {
			cam += camspeed * camFront;
		}
		if (OpenGLInput.IsKeyDown(Keyboard::S)) {
			cam -= camspeed * camFront;
		}
		if (OpenGLInput.IsKeyDown(Keyboard::A)) {
			cam -= glm::normalize(glm::cross(camFront, camUp)) * camspeed;
		}
		if (OpenGLInput.IsKeyDown(Keyboard::D)) {
			cam += glm::normalize(glm::cross(camFront, camUp)) * camspeed;
		}
		if (OpenGLInput.IsKeyDown(Keyboard::Q)) {
			cam.y += camspeed;
		}
		if (OpenGLInput.IsKeyDown(Keyboard::E)) {
			cam.y -= camspeed;
		}
	}

	mouseCam(OpenGLInput.get_mouse_pos().x, OpenGLInput.get_mouse_pos().y);
}

void Camera::SetCamSpeed(float speed) {
	camspeed = speed;
}

void Camera::SetPosition(Vec3 pos) {
	cam = pos;
}


void Camera::mouseCam(double xpos, double ypos)
{
	if (camMove)
	{
		if(!sScene.lightMode)
		{
			float xoffset = OpenGLInput.get_mouse_pos().x - OpenGLInput.get_mouse_previous_pos().x;
			float yoffset = OpenGLInput.get_mouse_pos().y - OpenGLInput.get_mouse_previous_pos().y;

			lastX = xpos;
			lastY = ypos;

			float sensitivity = 0.05;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw += xoffset;
			pitch += yoffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			Vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			camFront = glm::normalize(front);
		}
	}
}

