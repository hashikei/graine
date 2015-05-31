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
#include "../../h/Object/GameStop.h"
#include "../../h/System/Input.h"
#include <tchar.h>

//――――――――――――――――――――――――――――――――――――――――――――
// using宣言
//――――――――――――――――――――――――――――――――――――――――――――
using namespace System;
using namespace Input;


// ----- メンバ定数
// private:
const LPCTSTR CGameStop::TEX_FILENAME[MAX_TEX] = {
	_T("res/img/GameScene/Object/block.png"),		// ウィンドウテクスチャファイル名
	_T("res/img/GameScene/Object/block.png"),		// ボタンテクスチャファイル名
};

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

	m_pButtonGoGame		= NULL;
	m_pButtonReset		= NULL;
	m_pButtonGoSelect	= NULL;

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
	m_pWnd				= CTexture::Create(TEX_FILENAME[TEX_WND_0]);
	m_pWnd->Init(W_0_DEFAULET_SIZE,D3DXVECTOR3(W_0_DEFAULET_POS.x,W_0_DEFAULET_POS.y,0));
	
	// ボタン作成
	m_pButtonGoGame		= CButton::Create(TEX_FILENAME[TEX_BUTTON_0]);
	m_pButtonReset		= CButton::Create(TEX_FILENAME[TEX_BUTTON_0]);
	m_pButtonGoSelect	= CButton::Create(TEX_FILENAME[TEX_BUTTON_0]);

	m_pButtonGoGame->Init(B_0_DEFAULET_SIZE,D3DXVECTOR3(B_0_DEFAULET_POS.x - B_0_POS_INTERVAL_X,B_0_DEFAULET_POS.y,0));
	m_pButtonReset->Init(B_0_DEFAULET_SIZE,D3DXVECTOR3(B_0_DEFAULET_POS.x,B_0_DEFAULET_POS.y,0));
	m_pButtonGoSelect->Init(B_0_DEFAULET_SIZE,D3DXVECTOR3(B_0_DEFAULET_POS.x + B_0_POS_INTERVAL_X,B_0_DEFAULET_POS.y,0));
	
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

	// デバッグ用
	m_pWnd->SetColor(D3DXVECTOR3(128,128,128));
	m_pWnd->SetAlpha(190);

	for(unsigned int i = 0;i < m_vecButton.size();i++)
		m_vecButton[i]->SetAlpha(190);
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
	m_pWnd->DrawAlpha();

	for(unsigned int i = 0;i < m_vecButton.size();i++){
		m_vecButton[i]->Draw();
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
	if(GetTrgKey(DIK_RIGHT)){
		if(m_nCurrent < MAX_BUTTON - 1)
			m_nCurrent++;
	}
	if(GetTrgKey(DIK_LEFT)){
		if(m_nCurrent > 0)
			m_nCurrent--;
	}

	// 現在選択されてるボタン
	for(unsigned int i = 0;i < m_vecButton.size();i++){

		if(i == m_nCurrent)
			m_vecButton[i]->SetPhase(B_PHASE_CHOICE);
		else
			m_vecButton[i]->SetPhase(B_PHASE_WAIT);

		if(m_vecButton[i]->GetPhase() == B_PHASE_CHOICE){
			m_vecButton[i]->SetColor(D3DXVECTOR3(255,255,0));
		}else{
			m_vecButton[i]->SetColor(D3DXVECTOR3(255,255,255));
		}
	}

	// 決定
	if(GetTrgKey(DIK_RETURN)){
		m_vecButton[m_nCurrent]->SetPhase(B_PHASE_ENTER);
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
	
	m_nPhase++;
}
//========================================================================================
//	End of File
//========================================================================================