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
	// const float TWOPI = 2.0f * 3.1415926f;
	const float PHI = 1.618033f;
	const int WIDTH = 1280;
	const int HEIGHT = 720;
	
	// Bounds of function
	float a, b;
	std::cout << "Enter the bounds of the function." << std::endl;
	std::cout << "Enter a value to start from (a): ";
	std::cin >> a;
	std::cout << "Enter a value to end at (b): ";
	std::cin >> b;
	
	// Number of iterations
	int iterations;
	std::cout << "Enter the number of iterations (lines) each frame: ";
	std::cin >> iterations;
	
	// GLFW initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Window creation
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Title", nullptr, nullptr);
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
	glEnable(GL_DEPTH_TEST);
	
	std::vector<float> vertices;
	std::vector<float> vertices3D;
	
	// TODO: store vertices so that it replaces lines
	
	float incrementSize = 0.05f;
	while(a <= b)
	{	
		// Insert function here
		vertices.push_back(a);				// x
		vertices.push_back(a * a);			// y
		vertices.push_back(0.0);			// z
		
		a += incrementSize;	
	}
	Mesh line(vertices);
	
	Shader shader("../function-rotator/res/shaders/vertex.glsl", "../function-rotator/res/shaders/fragment.glsl");
	shader.useProgram();
	
	glm::mat4 view(1.0);
	shader.setMat4(view, "view");
	
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WIDTH) / HEIGHT, 0.01f, 50.0f);
	shader.setMat4(projection, "projection");
	
	int vertexCount = static_cast<int>(vertices.size() / 3);
	while(!glfwWindowShouldClose(window))
	{
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Bind the mesh
		line.bind();
		
		glm::mat4 model(1.0);
		model = glm::translate(model, {0.0, 0.0, -10.0});
		
		if(rotate)
			model = glm::rotate(model, glm::radians(100.0f * static_cast<float>(glfwGetTime())), {0.0, 1.0, 0.0});
		
		if(show3D)
		{
			// Rotate the positions for phi radians and draw the function
			for(int i = 0; i < iterations; i++)
			{
				model = glm::rotate(model, PHI, {1.0, 0.0, 0.0});
				shader.setMat4(model, "model");
				glDrawArrays(GL_LINE_STRIP, 0, vertexCount);			
			}	
		} else 
		{
			shader.setMat4(model, "model");			
			glDrawArrays(GL_LINE_STRIP, 0, vertexCount);	
		}
		
		// Unbind the mesh
		line.unbind();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
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
