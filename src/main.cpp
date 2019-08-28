#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils.h"
#include "shader.h"
#include "mesh.h"
#include "function.h"

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
static int intAxisNum = 1;

int main()
{
	// Window dimensions
	const int WIDTH = 1280;
	const int HEIGHT = 720;

	// Enter and parse the function
	std::string function;
	printf("Enter a function: ");
	std::getline(std::cin, function);
	std::vector<std::string> parsed = ParseFunction(function);

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

	// Plane for intersection visualization
	float planeCoord = 2.0f;
	std::vector<float> yzPlaneVertices
	{
		0.0f, -1.0f, -1.0f,
		0.0f, -1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, -1.0f,
		0.0f, -1.0f, -1.0f
	};

	std::vector<float> xzPlaneVertices
	{
		-1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, -1.0f
	};
	std::vector<float> xyPlaneVertices
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f
	};
	Mesh xyplane(xyPlaneVertices);
	Mesh xzplane(xzPlaneVertices);
	Mesh yzplane(yzPlaneVertices);

	// Generate the function's curve as plotted on a 2D xy plane
	std::vector<float> vertices;
	MathFunction(vertices, parsed, a, b, incrementSize);
	Mesh curve(vertices);

	// Create the rotated 3D mesh
	Mesh shape = CreateRotatedMesh(vertices, numRotations, axis);

#ifdef QTCREATOR
    Shader shader("../res/shaders/vertex.glsl", "../res/shaders/fragment.glsl");	// Used for function 2D and 3D mesh drawing
    Shader defaultShader("../res/shaders/vDefault.glsl", "../res/shaders/fDefault.glsl");	// Used for axes mesh drawing
#else
    Shader shader("./res/shaders/vertex.glsl", "./res/shaders/fragment.glsl");	// Used for function 2D and 3D mesh drawing
    Shader defaultShader("./res/shaders/vDefault.glsl", "./res/shaders/fDefault.glsl");	// Used for axes mesh drawing
#endif
	// Variable used for model matrix rotation
	double xpos = 0.0;

	while(!glfwWindowShouldClose(window))
	{
		// Clear window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Process mouse input
		processMouse(window, xpos);

		// Setup full color shader
		defaultShader.UseProgram();

		// Perspective projection matrix creation
		glm::mat4 projection = glm::perspective(glm::radians(fov), static_cast<float>(WIDTH) / HEIGHT, 0.1f, 50.0f);
		defaultShader.SetMat4(projection, "projection");

		// Model matrix creation
		glm::mat4 model(1.0);
		defaultShader.SetMat4(model, "model");

		// Angle to rotate view matrix by, normalized
		float nAngle = Map(0.0f, WIDTH, -180.0f, 180.0f, static_cast<float>(xpos));

		// View matrix creation
		glm::mat4 view(1.0);
		view = glm::translate(view, { 0.0f, -0.25f, -10.0f });	// Move the camera above and away from the mesh
		view = glm::rotate(view, glm::radians(20.0f), { 1.0f, 0.0f, 0.0f }); // Tilt the camera downwards
		view = glm::rotate(view, glm::radians(nAngle), { 0.0, 1.0, 0.0 }); // Adjust position based on mouse
		defaultShader.SetMat4(view, "view");	// Pass the rotated view matrix to the shaders

		// If automatic rotation is enabled
		if(rotate)
		{
			view = glm::rotate(view, glm::radians(25.0f * static_cast<float>(glfwGetTime())), { 0.0, 1.0, 0.0 });
			defaultShader.SetMat4(view, "view");
		}

		// Draw the xAxis
		defaultShader.SetVec3(1.0f, 0.0f, 0.0f, "col");
		xAxis.DrawNonIndexed();

		// Draw the yAxis
		defaultShader.SetVec3(0.0f, 0.0f, 1.0f, "col");
		yAxis.DrawNonIndexed();

		// Draw the zAxis
		defaultShader.SetVec3(0.0f, 1.0f, 0.0f, "col");
		zAxis.DrawNonIndexed();

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

		// Switch to mesh shader
		shader.UseProgram();
		shader.SetInt(intAxisNum, "plane");
		shader.SetFloat(planeCoord, "planeCoord");

		// Set all uniforms for mesh
		if(lighting)
		{
			shader.SetVec3(1.0f, 1.0f, 1.0f, "col");
			shader.SetVec3(0.0f, 50.0f, 0.0f, "lightPos");
		} else
		{
			shader.SetVec3(0.2f, 0.2f, 0.2f, "col");
			shader.SetVec3(0.0f, -500.0f, 0.0f, "lightPos");
		}
		shader.SetMat4(model, "model");
		shader.SetMat4(view, "view");
		shader.SetMat4(projection, "projection");

		if(show3D)	// Draw the 3D mesh
			shape.Draw();
		else		// Draw the 2D curve
			curve.DrawNonIndexed();

		// Set the correct position for the intersection plane
		defaultShader.UseProgram();
		if(intAxisNum == 0)
			model = glm::translate(model, glm::vec3(planeCoord, 0.0f, 0.0f));
		else if(intAxisNum == 1)
			model = glm::translate(model, glm::vec3(0.0f, planeCoord, 0.0f));
		else if(intAxisNum == 2)
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, planeCoord));
		defaultShader.SetMat4(model, "model");

		// Draw the plane
		defaultShader.SetVec3(1.0f, 0.0f, 0.0f, "col");
		if(intAxisNum == 0)
			yzplane.DrawNonIndexed();
		else if(intAxisNum == 1)
			xzplane.DrawNonIndexed();
		else if(intAxisNum == 2)
			xyplane.DrawNonIndexed();

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

	if(key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		// Change axis
		intAxisNum++;
		if(intAxisNum >= 3) intAxisNum = 0;
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
