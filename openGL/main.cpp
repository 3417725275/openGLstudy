#include "Data/window.h"
#include "Data/shader.h"
#include "Data/model.h"
#include "Data/camera.h"
#include "Init.h"
#include "render.h"

int main()
{
	Init::getInstance().Initialization();
	Model model("../resource/nanosuit/nanosuit.obj");
	Window window(800, 800, "helloworld");
	Camera camera;
	VertShader vertShader("./Shader/vertexShader.vert");
	FragShader fragShader("./Shader/fragShader.frag");
	ShaderProgram shader_program(vertShader.getID(), fragShader.getID());
	RenderProgram render_program(&window, &camera, &shader_program, &model);
	/*render_program.activeCallback();
	render_program.render();*/
	return 0;
}



