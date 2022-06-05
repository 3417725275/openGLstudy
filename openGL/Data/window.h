#pragma once
#include <iostream>
#include <GLFW/glfw3.h>

#include "shader.h"

typedef void (*frameBuffer_size_callback)(GLFWwindow*, int, int);

class Window
{
public:
	Window(unsigned int width, unsigned height, const char* name) :_width(width), _height(height), _window(nullptr)
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
	unsigned int getWidth() { return _width; }
	unsigned int getHeight() { return _height; }

private:
	unsigned int _width;
	unsigned int _height;
	GLFWwindow* _window;

};