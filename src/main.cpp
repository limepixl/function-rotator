#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "Mesh.hpp"

// Keyboard input callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Whether to show the 3D view or not
static bool show3D = false;

// Whether to rotate the object
static bool rotate = false;

int main()
{
	// Constants
	const int WIDTH = 1280;
	const int HEIGHT = 720;

	// Bounds of function
	float a, b;
	std::cout << "Enter the bounds of the function." << std::endl;
	std::cout << "Enter a value to start from (a): ";
	std::cin >> a;
	std::cout << "Enter a value to end at (b): ";
	std::cin >> b;

	// Increment size between 2 points on the curve,
	// AKA distance. Affects concentric circle delta.
	float incrementSize;
	std::cout << "Enter the increment size between any 2 X values: " << std::endl;
	std::cout << "(0 for default, 0.05) ";
	std::cin >> incrementSize;

	if(incrementSize <= 0.0f)
		incrementSize = 0.05f;

	// GLFW initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Window creation
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Function rotator", nullptr, nullptr);
	if(window == nullptr)
	{
		std::cout << "Failed to create window!" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback); // Keyboard events callback func

	// GLEW init
	// glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
	}
	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	// Window background color
	glEnable(GL_DEPTH_TEST);

	// Generate the function's curve as plotted on a 2D xy plane
	std::vector<float> vertices;
	for(float s = a; s<=b; s+=incrementSize)
	{
		// Insert function here
		float x = s;
		float y = std::sin(std::cos(s));
		float z = 0.0;
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
	}
	Mesh curve(vertices);

	// The number of times the points will be rotated
	// AKA the number of points in each circle at each x value.
	int iterations = 100;

	// Increment size to rotate by (in degrees)
	// to reach 360 degrees with the number of
	// set iterations.
	float mul = static_cast<float>(360)/iterations;

	// Generate rotated vertices along the circle
	// defined by the vertex's y coord as the radius.
	std::vector<float> vertices3D;
	for(size_t i = 0; i < vertices.size(); i+=3)
	{
		glm::vec3 current(vertices[i], vertices[i+1], vertices[i+2]);

		// There are 360 vertices in each row
		for(int j = 0; j <= iterations; j++)
		{
			glm::mat4 rotator(1.0);

			// Store a rotation along the x axis, by mul*j degrees in the matrix
			rotator = glm::rotate(rotator, glm::radians(mul * j), {1.0, 0.0, 0.0});

			// Rotated vertex
			glm::vec3 rotated = rotator * glm::vec4(current.x, current.y, current.z, 1.0);

			vertices3D.push_back(rotated.x);
			vertices3D.push_back(rotated.y);
			vertices3D.push_back(rotated.z);
		}
	}
	Mesh shape(vertices3D);

	Shader shader("../function-rotator/res/shaders/vertex.glsl", "../function-rotator/res/shaders/fragment.glsl");
	shader.useProgram();

	glm::mat4 view(1.0);
	shader.setMat4(view, "view");

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WIDTH) / HEIGHT, 0.1f, 50.0f);
	shader.setMat4(projection, "projection");

	// Number of vertices that make up the initial curve
	int vertexCount = static_cast<int>(vertices.size() / 3);
	while(!glfwWindowShouldClose(window))
	{
		// Clear window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0);
		model = glm::translate(model, {0.0, 0.0, -10.0});
		shader.setMat4(model, "model");

		if(rotate)
		{
			model = glm::rotate(model, glm::radians(25.0f * static_cast<float>(glfwGetTime())), {0.0, 1.0, 0.0});
			shader.setMat4(model, "model");
		}

		// Draw the 3D generated shape
		if(show3D)
		{
			shape.bind();
			glDrawArrays(GL_LINE_STRIP, 0, shape.vertexCount);
			shape.unbind();
		}
		else // Draw the 2D curve
		{
			curve.bind();
			glDrawArrays(GL_LINE_STRIP, 0, vertexCount);
			curve.unbind();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Delete the window
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		show3D = !show3D;

	if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
		rotate = !rotate;
}
