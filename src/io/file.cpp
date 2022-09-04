#include "file.h"
#include "calc/random.h"
#include "gui/window.h"

#define BUF_SIZE 512 //パスを読み込む用
#define EXTENSION_NUM 3

SearchFile::SearchFile()
{
	std::wstring dirName;

	getDirName(dirName); 

	if(!dirName.empty()){
		dirName += L"\\";
		filePath = getImageName(dirName);
	}
	else{
		MessageBox(NULL , TEXT("フォルダが見つかりません。") , TEXT("エラー") , MB_OK | MB_ICONQUESTION );
		exit(0);
	}

#ifdef _DEBUG
	printf("pushNum :%d\n", pushNum);
#endif

	if(pushNum == 0){
		MessageBox(NULL , TEXT("対応する拡張子の画像が見つかりません。") , TEXT("エラー") , MB_OK | MB_ICONQUESTION );
		exit(0);
	}
}

#if 0

std::string SearchFile::getFilePath() //順番に表示
{
	static uint32_t loadNum = 0; //再生したデータ数を保存

	if(loadNum >= pushNum)loadNum = 0;
	return filePath[++loadNum];
}

#else

std::string SearchFile::getFilePath() //ランダムに表示
{
	static class Random random;

	return filePath[random.getIntRange(0, pushNum-1)];
}

#endif

std::vector<std::string> SearchFile::getImageName(std::wstring &dirName)
{
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	std::vector<std::string> fileNames;
	const std::wstring extension[EXTENSION_NUM] = {L"png", L"jpg", L"bmp"};
	std::wstring searchName;

	pushNum = 0; //念のため初期化

	for(int32_t i=0; i<EXTENSION_NUM; ++i){

		searchName = dirName;
		searchName += L"*.";
		searchName += extension[i];

		hFind = FindFirstFile(searchName.data(), &win32fd);

		if(hFind == INVALID_HANDLE_VALUE){
			continue;
		}
		do{
			if(win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				//ディレクトリは無視
			}
			else{
				char buffer[BUF_SIZE];

				WideCharToMultiByte(CP_OEMCP, 0, dirName.data(), -1, buffer, BUF_SIZE, NULL, NULL);
				std::string fullPath = buffer;
				WideCharToMultiByte(CP_OEMCP, 0, win32fd.cFileName, -1, buffer, BUF_SIZE, NULL, NULL);
				fullPath += buffer;
				fileNames.emplace_back(fullPath.c_str());
#ifdef _DEBUG
				printf("fullPath :%s\n", fileNames[pushNum].c_str());
#endif
				++pushNum;
			}
		}while(FindNextFile(hFind, &win32fd));

		FindClose(hFind);
	}

	return fileNames;
}

void SearchFile::getDirName(std::wstring &dirName)
{
	IFileDialog *pDialog = NULL;
	HRESULT hr;
	DWORD options;

	//インスタンス生成
	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDialog));
	if(FAILED(hr))return;

	//設定の初期化
	pDialog->GetOptions(&options);
	pDialog->SetOptions(options | FOS_PICKFOLDERS);

	//フォルダ選択ダイアログを表示
	hr = pDialog->Show(NULL);

	//結果取得
	if(SUCCEEDED(hr)){
		IShellItem *pItem = NULL;
		wchar_t *pPath = NULL;

		hr = pDialog->GetResult(&pItem);
		if(SUCCEEDED(hr)){
			hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pPath);
			if(SUCCEEDED(hr)){
				dirName = pPath;
				CoTaskMemFree(pPath); //pPathのメモリを開放する
			}
		}
	}
	pDialog->Release(); //ダイアログインスタンスの開放
}