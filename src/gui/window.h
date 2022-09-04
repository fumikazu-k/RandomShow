#pragma once
#include "gl/opengl.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//操作用

class MainWindow
{
public:
	MainWindow();
	~MainWindow();
	void playPause();
	void nextImg(class ImageWindow &imageWindow, class SearchFile &searchFile);
	void refWindow(class ImageWindow &imageWindow, class SearchFile &searchFile);

private:
	bool playFlag; //再生開始フラグ
	HWND hWnd,editCurrentTime,editIntervalTime;
	int32_t intervalTime; //次の描画までの秒数
	int32_t currentTime; //現在の再生時間(秒)

	void loadIntervalTime(); //更新時間変更
};

enum
{
	BUTTON_ID1,
	BUTTON_ID2,
	EDIT_ID1,
	EDIT_ID2,
	TIMER_ID1
};