#include "opengl.h"

#define DEFAULT_SIZE 1
#define INIT_WNDPOS_X 440
#define INIT_WNDPOS_Y 30
#define IMAGE_WIDTH_MAX 1600
#define IMAGE_HEIGHT_MAX 900

void windowCloseCallback(GLFWwindow *window)
{
	PostQuitMessage(0);
}

ImageWindow::ImageWindow():tWidth(DEFAULT_SIZE),tHeight(DEFAULT_SIZE)
{
	if(!glfwInit())exit(-1);
	gWindow = glfwCreateWindow(tWidth, tHeight, "", NULL, NULL);
	glfwMakeContextCurrent(gWindow);
	glfwSetInputMode(gWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetWindowCloseCallback(gWindow, windowCloseCallback);
	glfwSetWindowPos(gWindow, INIT_WNDPOS_X, INIT_WNDPOS_Y);
}

ImageWindow::~ImageWindow()
{
	if(texDsp != 0)glDeleteTextures(1 , &texDsp);
	glfwDestroyWindow(gWindow);
	glfwTerminate();
}

int32_t ImageWindow::glGetKey()
{
	//キーが押されていたら
	if(glfwGetKey(gWindow, GLFW_KEY_ESCAPE ) == GLFW_PRESS)return GLFW_KEY_ESCAPE;
	if(glfwGetKey(gWindow, GLFW_KEY_SPACE ) == GLFW_PRESS)return GLFW_KEY_SPACE;
	if(glfwGetKey(gWindow, GLFW_KEY_ENTER ) == GLFW_PRESS)return GLFW_KEY_ENTER;

	return 0;
}

void ImageWindow::display()
{
	//描画処理
	if(texDsp != 0){
		glfwSetWindowSize(gWindow, tWidth, tHeight);
		render(texDsp);
	}
	glfwSwapBuffers(gWindow);
}

void ImageWindow::render(const GLuint i)
{
	//OpenGLの描画処理
	glViewport(0, 0, tWidth, tHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D , i);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);  
	glTexCoord2f(0.0, 0.0); glVertex2f(-1.0,-1.0);  
	glTexCoord2f(0.0, 1.0); glVertex2f(-1.0, 1.0);  
	glTexCoord2f(1.0, 1.0); glVertex2f( 1.0, 1.0);  
	glTexCoord2f(1.0, 0.0); glVertex2f( 1.0,-1.0);  
	glEnd();
	glDisable(GL_TEXTURE_2D);
#ifdef _DEBUG
	printf("render texDsp :%d\n", i);
#endif
}

void ImageWindow::loadTexture(class SearchFile &searchFile)
{
	std::string fPath = searchFile.getFilePath();

	if(texDsp != 0)glDeleteTextures(1 , &texDsp);
#ifdef _DEBUG
	printf("loadTexture :%s\n", fPath.c_str());
#endif
	texDsp = SOIL_load_OGL_texture(fPath.c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	glBindTexture(GL_TEXTURE_2D , texDsp);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &tHeight);
	glBindTexture(GL_TEXTURE_2D , 0);

	if( tWidth >= IMAGE_WIDTH_MAX || tHeight >= IMAGE_HEIGHT_MAX){ //画像が大きすぎるときは一定サイズまで小さくする
		tWidth  = tWidth * IMAGE_HEIGHT_MAX / tHeight;
		tHeight = IMAGE_HEIGHT_MAX;
	}
#ifdef _DEBUG
	printf("texDsp :%d\n", texDsp);
	printf("texWidth :%d\n", tWidth);
	printf("texHeight :%d\n", tHeight);
#endif
}