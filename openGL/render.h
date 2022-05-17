#pragma once


class Window;
class ShaderProgram;
class Model;
class Camera;
class VertShader;
class FragShader;
class GLFWwindow;
class RenderProgram
{
public:
	RenderProgram(Window* window, Camera* camera, ShaderProgram* shaderProgram, Model* model);
	~RenderProgram() = default;

public:
	void render() const;

	void activeCallback();
public:
	//�ص������������ӿڴ�СΪ���ڵĴ�С��ÿ�����ڵ�����С��ʱ������������
	void set_frame_buffer_size_callback(frameBuffer_size_callback callback = nullptr) const;
	//���ת��ʱ������������ӽ�
	void set_mouse_callback(mouse_callback callback = nullptr) const;
	//�����ƶ�������������ӽ�
	void set_scroll_callback(scroll_callback callback = nullptr) const;
	
private:
	void processInput(GLFWwindow* window) const;
	bool check() const;

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

}