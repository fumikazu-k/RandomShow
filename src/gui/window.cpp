#include "window.h"

#define DEFAULT_INTERVALTIME 600 //画像が切り替わるまでの秒数
#define BUF_SIZE 32 //秒数表示用
#define INIT_WNDPOS_X 0
#define INIT_WNDPOS_Y 0
#define SECOND_MAX 86400 //扱う最大秒(暫定で1日の秒数)

MainWindow::MainWindow():intervalTime(DEFAULT_INTERVALTIME)
{
	WNDCLASS wc;
	wchar_t setIntervalTime[BUF_SIZE];
	_itow_s(DEFAULT_INTERVALTIME, setIntervalTime, 10); //windowに表示用

	CoInitialize(NULL);
#ifdef _DEBUG
	FILE* fpCon = NULL;
	AllocConsole();
	freopen_s(&fpCon, "CONOUT$", "w", stdout);
	freopen_s(&fpCon, "CONIN$", "r", stdin);
#endif

	HINSTANCE hInst = GetModuleHandle(NULL);
	//ウィンドウクラスの登録
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInst;
	wc.hIcon		 = LoadIcon(hInst , L"ICON_1");
	wc.hCursor		 = LoadCursor(NULL , IDC_ARROW);
	wc.hbrBackground = WHITE_BRUSH;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = TEXT("menu") ;
	if (!RegisterClass(&wc)) exit(-1);
	//hWnd = CreateWindow(TEXT("menu"), TEXT("RandomShow"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 415, 218, NULL, NULL, hInst, NULL);
	hWnd = CreateWindow(TEXT("menu"), TEXT("RandomShow"), WS_OVERLAPPEDWINDOW, INIT_WNDPOS_X, INIT_WNDPOS_Y, 415, 218, NULL, NULL, hInst, NULL);
	CreateWindow(TEXT("BUTTON") , TEXT("再生/停止") ,WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,0 , 0 , 200 , 150 ,hWnd , (HMENU)BUTTON_ID1 , hInst , NULL);
	CreateWindow(TEXT("BUTTON") , TEXT("次の画像") ,WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,200 , 0 , 200 , 150 ,hWnd , (HMENU)BUTTON_ID2 , hInst , NULL);
	editCurrentTime = CreateWindow(TEXT("EDIT") , NULL , WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT , 80 , 150 , 120 , 30 , hWnd , (HMENU)EDIT_ID1 ,hInst , NULL);
	editIntervalTime = CreateWindow(TEXT("EDIT") , NULL , WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT , 280 , 150 , 120 , 30 , hWnd , (HMENU)EDIT_ID2 ,hInst , NULL);
	SetWindowText(editIntervalTime, setIntervalTime);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
#ifdef _DEBUG
	printf("finish wndInit\n");
#endif
}

MainWindow::~MainWindow()
{
	KillTimer(hWnd, TIMER_ID1);
	CoUninitialize();
	DestroyWindow(hWnd);
}

void MainWindow::refWindow(class ImageWindow &imageWindow, class SearchFile &searchFile)
{
	char text[BUF_SIZE];
	wchar_t wtext[BUF_SIZE];

	if(playFlag){
		++currentTime;
		if(currentTime > intervalTime)nextImg(imageWindow, searchFile);
		imageWindow.display();
	}
	
	//現在の再生時間を表示
	sprintf_s(text, BUF_SIZE, "%I32d", currentTime);
	mbstowcs_s(NULL, wtext, BUF_SIZE, text, strlen(text) + 1);
	SetWindowText(editCurrentTime , wtext);
}

void MainWindow::playPause()
{
	playFlag = playFlag ? false : true;
	loadIntervalTime();
}

void MainWindow::nextImg(class ImageWindow &imageWindow, class SearchFile &searchFile)
{
	currentTime = 0;
	imageWindow.loadTexture(searchFile); //次の画像読み込み
	imageWindow.display();
	loadIntervalTime();
}

void MainWindow::loadIntervalTime()
{
	wchar_t *strText = NULL;
	wchar_t setIntervalTime[BUF_SIZE];

	try{
		strText = new wchar_t[GetWindowTextLength(editIntervalTime) + 1];
		if(strText != NULL)GetWindowText(editIntervalTime , strText , GetWindowTextLength(editIntervalTime) + 1);
		intervalTime = _wtof(strText); //小数点切り捨て
		delete[] strText;
	}
	catch(std::bad_alloc& ex){
#ifdef _DEBUG
		printf("bad_alloc: %s\n", ex.what());
#endif
		intervalTime = DEFAULT_INTERVALTIME;
	}

	if(SECOND_MAX < intervalTime)intervalTime = SECOND_MAX;
	else if(intervalTime < 0)intervalTime = DEFAULT_INTERVALTIME;
	_itow_s(intervalTime, setIntervalTime, 10); //windowに表示用
	SetWindowText(editIntervalTime, setIntervalTime);
}