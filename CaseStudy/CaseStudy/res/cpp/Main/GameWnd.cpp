//========================================================================================
//		File        : GameWnd.cpp
//		Program     : ゲーム用ウィンドウクラス
//
//		Description : ゲーム用ウィンドウクラスの実装
//
//		History     : 2014/04/17	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include "../../h/Main/GameWnd.h"
#include "../../h/System/System.h"

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
#ifdef __OPT_HIGH_TIMER
const double SET_FPS_TIME = 0.5;	// FPS描画時間
#else
const int SET_FPS_TIME = 500;		// FPS計算時間
#endif


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : ウィンドウを初期化する
//	Arguments   : None.
//	Returns     : 成否(true:成功)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CGameWindow::InitInstance()
{
	if (!CWindow::InitInstance()) return false;

	//m_bWindow = false;
	//if (IDYES == MessageBox(_T("ウィンドウモードで実行しますか？"), _T("画面モード"), MB_YESNO))
	m_bWindow = true;

	// タイマー処理の事前準備
#ifdef __OPT_HIGH_TIMER
	// 高解像度タイマー初期化
	m_pTimer = &CTimer::GetInstance();
#else
	::timeBeginPeriod(1);	// タイマの分解能を最小にセット
#endif

#ifdef __OPT_WINSOCK
	// WinSock通信初期化
	m_pWinSock = &CWinSock::GetInstance();
#endif

	// グラフィックデバイス初期化
	m_pGraph = CGraphics::Create(GetHwnd(), SCREEN_WIDTH, SCREEN_HEIGHT, m_bWindow);
	if (!m_pGraph) return false;
	
	// サウンドデバイス初期化
	m_pSound = &CSound::GetInstance();
	m_pSound->InitSound(GetHwnd());

	// 入出力デバイス初期化
	m_pInput = &CInput::GetInstance();
	m_pInput->InitInput(GetInstance(), GetHwnd());

	// シーン初期化
	m_pGameMain = CGameMain::Create(m_pGraph, m_pSound);
	if (!m_pGameMain) return false;

	// タイマー処理開始
#ifdef __OPT_HIGH_TIMER
	// 高解像度タイマーとして処理を開始(未対応の場合は内部で低解像度タイマーとして動作)
	m_pTimer->Start();
	m_dFPSLastTime = m_pTimer->GetTime();
#else
	m_dwFPSLastTime = ::timeGetTime();
	m_dwExecLastTime = m_dwFPSLastTime - (DWORD)FRAME_RATE;// 必ず最初に m_pGameMain->Update() を実行
	m_dwFrameCount = 0;
#endif

	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 終了処理
//	Description : ウィンドウを解放する
//	Arguments   : None.
//	Returns     : 実行ファイルの戻り値
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
int CGameWindow::ExitInstance()
{
	// シーン解放
	SAFE_RELEASE(m_pGameMain);

	// 入出力デバイス解放
	SAFE_RELEASE(m_pInput);

	// サウンドデバイス解放
	SAFE_RELEASE(m_pSound);

	// グラフィック デバイス解放
	SAFE_RELEASE(m_pGraph);

#ifdef __OPT_HIGH_TIMER
	// 高解像度タイマーの解放(現在は未処理)
	#ifdef __TIMER_SINGLETON
	#else
	//SAFE_RELEASE(m_pTimer);
	#endif
#else
	::timeEndPeriod(1);				// タイマの分解能を元に戻す
#endif

	return CWindow::ExitInstance();	// 実行ファイルの戻り値
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : アイドル処理
//	Description : アイドル時処理
//	Arguments   : lCount / アイドルカウンタ
//	Returns     : 成否(true:成功)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CGameWindow::OnIdle(long lCount)
{
	// フレーム処理
#ifdef __OPT_HIGH_TIMER
	// 経過フレーム数を取得
	int cnt = m_pTimer->Run();

	// 経過フレーム数分の処理を実行
	for(int i = 0; i < cnt; ++i)
	{
		if(m_pGameMain)
		{
			if(m_pTimer->GetTime() - m_dFPSLastTime >= SET_FPS_TIME)	// 0.5秒ごとに描画
			{
				m_pGameMain->SetFPS(m_pTimer->GetFps());	// フレーム数を取得・セット
				m_dFPSLastTime = m_pTimer->GetTime();		// タイマ値を更新
			}
		}
#else
	// この辺でフレーム数カウント
	DWORD dwCurrentTime = ::timeGetTime();		// 現在のタイマ値を取得
	if (dwCurrentTime - m_dwFPSLastTime >= SET_FPS_TIME) {	// 0.5 秒ごとに計測
		// フレーム数を計算
		if (m_pGameMain) {
			m_pGameMain->SetFPS(m_dwFrameCount * 1000 / (dwCurrentTime - m_dwFPSLastTime));
		}
		m_dwFPSLastTime = dwCurrentTime;	// タイマ値を更新
		m_dwFrameCount = 0;					// フレームカウンタをリセット
	}
	// この辺で時間管理
	while (dwCurrentTime - m_dwExecLastTime >= FRAME_RATE) {	// 一定時間が経過したら…
		m_dwExecLastTime += (DWORD)FRAME_RATE;					// タイマ値を更新
#endif

		if (m_pGameMain) {
#ifdef __OPT_WINSOCK
			m_pWinSock->Update();				// WinSock通信 更新
#endif
			m_pInput->UpdateInput(GetHwnd());	// 入力デバイス情報更新
			m_pGameMain->Update();				// ゲーム メイン処理
		}
		
#ifdef __OPT_HIGH_TIMER
#else
		m_dwFrameCount++;					// フレームカウント＋１
#endif
	}
	if (m_pGameMain) {
		m_pGameMain->Render();			// レンダリング処理
	}
	if(m_pGameMain->GetEndFlg())		// ゲーム終了
		OnClose();

	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : ハンドラ
//	Description : WM_KEYDOWN ハンドラ
//	Arguments   : nChar   / キーコード
//				  nRepCnt / リピートカウント
//				  nFlags  / フラグ
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameWindow::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// [Esc] が押されていたら
	//if (nChar == VK_ESCAPE) {
	//	// ウィンドウを閉じる
	//	OnClose();
	//}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : ハンドラ
//	Description : WM_ERASEBKGND ハンドラ
//	Arguments   : hdc / デバイスコンテキスト
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CGameWindow::OnEraseBkgnd(HDC hdc)
{
	// 背景消去しないで消去済みとして返す
	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : ハンドラ
//	Description : WM_SETCURSOR ハンドラ
//	Arguments   : hwnd      / ウィンドウハンドル
//				  nHitTest  / ヒットテストコード
//				  uMouseMsg / マウスメッセージID
//	Returns     : 成否(true:成功)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CGameWindow::OnSetCursor(HWND hwnd, UINT nHitTest, UINT uMouseMsg)
{
	// 全画面モードか、マウスカーソル座標がクライアント領域内なら
	if (!m_bWindow || nHitTest == HTCLIENT) {
		// マウスカーソル消去
//		::SetCursor(NULL);
		return true;
	}
	return false;
}


//========================================================================================
//	End of File
//========================================================================================