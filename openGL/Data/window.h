#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef void (*framebuffer_size_callback)(GLFWwindow*, int, int);
typedef void (*mouse_callback)(GLFWwindow*, double, double);
typedef void (*scroll_callback)(GLFWwindow*, double, double);

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
	~Window() {}

	void active() const
	{
		glfwMakeContextCurrent(_window);
	}

	void render()
	{
		active();
		while (!glfwWindowShouldClose(_window))//检查GLFW是否被要求退
		{
			processInput();
			glfwSwapBuffers(_window); //交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
			glfwPollEvents();//检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
		}
		glfwTerminate();//确释放/删除之前的分配的所有资源
	}

	//回调函数，设置视口大小为窗口的大小，每当窗口调整大小的时候调用这个函数
	void setFramebufferSizeCallback(framebuffer_size_callback callback = nullptr) 
	{
		if (callback != nullptr)
			glfwSetFramebufferSizeCallback(_window, callback);
		else
			glfwSetFramebufferSizeCallback(_window, [](
				GLFWwindow* window, int width, int height) {
					glViewport(0, 0, width, height);
				});
	}

	void setMouseCallback(mouse_callback callback = nullptr) 
	{

	}

	void setScrollCallback(scroll_callback callback = nullptr) 
	{
		
	}

private:
	void processInput()
	{
		if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//ESC键按下后，关闭窗口
			glfwSetWindowShouldClose(_window, true);

	}

private:
	unsigned int _width;
	unsigned int _height;
	GLFWwindow* _window;
};