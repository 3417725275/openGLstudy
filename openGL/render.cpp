#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "render.h"
#include "Data/window.h"
#include "Data/shader.h"
#include "Data/model.h"
#include "Data/camera.h"

RenderProgram::RenderProgram(Window* window, Camera* camera, ShaderProgram* shaderProgram, Model* model)
	:_window(window), _camera(camera), _shaderProgram(shaderProgram), _model(model)
{

}

void RenderProgram::render() const
{
	if (!check())
		return;
	set_frame_buffer_size_callback();
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	_window->active();
	glEnable(GL_DEPTH_TEST);
	auto window = _window->getGLFWwindow();

	while (!glfwWindowShouldClose(window))//检查GLFW是否被要求退
	{
		processInput();
		glfwSwapBuffers(window); //交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwPollEvents();//检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
	}
	glfwTerminate();//确释放/删除之前的分配的所有资源
}

void RenderProgram::activeCallback()
{
	set_frame_buffer_size_callback();
	set_mouse_callback();
	set_scroll_callback();
}

void RenderProgram::set_frame_buffer_size_callback(frameBuffer_size_callback callback) const
{
	if (callback != nullptr)
		glfwSetFramebufferSizeCallback(_window->getGLFWwindow(), callback);
	else
		glfwSetFramebufferSizeCallback(_window->getGLFWwindow(), [](
			GLFWwindow* window, int width, int height) {
				glViewport(0, 0, width, height);
			});
}

// glfw: whenever the mouse moves, this callback is called
void RenderProgram::set_mouse_callback(mouse_callback callback) const
{
	if (callback != nullptr)
		glfwSetCursorPosCallback(_window->getGLFWwindow(), callback);
	else
		glfwSetCursorPosCallback(_window->getGLFWwindow(), []
		(GLFWwindow* window, double xposIn, double yposIn) {
				float xpos = static_cast<float>(xposIn);
				float ypos = static_cast<float>(yposIn);

				if (firstMouse)
				{
					lastX = xpos;
					lastY = ypos;
					firstMouse = false;
				}

				float xoffset = xpos - lastX;
				float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

				lastX = xpos;
				lastY = ypos;

				_camera->ProcessMouseMovement(xoffset, yoffset);
			});

}

void RenderProgram::set_scroll_callback(scroll_callback callback) const
{
	if (callback != nullptr)
		glfwSetScrollCallback(_window->getGLFWwindow(), callback);
	else
		glfwSetScrollCallback(_window->getGLFWwindow(), []
		(GLFWwindow* window, double xoffset, double yoffset) {
				_camera.ProcessMouseScroll(static_cast<float>(yoffset));
			});
}

bool RenderProgram::check() const
{
	if (_window && _camera && _model && _shaderProgram)
		return true;
	return false;
}

void RenderProgram::processInput(GLFWwindow* window) const
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//ESC键按下后，关闭窗口
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		_camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		_camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		_camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		_camera.ProcessKeyboard(RIGHT, deltaTime);

}