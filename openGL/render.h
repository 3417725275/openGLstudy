#pragma once
#include <glm/glm.hpp>
#include "Data/window.h"


class Window;
class ShaderProgram;
class Model;
class Camera;
class VertShader;
class FragShader;
struct GLFWwindow;
typedef void (*mouse_callback)(GLFWwindow*, double, double);
typedef void (*scroll_callback)(GLFWwindow*, double, double);
class RenderProgram
{
public:
	RenderProgram(Window* window, Camera* camera, ShaderProgram* shaderProgram, Model* model);

	~RenderProgram() {}

public:
	void render();
	void activeCallback()
	{
		set_frame_buffer_size_callback();
		set_mouse_callback();
		set_scroll_callback();
	}

public:
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	glm::mat4 getModelMatrix();

public:
	//�ص������������ӿڴ�СΪ���ڵĴ�С��ÿ�����ڵ�����С��ʱ������������
	void set_frame_buffer_size_callback(frameBuffer_size_callback callback = nullptr);
	//���ת��ʱ������������ӽ�
	void set_mouse_callback(mouse_callback callback = nullptr);
	//�����ƶ�������������ӽ�
	void set_scroll_callback(scroll_callback callback = nullptr);

	void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);

private:
	void processInput(GLFWwindow* window);
	bool check();

public:
	void setCamera(Camera* camera) { _camera = camera; }
	Camera* getCamera() { return _camera; }
	void setShader(FragShader* fragshader) { _fragshader = fragshader; }
	FragShader* getFragShader() { return _fragshader; }
	void setShader(VertShader* vertshader) { _vertshader = vertshader; }
	VertShader* getVertShader() { return _vertshader; }
	void setShaderProgram(ShaderProgram* shaderProgram) { _shaderProgram = shaderProgram; }
	ShaderProgram* getShaderProgram() { return _shaderProgram; }

private:
	Window* _window = nullptr;
	Camera* _camera = nullptr;
	Model* _model = nullptr;
	ShaderProgram* _shaderProgram = nullptr;
	VertShader* _vertshader = nullptr;
	FragShader* _fragshader = nullptr;
};