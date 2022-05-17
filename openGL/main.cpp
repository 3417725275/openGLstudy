#pragma once
#include "Data/model.h"
#include "Init.h"

int main()
{
	Init::getInstance().Initialization();
	Model model("../resource/nanosuit/nanosuit.obj");
	//Window window(800, 800, "helloworld");
	//window.render();
	return 0;
}



