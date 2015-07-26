//========================================================================================
//		File        : Button.h
//		Program     : ボタンベース
//
//		Description : ボタンベースの定義
//
//		History     : 2015/05/29	作成開始
//						   
//
//																Author : きんたまズ
//========================================================================================
//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include <math.h>
#include "../../h/Scene/GameMain.h"
#include "../../h/Object/GameStop.h"
#include "../../h/System/Input.h"
#include "../../h/System/Timer.h"
#include <tchar.h>

//――――――――――――――――――――――――――――――――――――――――――――
// using宣言
//――――――――――――――――――――――――――――――――――――――――――――
using namespace System;
using namespace Input;


//――――――――――――――――――――――――――――――――――――――――――――
// メンバ実体宣言
//――――――――――――――――――――――――――――――――――――――――――――
// ----- メンバ定数
// private:
const LPCTSTR CGameStop::TEX_FILENAME[MAX_TEX] = {
	_T("res/img/GameScene/Popup/WindowL.png"),		// ウィンドウテクスチャファイル名
	_T("res/img/GameScene/Popup/Close.png"),		// ボタンテクスチャファイル名
	_T("res/img/GameScene/Popup/Reset.png"),		// ボタンテクスチャファイル名
	_T("res/img/GameScene/Popup/Select.png"),		// ボタンテクスチャファイル名
	_T("res/img/GameScene/Popup/Pause.png"),		// テキストテクスチャファイル名
};

const D3DXVECTOR2 CGameStop::W_0_DEFAULET_SIZE		= D3DXVECTOR2(330,439);
const D3DXVECTOR3 CGameStop::W_0_DEFAULET_POS		= D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0);

const D3DXVECTOR2 CGameStop::B_0_DEFAULET_SIZE		= D3DXVECTOR2(200,73);
const D3DXVECTOR3 CGameStop::B_0_DEFAULET_POS		=  D3DXVECTOR3(SCREEN_WIDTH / 2 - 5.0f,
														SCREEN_HEIGHT / 2 + 45.0f,0);

const D3DXVECTOR2 CGameStop::TEXT_SIZE(399.0f * 0.7f, 109.0f * 0.7f);
const D3DXVECTOR3 CGameStop::TEXT_POS((float)SCREEN_WIDTH * 0.5f + 5.0f, 195.0f, 0.0f);

const float CGameStop::B_0_POS_INTERVAL_Y = 80;

const double CGameStop::SELECT_ANIME_TIME = 0.5;
const D3DXVECTOR3 CGameStop::SELECT_BUTTON_SCALE_L = D3DXVECTOR3(1.1f, 1.1f, 1.0f);
const D3DXVECTOR3 CGameStop::SELECT_BUTTON_SCALE_S = D3DXVECTOR3(0.95f, 0.95f, 1.0f);


