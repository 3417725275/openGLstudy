#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "shader.h"

typedef void (*frameBuffer_size_callback)(GLFWwindow*, int, int);
typedef void (*mouse_callback)(GLFWwindow*, double, double);
typedef void (*scroll_callback)(GLFWwindow*, double, double);

class Window
{
public:
	Window(const unsigned int width, const unsigned height, const char* name) :_width(width), _height(height), _window(nullptr)
	{
		_window = glfwCreateWindow(width, height, name, NULL, NULL);
		if (_window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}
	}
	~Window() = default;
public:
	void active() const
	{
		glfwMakeContextCurrent(_window);
	}

public:
	GLFWwindow* getGLFWwindow() { return _window; }

private:
	const unsigned int _width;
	const unsigned int _height;
	GLFWwindow* _window;

};