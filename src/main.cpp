#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "utils.h"
#include "Shader.hpp"
#include "Mesh.hpp"

// Rotate all points of a mesh around an axis
std::vector<float> RotateAroundAxis(std::vector<float>& vertices2D, int numRotations, char axis);

// Map a value from one range to another
inline float Map(float oldmin, float oldmax, float newmin, float newmax, float current)
{
	return newmin + (newmax - newmin) * ((current - oldmin) / (oldmax - oldmin));
}

// Process mouse input
void processMouse(GLFWwindow* window, double& xpos);

// Keyboard input callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Mouse scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Window resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static bool lighting = true;	// Whether to show the lighting
static bool wireframe = true;	// Whether to show the mesh as a wireframe mesh
static float fov = 45.0f;		// Field of view for projection matrix
static bool show3D = false;		// Whether to show the 3D view or not
static bool rotate = true;		// Whether to rotate the object

int main()
{
	// Window dimensions
	const int WIDTH = 1280;
	const int HEIGHT = 720;

	// Enter and parse the function
	std::string function;
	std::getline(std::cin, function);
	ParseFunction(function);

	// Bounds of the function
	float a, b;
	printf("Enter the bounds of the function.\n");
	printf("Enter a value to start from (a): ");
	scanf("%f", &a);
	printf("Enter a value to end at (b): ");
	scanf("%f", &b);

	// Increment size between every 2 points on the curve.
	// AKA the step size for each x.
	float incrementSize = 0.0f;
#ifdef _DEBUG
	printf("Enter the increment size between any 2 X values: \n");
	printf("(0 for default, 0.01) ");
	scanf("%f", &incrementSize);
#endif

	// The number of times the points will be rotated, and
	// the number of points in each circle at each x value.
	int numRotations = 0;
#ifdef _DEBUG
	printf("Enter the desired number of rotations for each vertex: \n");
	printf("(0 for default, 100) ");
	scanf("%d", &numRotations);
#endif

	// Default values
	if(incrementSize <= 0.0f)
		incrementSize = 0.01f;
	if(numRotations == 0)
		numRotations = 100;

	// The desired axis of rotation
	char axis;
	printf("Enter the desired axis of rotation:\n");
	printf("(X, Y or Z) ");
	scanf(" %c", &axis);
	if(axis != 'x' && axis != 'X' && axis != 'y' && axis != 'Y' && axis != 'z' && axis != 'Z')
	{
		printf("Invalid axis entered!\n");
		glfwTerminate();
		return 0;
	}

	// The desired intersection axis
	char intAxis;
	printf("Enter the desired axis of intersection:\n");
	printf("(X, Y or Z) ");
	scanf(" %c", &intAxis);
	
	int intAxisNum;
	if(intAxis == 'x' || intAxis == 'X')
		intAxisNum = 0;
	else if(intAxis == 'y' || intAxis == 'Y')
		intAxisNum = 1;
	else if(intAxis == 'z' || intAxis == 'Z')
		intAxisNum = 2;
	else
	{
		printf("Invalid axis entered!\n");
		glfwTerminate();
		return 0;
	}

	// GLFW initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	// Window creation
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Function rotator", nullptr, nullptr);
	if(window == nullptr)
	{
		printf("Failed to create window!\n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSwapInterval(1); // VSync

	// GLAD init
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD. Please make sure you have updated your graphics drivers.\n");
		printf("If this error persists after updating, your GPU does not support OpenGL 2.1, which is the minimum requirement.\n");
		return -1;
	}
	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	// Window background color
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate a mesh for each axis
	std::vector<float> xAxisV;
	std::vector<float> yAxisV;
	std::vector<float> zAxisV;
	for(float s = -10.0f; s <= 10.0f; s += 2.0f)
	{
		xAxisV.push_back(s);
		xAxisV.push_back(0.0f);
		xAxisV.push_back(0.0f);

		yAxisV.push_back(0.0f);
		yAxisV.push_back(s);
		yAxisV.push_back(0.0f);

		zAxisV.push_back(0.0f);
		zAxisV.push_back(0.0f);
		zAxisV.push_back(s);
	}
	Mesh xAxis(xAxisV);
	Mesh yAxis(yAxisV);
	Mesh zAxis(zAxisV);

	// Generate the function's curve as plotted on a 2D xy plane
	std::vector<float> vertices;
	for(float s = a; s <= b; s += incrementSize)
	{
		// Insert function here
		float x = s;
		float y = sin(x);
		float z = 0.0f;

		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
	}
	Mesh curve(vertices);

	// Plane for intersection visualization
	float planeCoord = 2.0f;
	std::vector<float> verticesPlane;

	if(intAxisNum == 0) // X
	{
		verticesPlane =
		{
			0.0f, -1.0f, -1.0f,
			0.0f, -1.0f, 1.0f,
			0.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 1.0f,
			0.0f, 1.0f, -1.0f,
			0.0f, -1.0f, -1.0f
		};
	} else if(intAxisNum == 1) // Y
	{
		verticesPlane =
		{
			-1.0f, 0.0f, -1.0f,
			1.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f,
			-1.0f, 0.0f, 1.0f,
			-1.0f, 0.0f, -1.0f
		};
	} else if(intAxisNum == 2) // Z
	{
		verticesPlane =
		{
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f
		};
	}
	Mesh plane(verticesPlane);

	// Generate rotated vertices along the circle
	// defined by the function's value as the radius.
	std::vector<float> vertices3D = RotateAroundAxis(vertices, numRotations, axis);

	// This is the number of actual vertices - the number of rotations
	// for each vertex. It's basically the number of faces.
	size_t numIterations = vertices3D.size() / 3 - numRotations - 1;

	// Store indices
	std::vector<unsigned int> indices;
	for(unsigned int i = 0; i < numIterations; i++)
	{
		// ...
		// 3 2 7 6 ...
		// 0 1 4 5 ...

		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + numRotations + 1);
		indices.push_back(i + numRotations + 1);
		indices.push_back(i + numRotations);
		indices.push_back(i);
	}

	// Calculate normal vectors
	std::vector<float> normals;
	size_t rotationsPerVertex = static_cast<size_t>(3) * numRotations;
	for(size_t i = 0; i < vertices3D.size() - rotationsPerVertex; i += 3)
	{
		// The first of 2 triangles in each face
		// C
		// |\
		// | \
		// |  \
		// A---B

		glm::vec3 A{ vertices3D[i], vertices3D[i + 1], vertices3D[i + 2] };
		glm::vec3 B{ vertices3D[i + 3], vertices3D[i + 4], vertices3D[i + 5] };
		glm::vec3 C{ vertices3D[i + rotationsPerVertex], vertices3D[i + 1 + rotationsPerVertex], vertices3D[i + 2 + rotationsPerVertex] };

		// For the triangle ABC
		glm::vec3 u = B - A;
		glm::vec3 v = C - A;

		// The cross product is the vector that 
		// has a direction normal to the uv plane.
		glm::vec3 normal = glm::cross(u, v);

		normals.push_back(normal.x);
		normals.push_back(normal.y);
		normals.push_back(normal.z);
	}
	Mesh shape(vertices3D, indices, normals);

	Shader shader("./res/shaders/vertex.glsl", "./res/shaders/fragment.glsl");	// Used for function 2D and 3D mesh drawing
	Shader defaultShader("./res/shaders/vDefault.glsl", "./res/shaders/fDefault.glsl");	// Used for axes mesh drawing

	// Variable used for model matrix rotation
	double xpos = 0.0;

	while(!glfwWindowShouldClose(window))
	{
		// Clear window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Process mouse input
		processMouse(window, xpos);

		// Setup full color shader
		defaultShader.useProgram();

		// Perspective projection matrix creation
		glm::mat4 projection = glm::perspective(glm::radians(fov), static_cast<float>(WIDTH) / HEIGHT, 0.1f, 50.0f);
		defaultShader.setMat4(projection, "projection");

		// Model matrix creation
		glm::mat4 model(1.0);
		defaultShader.setMat4(model, "model");

		// Angle to rotate view matrix by, normalized
		float nAngle = Map(0.0f, WIDTH, -180.0f, 180.0f, static_cast<float>(xpos));

		// View matrix creation
		glm::mat4 view(1.0);
		view = glm::translate(view, { 0.0f, -0.25f, -10.0f });	// Move the camera above and away from the mesh
		view = glm::rotate(view, glm::radians(20.0f), { 1.0f, 0.0f, 0.0f }); // Tilt the camera downwards
		view = glm::rotate(view, glm::radians(nAngle), { 0.0, 1.0, 0.0 }); // Adjust position based on mouse
		defaultShader.setMat4(view, "view");	// Pass the rotated view matrix to the shaders

		// If automatic rotation is enabled
		if(rotate)
		{
			view = glm::rotate(view, glm::radians(25.0f * static_cast<float>(glfwGetTime())), { 0.0, 1.0, 0.0 });
			defaultShader.setMat4(view, "view");
		}

		// Draw the xAxis
		defaultShader.setVec3(1.0f, 0.0f, 0.0f, "col");
		xAxis.drawNonIndexed();

		// Draw the yAxis
		defaultShader.setVec3(0.0f, 0.0f, 1.0f, "col");
		yAxis.drawNonIndexed();

		// Draw the zAxis
		defaultShader.setVec3(0.0f, 1.0f, 0.0f, "col");
		zAxis.drawNonIndexed();

		// Process plane position
		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			// Get cursor pisition
			double x, y;
			glfwGetCursorPos(window, &x, &y);

			if(intAxisNum == 0)
				planeCoord = Map(0.0f, WIDTH, -10.0f, 10.0f, (float)x);
			else if(intAxisNum == 1)
				planeCoord = Map(HEIGHT, 0.0f, -5.0f, 5.0f, (float)y);
			else if(intAxisNum == 2)
				planeCoord = Map(WIDTH, 0.0f, -10.0f, 10.0f, (float)x);
		}

		glm::mat4 temp = model;
		if(intAxisNum == 0)
			model = glm::translate(model, glm::vec3(planeCoord, 0.0f, 0.0f));
		else if(intAxisNum == 1)
			model = glm::translate(model, glm::vec3(0.0f, planeCoord, 0.0f));
		else if(intAxisNum == 2)
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, planeCoord));
		defaultShader.setMat4(model, "model");

		// Draw the plane
		defaultShader.setVec3(1.0f, 0.0f, 0.0f, "col");
		plane.drawNonIndexed();

		// Reset position of model
		model = temp;
		defaultShader.setMat4(model, "model"); 

		// Switch to mesh shader
		shader.useProgram();
		shader.setInt(intAxisNum, "plane");
		shader.setFloat(planeCoord, "planeCoord");

		// Set all uniforms for mesh
		if(lighting)
		{
			shader.setVec3(1.0f, 1.0f, 1.0f, "col");
			shader.setVec3(0.0f, 50.0f, 0.0f, "lightPos");
		} else
		{
			shader.setVec3(0.2f, 0.2f, 0.2f, "col");
			shader.setVec3(0.0f, -500.0f, 0.0f, "lightPos");
		}
		shader.setMat4(model, "model");
		shader.setMat4(view, "view");
		shader.setMat4(projection, "projection");

		if(show3D)	// Draw the 3D mesh
			shape.draw();
		else		// Draw the 2D curve
			curve.drawNonIndexed();

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

	if(key == GLFW_KEY_R && action == GLFW_PRESS)
		rotate = !rotate;

	if(key == GLFW_KEY_L && action == GLFW_PRESS)
		lighting = !lighting;

	if(key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		if(wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			wireframe = false;
		} else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframe = true;
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Decrease the fov if the user scrolls down
	fov -= 2.0f * static_cast<float>(yoffset);

	// If the value is too low
	if(fov < 5.0f)
		fov = 5.0f;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

std::vector<float> RotateAroundAxis(std::vector<float>& vertices2D, int numRotations, char axis)
{
	// Increment size to rotate by (in degrees),
	// to reach 360 degrees with the number of
	// set iterations.
	float mul = 360.0f / numRotations;

	std::vector<float> vertices3D;
	for(size_t i = 0; i < vertices2D.size(); i += 3)
	{
		glm::vec3 current(vertices2D[i], vertices2D[i + 1], vertices2D[i + 2]);

		// Complete a full circle 
		for(int j = 0; j < numRotations; j++)
		{
			glm::mat4 rotator(1.0);

			if(axis == 'x' || axis == 'X')
			{
				// Rotate along the X axis
				rotator = glm::rotate(rotator, glm::radians(mul * j), { 1.0, 0.0, 0.0 });
			} else if(axis == 'y' || axis == 'Y')
			{
				// Rotate along the Y axis
				rotator = glm::rotate(rotator, glm::radians(mul * j), { 0.0, 1.0, 0.0 });
			} else if(axis == 'z' || axis == 'Z')
			{
				// Rotate along the Z axis
				rotator = glm::rotate(rotator, glm::radians(mul * j), { 0.0, 0.0, 1.0 });
			} else
			{
				printf("Axis %c is not supported!\n", axis);
			}

			// Rotated vertex
			glm::vec4 rotatedCurrent = rotator * glm::vec4(current, 1.0);

			vertices3D.push_back(rotatedCurrent.x);
			vertices3D.push_back(rotatedCurrent.y);
			vertices3D.push_back(rotatedCurrent.z);
		}
	}

	return vertices3D;
}

void processMouse(GLFWwindow* window, double& xpos)
{
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if(rotate)
			rotate = !rotate;

		double ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
	}
}
