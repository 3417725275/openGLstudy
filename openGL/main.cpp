#pragma once
#include "Data/window.h"
#include "Init.h"

int main()
{
	Init::getInstance().Initialization();
	Window window(800, 800, "helloworld");
	window.render();
	return 0;
}



