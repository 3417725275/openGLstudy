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
	//回调函数，设置视口大小为窗口的大小，每当窗口调整大小的时候调用这个函数
	void set_frame_buffer_size_callback(frameBuffer_size_callback callback = nullptr) const;
	//鼠标转动时，调整摄像机视角
	void set_mouse_callback(mouse_callback callback = nullptr) const;
	//滚轮移动，调整摄像机视角
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