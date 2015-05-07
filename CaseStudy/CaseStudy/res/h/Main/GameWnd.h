//========================================================================================
//		File        : GameWnd.h
//		Program     : ゲーム用ウィンドウクラス
//
//		Description : ゲーム用ウィンドウクラスの定義
//
//		History     : 2014/04/17	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================

#pragma once

//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../System/PreInclude.h"
#
#include "../Main/Window.h"
#ifdef __OPT_HIGH_TIMER
	#include "../System/Timer.h"
#endif
#include "../System/Graphics.h"
#include "../System/Input.h"
#include "../System/Sound.h"
#include "../Scene/GameMain.h"

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CGameWindow : public CWindow
{
// ===== メンバ変数
private:
#ifdef __OPT_WINSOCK
	CWinSock*	m_pWinSock;			// WinSock通信 クラス
#endif

#ifdef __OPT_HIGH_TIMER
	CTimer*		m_pTimer;			// タイマー処理クラス
	double		m_dFPSLastTime;		// FPS描画用
#else
	DWORD		m_dwExecLastTime;	// FPS計測用
	DWORD		m_dwFPSLastTime;	// FPS描画用
	DWORD		m_dwFrameCount;		// フレームカウンタ
#endif

	CGraphics*	m_pGraph;			// 描画デバイス クラス
	CInput*		m_pInput;			// 入出力デバイス クラス
	CSound*		m_pSound;			// サウンドデバイス クラス
	CGameMain*	m_pGameMain;		// ゲームメイン

	bool		m_bWindow;			// ウィンドウ / フルスクリーン
	
// ===== メンバ関数
private:
	bool InitInstance();
	int ExitInstance();
	bool OnIdle(long lCount);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	bool OnEraseBkgnd(HDC hdc);
	bool OnSetCursor(HWND hwnd, UINT nHitTest, UINT uMouseMsg);
};


//========================================================================================
//	End of File
//========================================================================================