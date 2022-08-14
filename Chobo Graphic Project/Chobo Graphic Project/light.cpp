#include "OpenGL.h"

Light::Light(GLFWwindow* window_ptr) : window_ptr(window_ptr)
{
	Reset();
	nearPlane = 2.0f;
	farPlane = 500.0f;
	fov = QUARTER_PI;
	topPlane = tan(fov / 2) * nearPlane;
	bottomPlane = -topPlane;
	aspect = 1.0f * 2048.f / 2048.f;
	rightPlane = topPlane * aspect;
	leftPlane = -rightPlane;
	lightx = 1.f;
	lighty = 1.f;
	lightz = 1.f;
	lightFront = { 0, 0, 0 };
	ShadInit();
}

void Light::ComputeViewProjMats()
{	/*  Update view transform matrix */
	Vec3 lightPos{ lightx, lighty, lightz };

	viewMat = LookAt(lightPos, lightPos + lightFront, lightUp);

	/*  Update projection matrix */
	projMat = Frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);

	/*  Update view/projection-related matrices for non-animating objects */

	magMat = Scale(Vec3(10));
	vpMat = projMat * viewMat * magMat;

	Mat4 invProj = inverse(vpMat);
	points = { invProj * Vec4(-1.f, -1.f, -0.f, 1.f), invProj * Vec4(-1.f, -1.f, 1.f, 1.f),
				invProj * Vec4(-1.f, +1.f, -0.f, 1.f), invProj * Vec4(-1.f, +1.f, 1.f, 1.f),
				invProj * Vec4(+1.f, +1.f, -0.f, 1.f), invProj * Vec4(+1.f, +1.f, 1.f, 1.f),
				invProj * Vec4(+1.f, -1.f, -0.f, 1.f), invProj * Vec4(+1.f, -1.f, 1.f, 1.f), 
				invProj * Vec4(-1.f, -1.f, -0.f, 1.f), invProj * Vec4(-1.f, -1.f, 1.f, 1.f) };
	for (int i = 0; i < points.size(); ++i) {
		points[i].x /= points[i].w;
		points[i].y /= points[i].w;
		points[i].z /= points[i].w;
		points[i].w = 1.f;
	}
}

void Light::DrawFrustum() {
	for (int i = 0; i < points.size() - 2; i += 2) {
		float vertices[12];
		vertices[0] = points[i + 2].x;
		vertices[1] = points[i + 2].y;
		vertices[2] = points[i + 2].z;
		vertices[3] = points[i].x;
		vertices[4] = points[i].y;
		vertices[5] = points[i].z;
		vertices[6] = points[i + 1].x;
		vertices[7] = points[i + 1].y;
		vertices[8] = points[i + 1].z;
		vertices[9] = points[i + 3].x;
		vertices[10] = points[i + 3].y;
		vertices[11] = points[i + 3].z;
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(shadpgm);
		GLint uniform_mvpmat = glGetUniformLocation(shadpgm, "mvpMat");
		if (uniform_mvpmat >= 0) {
			glUniformMatrix4fv(uniform_mvpmat, 1, GL_FALSE, ValuePtr(sScene.lightMode ? OpenGLLight.vpMat : OpenGLCamera.vpMat));
		}
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, 4);
		glUseProgram(0);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
}

void Light::ComputeLightMats()
{
	Vec3 lightvec;
	Mat4 rot;
	if (shadow == false) {
		lightvec = glm::normalize(Vec3{ lightx, lighty, lightz });
		float y = acos(lightvec.y);
		rot = Rotate(-y, Vec3(-lightz, 0.f, lightx));
		if (lightvec.x == 0.f && lightvec.z == 0.f) {
			if (lightvec.y >= 0.f) {
				rot = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
			}
			else {
				rot = Rotate(PI, Vec3(1.f, 0.f, 0.f));
			}
		}
		lightvec *= lightvecmult;
		litMat = OpenGLCamera.projMat * OpenGLCamera.viewMat * Translate(lightvec) * rot * Scale(Vec3(2));
	}
	else {
		lightvec = glm::normalize(Vec3(lightFront.x, lightFront.y, lightFront.z));
		float y = acos(-lightvec.y);
		rot = Rotate(-HALF_PI ,Vec3(0, 1, 0)) * Rotate(y, Vec3(lightvec.x, 0.f, lightvec.z));
		if (lightvec.x == 0.f && lightvec.z == 0.f) {
			if (lightvec.y >= 0.f) {
				rot = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
			}
			else {
				rot = Rotate(PI, Vec3(1.f, 0.f, 0.f));
			}
		}
		litMat = OpenGLCamera.projMat * OpenGLCamera.viewMat * Translate(Vec3{ lightx, lighty, lightz }) * rot * Scale(Vec3(2));
		if (sScene.lightMode == true) {
			litMat = OpenGLLight.projMat * OpenGLLight.viewMat * Translate(Vec3{ lightx, lighty, lightz } - lightFront) * rot * Scale(Vec3(2));
		}
	}
	ltnmMat = Scale(Vec3(1));
}


void Light::Reset()
{
	nearPlane = 2.f;
	farPlane = 500.f;
	fov = QUARTER_PI;
	lightFront = { 0, 0, -1 };
	lightUp = { 0, 1, 0 };
	lightx = 1.f;
	lighty = 1.f;
	lightz = 1.f;
	lightvecmult = 15;
	yaw = 270;
	pitch = 0;
}

void Light::Update()
{
	if (OpenGLCamera.camMove) {
		glfwSetInputMode(window_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		glfwSetInputMode(window_ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (sScene.lightMode)
	{
		Vec3 lightPos{ lightx, lighty, lightz };
		if (OpenGLInput.IsKeyDown(Keyboard::W)) {
			lightPos += lightspeed * lightFront;
		}
		if (OpenGLInput.IsKeyDown(Keyboard::S)) {
			lightPos -= lightspeed * lightFront;
		}
		if (OpenGLInput.IsKeyDown(Keyboard::A)) {
			lightPos -= glm::normalize(glm::cross(lightFront, lightUp)) * lightspeed;
		}
		if (OpenGLInput.IsKeyDown(Keyboard::D)) {
			lightPos += glm::normalize(glm::cross(lightFront, lightUp)) * lightspeed;
		}
		if (OpenGLInput.IsKeyDown(Keyboard::Q)) {
			lightPos.y += lightspeed;
		}
		if (OpenGLInput.IsKeyDown(Keyboard::E)) {
			lightPos.y -= lightspeed;
		}
		lightx = lightPos.x;
		lighty = lightPos.y;
		lightz = lightPos.z;
	}

	mouselight(OpenGLInput.get_mouse_pos().x, OpenGLInput.get_mouse_pos().y);

	topPlane = tan(fov / 2) * nearPlane;
	bottomPlane = -topPlane;
	aspect = 1.0f * 2048 / 2048;
	rightPlane = topPlane * aspect;
	leftPlane = -rightPlane;
}

void Light::SetLightVec(int mult) {
	lightvecmult = mult;
}

void Light::mouselight(double xpos, double ypos)
{
	if (OpenGLCamera.camMove)
	{
		if (sScene.lightMode)
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
			lightFront = glm::normalize(front);
		}
	}
}

void Light::ShadInit() {
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 pos;\n"
		"uniform mat4 mvpMat;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = mvpMat * vec4(pos, 1.0f);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f);\n"
		"}\0";
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	shadpgm = glCreateProgram();
	glAttachShader(shadpgm, vertexShader);
	glAttachShader(shadpgm, fragmentShader);
	glLinkProgram(shadpgm);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}