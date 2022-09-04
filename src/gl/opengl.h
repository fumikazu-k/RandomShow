#pragma once
#include "SOIL/SOIL.h"
#include "GLFW/glfw3.h"
#include "io/file.h"

void windowCloseCallback(GLFWwindow *window);

class ImageWindow
{
public:
	ImageWindow();
	~ImageWindow();
	int32_t glGetKey();
	void display();
	void loadTexture(class SearchFile &searchFile);

private:
	GLFWwindow *gWindow;
	GLuint texDsp;
	int32_t tWidth;
	int32_t tHeight;

	void render(const GLuint i);
};