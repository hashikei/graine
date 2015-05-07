//========================================================================================
//		File        : Main.cpp
//		Program     : メイン処理
//
//		Description : ゲームのメイン処理
//
//		History     : 2013/12/18	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include "../../h/Main/GameWnd.h"
#include "../../h/System/System.h"

// ----- ライブラリのリンク
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
#define CAPTION_NAME	_T("CaseStudy")		// キャプション名


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : メイン
//	Description : メイン処理
//	Arguments   : hInstance / インスタンスハンドル
//				  hPrevInst / 直前のインスタンスハンドル
//				  lpCmdLine / コマンドライン文字列
//				  iCmdShow  / ウィンドウ状態
//	Returns     : リザルト
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int iCmdShow)
{
	// ウィンドウを作成
	int nResult = EXIT_FAILURE;
	CGameWindow* pWnd = new CGameWindow();
	if (pWnd->CreateFrame(CAPTION_NAME,				// ウィンドウ キャプション
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, 0,	// ウィンドウ スタイル，拡張スタイル
		CW_USEDEFAULT, CW_USEDEFAULT,				// ウィンドウ位置
		SCREEN_WIDTH, SCREEN_HEIGHT)) {				// クライアント領域サイズ
		pWnd->Show(iCmdShow);						// ウィンドウを表示
		pWnd->Update();								// ウィンドウ表示を更新
		nResult = pWnd->Run();						// メッセージ ループ
	}
	delete pWnd;
	return nResult;
}


//========================================================================================
//	End of File
//========================================================================================