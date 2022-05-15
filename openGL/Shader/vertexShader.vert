#version 330 core
layout (location =0) in vec3 aPos;
layout (location =1) in vec3 aColor;

out vec3 ourColor;

void main()
{
	gl_Position=vec4(aPos,1.0);
	ourColor=aColor;
}


//gl_Position为预定义变量，是顶点着色器的输出