//========================================================================================
// public:
//========================================================================================
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Description : コンストラクタ
//	Arguments   : 
//	Returns     : 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CGameStop::CGameStop()
{
	m_nPhase	= 0;
	m_nCurrent	= 0;
	m_nGo		= 0;

	m_pWnd				= NULL;
	m_pText				= NULL;

	m_pButtonGoGame		= NULL;
	m_pButtonReset		= NULL;
	m_pButtonGoSelect	= NULL;

	m_selectAnimeTimer	= 0.0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成
//	Description : オブジェクトを生成する
//	Arguments   : pszFName / 読み込みファイル名
//	Returns     : オブジェクトデータ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CGameStop* CGameStop::Create()
{
	// ----- 変数宣言
	CGameStop* pObj;

	// ----- 初期化処理
	pObj = new CGameStop();
	
	return pObj;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 一番最初初期化
//	Description : 
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameStop::Initialize()
{
		// ウィンドウ作成
	m_pWnd				= CObject2D::Create(TEX_FILENAME[TEX_WND_0]);
	m_pWnd->Init(W_0_DEFAULET_SIZE,D3DXVECTOR3(W_0_DEFAULET_POS.x,W_0_DEFAULET_POS.y,0));
	
	m_pText				= CObject2D::Create(TEX_FILENAME[TEX_TEXT]);
	m_pText->Init(TEXT_SIZE,TEXT_POS);
	
	// ボタン作成
	m_pButtonGoGame		= CButton::Create(TEX_FILENAME[TEX_CLOSE]);
	m_pButtonReset		= CButton::Create(TEX_FILENAME[TEX_RESET]);
	m_pButtonGoSelect	= CButton::Create(TEX_FILENAME[TEX_SELECT]);

	m_pButtonGoGame->Init(B_0_DEFAULET_SIZE,D3DXVECTOR3(B_0_DEFAULET_POS.x,B_0_DEFAULET_POS.y - B_0_POS_INTERVAL_Y,0));
	m_pButtonReset->Init(B_0_DEFAULET_SIZE,D3DXVECTOR3(B_0_DEFAULET_POS.x,B_0_DEFAULET_POS.y,0));
	m_pButtonGoSelect->Init(B_0_DEFAULET_SIZE,D3DXVECTOR3(B_0_DEFAULET_POS.x,B_0_DEFAULET_POS.y + B_0_POS_INTERVAL_Y,0));
	
	m_pButtonGoGame->SetName("GoGame");
	m_pButtonReset->SetName("Reset");
	m_pButtonGoSelect->SetName("Select");
		
	m_vecButton.push_back(m_pButtonGoGame);
	m_vecButton.push_back(m_pButtonReset);
	m_vecButton.push_back(m_pButtonGoSelect);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 解放処理
//	Description : 
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameStop::Release()
{
	if(m_pWnd){
		m_pWnd->Uninit();
		SAFE_RELEASE(m_pWnd)
	}

	// リスト内全部後始末
	for (unsigned int i = 0;i < m_vecButton.size(); ++i){
		m_vecButton[i]->Uninit();
		SAFE_RELEASE(m_vecButton[i])
	}

	m_vecButton.clear();		// オブジェクトリスト
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : いろんな初期化
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameStop::Init()
{
	m_nPhase = GAME_STOP_PHASE_INIT;
	m_nCurrent = GOGAME_BUTTON;
	m_nGo		= GO_GAME;

	// 最初の選択は「ゲームに戻る」
	m_vecButton[GOGAME_BUTTON]->SetPhase(B_PHASE_CHOICE);

	m_selectAnimeTimer = 0.0;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : 
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameStop::Uninit()
{
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : いろんな更新
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameStop::Update()
{
	switch(m_nPhase)
	{
	case GAME_STOP_PHASE_INIT:
		// ----- BGM再生
		CGameMain::PlayBGM(BGM_PAUSE, DSBPLAY_LOOPING);
		m_nPhase = GAME_STOP_PHASE_WAIT;
		break;
	case GAME_STOP_PHASE_WAIT:
		Wait();
		break;
	case GAME_STOP_PHASE_ENTER:
		Enter();
		break;
	case GAME_STOP_PHASE_UNINIT:
		m_nPhase++;
	case GAME_STOP_PHASE_END:
		break;
	}
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : いろんな描画
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameStop::Draw()
{
	m_pWnd->DrawScreenAlpha();
	m_pText->DrawScreenAlpha();

	for(unsigned int i = 0;i < m_vecButton.size();i++){
		m_vecButton[i]->DrawScreenAlpha();
	}
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 決定待ち
//	Description : 
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameStop::Wait()
{
	// 選択　左右キー
	if(GetTrgKey(DIK_DOWN)){
		CGameMain::PlaySE(SE_CHOICE);
		if(m_nCurrent < MAX_BUTTON - 1) {
			m_nCurrent++;
			m_selectAnimeTimer = 0.0;
		}
	}
	if(GetTrgKey(DIK_UP)){
		CGameMain::PlaySE(SE_CHOICE);
		if(m_nCurrent > 0) {
			m_nCurrent--;
			m_selectAnimeTimer = 0.0;
		}
	}

	// 現在選択されてるボタン
	for(unsigned int i = 0;i < m_vecButton.size();i++){

		if(i == m_nCurrent)
			m_vecButton[i]->SetPhase(B_PHASE_CHOICE);
		else
			m_vecButton[i]->SetPhase(B_PHASE_WAIT);

		if(m_vecButton[i]->GetPhase() == B_PHASE_CHOICE){
//			m_vecButton[i]->SetColor(D3DXVECTOR3(255,255,0));
			if(CTimer::GetTime() - m_selectAnimeTimer > SELECT_ANIME_TIME) {
				m_selectAnimeTimer = CTimer::GetTime();

				if(m_vecButton[i]->GetScale().x >= SELECT_BUTTON_SCALE_L.x) {
					m_vecButton[i]->Scale(SELECT_BUTTON_SCALE_S);
				} else {
					m_vecButton[i]->Scale(SELECT_BUTTON_SCALE_L);
				}
			}
		}else{
//			m_vecButton[i]->SetColor(D3DXVECTOR3(255,255,255));
			m_vecButton[i]->Scale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		}
	}

	// 決定
	if(GetTrgKey(DIK_RETURN)){
		CGameMain::PlaySE(SE_ENTER);
		m_vecButton[m_nCurrent]->SetPhase(B_PHASE_ENTER);
		m_nPhase = GAME_STOP_PHASE_ENTER;
	}

	if(GetTrgKey(DIK_RSHIFT)){
		CGameMain::PlaySE(SE_ENTER);
		m_vecButton[0]->SetPhase(B_PHASE_ENTER);
		m_nPhase = GAME_STOP_PHASE_ENTER;
	}
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 決定
//	Description : 
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameStop::Enter()
{
	for(unsigned int i = 0;i < m_vecButton.size();i++){
		if(m_vecButton[i]->GetPhase() == B_PHASE_ENTER){
			switch(i)
			{
			case GOGAME_BUTTON:
				m_nGo = GO_GAME;
				CGameMain::PlayBGM(BGM_GAME, DSBPLAY_LOOPING);
				break;
			case RESET_BUTTON:
				m_nGo = GO_RESET;
				break;
			case GOSELECT_BUTTON:
				m_nGo = GO_SELECT;
				break;
			}
		}
	}
	
	CGameMain::StopBGM(BGM_PAUSE);
	
	m_nPhase++;
}
//========================================================================================
//	End of File
//========================================================================================