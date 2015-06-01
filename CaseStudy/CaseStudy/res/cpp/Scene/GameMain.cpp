//========================================================================================
//		File        : GameMain.cpp
//		Program     : ゲームのメインクラス
//
//		Description : ゲームのメインクラスの実装
//
//		History     : 2014/04/17	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#
#include "../../h/System/PreInclude.h"
#
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "../../h/System/System.h"
#include "../../h/System/Input.h"
#include "../../h/Scene/GameMain.h"
#include "../../h/Scene/Title.h"
#include "../../h/Scene/Select.h"
#include "../../h/Scene/Game.h"
#include "../../h/Scene/Result.h"

//――――――――――――――――――――――――――――――――――――――――――――
// using宣言
//――――――――――――――――――――――――――――――――――――――――――――
using namespace Input;

//――――――――――――――――――――――――――――――――――――――――――――
// メンバ実体宣言
//――――――――――――――――――――――――――――――――――――――――――――
CGraphics*	CGameMain::m_pGraph;		// グラフィックデバイス
CSound*		CGameMain::m_pSound;		// サウンドデバイス

TCHAR	CGameMain::m_szDebug[1024];		// デバッグ用文字列
double	CGameMain::m_fps;				// フレーム数カウント用

CScene* CGameMain::m_sceneList[MAX_SCENEID];	// 全シーンのリスト(SCENE_IDにて種別)
bool	CGameMain::m_bEnd = false;				// ゲーム終了フラグ

LPDIRECTSOUNDBUFFER8 CGameMain::m_pBGM[MAX_BGMID];	// BGMリスト
LPDIRECTSOUNDBUFFER8 CGameMain::m_pSE[MAX_SEID][CSound::MAX_DUP];	// SEリスト
int		CGameMain::m_lastPlaySE[MAX_SEID];		// 直近に再生したSE番号

CScene* CGameMain::m_pScene;			// 現在のシーン
int		CGameMain::m_curSceneID;		// 現在のシーンID

