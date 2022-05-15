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
		while (!glfwWindowShouldClose(_window))//���GLFW�Ƿ�Ҫ����
		{
			processInput();
			glfwSwapBuffers(_window); //������ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
			glfwPollEvents();//�����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã���
		}
		glfwTerminate();//ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
	}

	//�ص������������ӿڴ�СΪ���ڵĴ�С��ÿ�����ڵ�����С��ʱ������������
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
		if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//ESC�����º󣬹رմ���
			glfwSetWindowShouldClose(_window, true);

	}

private:
	unsigned int _width;
	unsigned int _height;
	GLFWwindow* _window;
};