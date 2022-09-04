#include "gui/window.h"

#ifdef _WIN64
#pragma comment(lib,"lib/glfw3.lib")
#pragma comment(lib,"lib/SOIL.lib")
#pragma comment(lib,"opengl32.lib")
#endif

#define TIMER_INTERVAL 1000 //ms

class MainWindow mainWindow;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	while(GetMessage(&msg, NULL, 0, 0)){ 
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static class ImageWindow imageWindow;
	static class SearchFile searchFile;

	HDC hdc;
	PAINTSTRUCT ps;
	int32_t keyNum;

	switch (msg){

	case WM_KEYDOWN:
		switch(wParam){

		case VK_SPACE:
			mainWindow.playPause();
			break;

		case VK_RETURN:
			mainWindow.nextImg(imageWindow, searchFile);
			break;

		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

		default:break;
		}
		break; //WM_KEYDOWN

	case WM_COMMAND:
		switch(LOWORD(wParam)){

		case BUTTON_ID1:
			mainWindow.playPause();
			break;

		case BUTTON_ID2:
			mainWindow.nextImg(imageWindow, searchFile);
			break;

		default:break;
		}
		break; //WM_COMMAND

	case WM_CREATE:
		imageWindow.loadTexture(searchFile); //テクスチャとして読み込み
		SetTimer(hWnd, TIMER_ID1, TIMER_INTERVAL, NULL);
		break;

	case WM_TIMER:
		//タイマー処理
		mainWindow.refWindow(imageWindow, searchFile);
		keyNum = imageWindow.glGetKey(); //キー入力がないか確認
		switch(keyNum){

		case GLFW_KEY_ESCAPE:
			PostQuitMessage(0);
			break;
		case GLFW_KEY_SPACE:
			mainWindow.playPause();
			break;

		case GLFW_KEY_ENTER:
			mainWindow.nextImg(imageWindow, searchFile);
			break;

		default:break;
		}
		glfwPollEvents(); //×で閉じる用
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_PAINT: //入れておかないと上のWM_TIMERが動作しない
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 5, 155, L"経過秒数", wcslen(L"経過秒数"));
		TextOut(hdc, 205, 155, L"表示秒数", wcslen(L"表示秒数"));
		EndPaint(hWnd, &ps);

		return DefWindowProc(hWnd, msg, wParam, lParam); 

	default: return(DefWindowProc(hWnd, msg, wParam, lParam));
	}

	return(DefWindowProc(hWnd, msg, wParam, lParam));
}