CMapData*	CGameMain::m_pMapData;		// マップデータ


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CGameMain::CGameMain()
{
	m_pGraph		= NULL;
	m_pSound		= NULL;
	m_szDebug[0]	= _T('\0');
	m_fps			= 0.0;

	for(int nCntBGM = 0; nCntBGM < MAX_BGMID; ++nCntBGM)
		m_pBGM[nCntBGM] = NULL;
	for(int nCntSE = 0; nCntSE < MAX_SEID; ++nCntSE) {
		for(int nCntDup = 0; nCntDup < CSound::MAX_DUP; ++nCntDup) {
			m_pSE[nCntSE][nCntDup] = NULL;
		}
		m_lastPlaySE[nCntSE] = 0;
	}

	m_pScene		= NULL;
	m_curSceneID	= MAX_SCENEID;

	m_pMapData		= NULL;

// ----- デバッグ用コンソール生成
#ifdef _DEBUG
	if(AllocConsole() == 0)
	{
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CWinSock::コンソールの生成に失敗しました。"), _T("error"), MB_OK);
#endif
	}
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
#endif
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CGameMain::~CGameMain()
{
// ----- デバッグ用コンソール破棄
#ifdef _DEBUG
	if(FreeConsole() == 0)
	{
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CWinSock::コンソールの破棄に失敗しました。"), _T("error"), MB_OK);
#endif
	}
#endif
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成処理
//	Description : ゲームメインクラスを生成する
//	Arguments   : pGraph / グラフィッククラス
//				  pSound / サウンドクラス
//	Returns     : ゲームメインクラス
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CGameMain* CGameMain::Create(CGraphics* pGraph, CSound* pSound)
{
	CGameMain* pGameMain = new CGameMain();
	if (pGameMain) {
		if (!pGameMain->Initialize(pGraph, pSound)) {
			SAFE_DELETE(pGameMain);
		}
	}
	return pGameMain;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 解放処理
//	Description : ゲームメインクラスを解放する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameMain::Release()
{
	Finalize();
	delete this;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画処理
//	Description : ゲームを描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameMain::Render()
{
	if (m_pGraph->Begin()) {	// 描画開始

		Draw();				// 描画処理

		m_pGraph->End();		// 描画終了
	}
	m_pGraph->SwapBuffer();		// バックバッファ入替
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新処理
//	Description : ゲームメイン処理
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameMain::Update()
{
	// ----- シーン更新
	m_pScene->Update();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 状態切り替え
//	Description : 現在のゲームステートを任意に切り替える
//	Arguments   : id / ステートID
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameMain::SetScene(int id)
{
	// ----- 次のシーンへ
	if(m_pScene)
	{
		m_pScene->Uninit();
	}
	if(id >= 0 && id < MAX_SCENEID)
	{
		m_pScene = m_sceneList[id];
		m_pScene->Init();
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : BGM再生
//	Description : 任意のBGMを再生する
//	Arguments   : id       / BGMのID
//				  loop     / ループフラグ(0:ループなし, 1:ループあり)
//				  priority / 優先度
//	Returns     : 処理結果
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
HRESULT CGameMain::PlayBGM(int id, int loop, int priority)
{
	m_pBGM[id]->SetCurrentPosition(0);
	return m_pBGM[id]->Play(0, priority, loop);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : SE再生
//	Description : 任意のSEを再生する
//	Arguments   : id       / SEのID
//				  loop     / ループフラグ(0:ループなし, 1:ループあり)
//				  priority / 優先度
//	Returns     : 処理結果
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
HRESULT CGameMain::PlaySE(int id, int loop, int priority)
{
	// ----- 変数宣言
	DWORD status;				// SE再生状態
	LPDIRECTSOUNDBUFFER8* ppSE;	// SEのバッファリスト

	// ----- 初期化処理
	status	= 0;
	ppSE	= m_pSE[id];	// SEリストを取得(同時発音するSEのバッファリスト)

	// ----- 事前処理
	ppSE[0]->GetStatus(&status);	// SEの再生状態を取得

	// ----- 再生処理
	if(status & DSBSTATUS_PLAYING)
	{
		// 再生可能かチェック
		for(int i = 1; i < CSound::MAX_DUP; ++i)
		{
			// 再生中なら未処理
			ppSE[i]->GetStatus(&status);
			if(status & DSBSTATUS_PLAYING)
				continue;

			// 再生処理
			ppSE[i]->SetCurrentPosition(0);				// 再生位置を初期化
			m_lastPlaySE[id] = i;						// 再生SE番号を保存
			return ppSE[i]->Play(0, priority, loop);	// 再生
		}

		// 一番古く再生したSEを停止し、新しく再生する
		int playNum = m_lastPlaySE[id] + 1;
		if(playNum >= CSound::MAX_DUP)
			playNum = 0;
		ppSE[playNum]->Stop();							// SEを停止
		ppSE[playNum]->SetCurrentPosition(0);			// 再生位置を初期化
		m_lastPlaySE[id] = playNum;						// 再生SE番号を保存
		return ppSE[playNum]->Play(0, priority, loop);	// 再生
	}
	ppSE[0]->SetCurrentPosition(0);				// 再生位置を初期化
	m_lastPlaySE[id] = 0;						// 再生SE番号を保存
	return ppSE[0]->Play(0, priority, loop);	// 再生
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : BGM停止
//	Description : 任意のBGMを停止する
//	Arguments   : id / BGMのID
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameMain::StopBGM(int id)
{
	m_pBGM[id]->Stop();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : SE停止
//	Description : 全てのSEを停止する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameMain::StopAllSE()
{
	for(int nCntSE = 0; nCntSE < MAX_SEID; ++nCntSE) {
		for(int nCntDup = 0; nCntDup < CSound::MAX_DUP; ++nCntDup) {
			m_pSE[nCntSE][nCntDup]->Stop();
		}
	}
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : デバイス、リソースを初期化する
//	Arguments   : pGraph / グラフィッククラス
//				  pSound / サウンドクラス
//	Returns     : 成否(true:成功)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CGameMain::Initialize(CGraphics* pGraph, CSound* pSound)
{
	// ----- システム初期化
	srand(timeBeginPeriod(1));	// 乱数の種を設定

	m_pGraph = pGraph;
	m_pSound = pSound;
	
	// ----- シーン初期化
	m_sceneList[SID_TITLE]	= CTitle::Create();		// タイトル
	m_sceneList[SID_SELECT]	= CSelect::Create();	// 楽曲選択
	m_sceneList[SID_GAME]	= CGame::Create();		// ゲーム本編
	m_sceneList[SID_RESULT]	= CResult::Create();	// リザルト

	// ----- BGM初期化
	for(int nCntBGM = 0; nCntBGM < MAX_BGMID; ++nCntBGM)
		m_pBGM[nCntBGM] = m_pSound->LoadSound(BGM_FILENAME[nCntBGM]);	// BGM読み込み

	// ----- SE初期化
	for(int nCntSE = 0; nCntSE < MAX_SEID; ++nCntSE)
	{
		m_pSE[nCntSE][0] = m_pSound->LoadSound(SE_FILENAME[nCntSE]);	// SE読み込み
		m_lastPlaySE[nCntSE] = -1;		// 直近に再生したSE番号を初期化
	}
	m_pSound->CopyBuffer(m_pSE, MAX_SEID);		// セカンダリバッファをコピー

	// ----- マップデータ準備
	m_pMapData = &CMapData::GetInstance();

	// ----- 最初のシーンをセット
//	SetScene(SID_TITLE);	// タイトルへ
	SetScene(SID_GAME);	// タイトルへ

	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : デバイス、リソースを解放する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameMain::Finalize()
{
	// ----- ゲーム用オブジェクトの後始末
	for(int nCntSE = MAX_SEID - 1; nCntSE >= 0; --nCntSE)
	{
		for(int nCntDup = CSound::MAX_DUP - 1; nCntDup >= 0; --nCntDup)
		{
			SAFE_RELEASE(m_pSE[nCntSE][nCntDup]);		// SE解放
		}
	}
	for(int nCntBGM = MAX_BGMID - 1; nCntBGM >= 0; --nCntBGM)
		SAFE_RELEASE(m_pBGM[nCntBGM]);		// BGM解放
	for(int nCntScene = MAX_SCENEID - 1; nCntScene >= 0; --nCntScene)
		SAFE_RELEASE(m_sceneList[nCntScene]);		// シーンテーブル解放
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画処理
//	Description : ゲームを描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameMain::Draw()
{
#ifdef _DEBUG
	m_szDebug[0] = _T('\0');	// デバッグ文字列初期化

	// FPS を画面に描画するための文字列を作成
	TCHAR	str[256];
	_stprintf(str, _T("FPS = %.2f\n"), m_fps);
	lstrcat(m_szDebug, str);
#endif
	
	// ----- シーン描画
	m_pScene->Draw();
	
#ifdef _DEBUG
	// デバッグ文字列描画
	m_pGraph->DrawText(0, 0, m_szDebug);
#endif
}


//========================================================================================
//	End of File
//========================================================================================