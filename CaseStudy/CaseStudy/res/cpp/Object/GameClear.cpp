//========================================================================================
//		File        : ゲームクリアウィンドウ
//		Program     : ゲームクリアウィンドウ
//
//		Description : ゲームクリアウィンドウ定義
//
//		History     : 2015/06/22	作成開始
//						   
//
//																Author : Kei Hashimoto
//========================================================================================
//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include <math.h>
#include "../../h/System/Graphics.h"
#include "../../h/System/Input.h"
#include "../../h/System/ChangeScene.h"
#include "../../h/Object/GameClear.h"
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
const LPCTSTR CGameClear::TEX_FILENAME[MAX_TEX] = {
	_T("res/img/GameScene/Object/block.png"),		// ウィンドウテクスチャファイル名
	_T("res/img/GameScene/Object/block.png"),		// ボタンテクスチャファイル名
};

const D3DXVECTOR2 CGameClear::W_0_DEFAULET_SIZE		= D3DXVECTOR2(512,256);
const D3DXVECTOR3 CGameClear::W_0_DEFAULET_POS		= D3DXVECTOR3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.0f);

const float CGameClear::B_0_POS_INTERVAL_X = 150;

const D3DXVECTOR2 CGameClear::B_0_DEFAULET_SIZE		= D3DXVECTOR2(128,64);
const D3DXVECTOR3 CGameClear::B_0_DEFAULET_POS		=  D3DXVECTOR3(SCREEN_WIDTH / 2 - B_0_POS_INTERVAL_X,
														SCREEN_HEIGHT / 2 + 60,0);

const D3DXVECTOR2 CGameClear::B_1_DEFAULET_SIZE		= D3DXVECTOR2(128,64);
const D3DXVECTOR3 CGameClear::B_1_DEFAULET_POS		=  D3DXVECTOR3(SCREEN_WIDTH / 2 + B_0_POS_INTERVAL_X,
														SCREEN_HEIGHT / 2 + 60,0);

const D3DXVECTOR3 CGameClear::DIRECTION_CAMERA_SPD	= D3DXVECTOR3(0.0f, 0.0f, 10.0f);	// 演出時のカメラ移動速度

//========================================================================================
// public:
//========================================================================================
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Description : コンストラクタ
//	Arguments   : 
//	Returns     : 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CGameClear::CGameClear()
{
	m_nPhase		= 0;
	m_nCurButton	= 0;
	m_nGo			= 0;

	m_pWnd				= NULL;

	m_pButtonNext		= NULL;
	m_pButtonGoSelect	= NULL;

	m_pCamera			= NULL;
	m_cameraStartPos	= D3DXVECTOR2(0.0f, 0.0f);
	m_dirDist			= 0.0f;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成
//	Description : オブジェクトを生成する
//	Arguments   : pszFName / 読み込みファイル名
//	Returns     : オブジェクトデータ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CGameClear* CGameClear::Create()
{
	// ----- 変数宣言
	CGameClear* pObj;

	// ----- 初期化処理
	pObj = new CGameClear();
	
	return pObj;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 一番最初初期化
//	Description : 
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameClear::Initialize()
{
		// ウィンドウ作成
	m_pWnd				= CObject2D::Create(TEX_FILENAME[TEX_WND_0]);
	m_pWnd->Init(W_0_DEFAULET_SIZE,W_0_DEFAULET_POS);
	
	// ボタン作成
	m_pButtonNext		= CButton::Create(TEX_FILENAME[TEX_BUTTON_0]);
	m_pButtonGoSelect	= CButton::Create(TEX_FILENAME[TEX_BUTTON_0]);

	m_pButtonNext->Init(B_0_DEFAULET_SIZE,B_0_DEFAULET_POS);
	m_pButtonGoSelect->Init(B_0_DEFAULET_SIZE,B_1_DEFAULET_POS);
	
	m_pButtonNext->SetName("Next");
	m_pButtonGoSelect->SetName("Select");
		
	m_vecButton.push_back(m_pButtonNext);
	m_vecButton.push_back(m_pButtonGoSelect);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 解放処理
//	Description : 
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameClear::Release()
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
void CGameClear::Init()
{
	m_nPhase = PHASE_INIT;
	m_nCurButton = NEXT_BUTTON;
	m_nGo		= GO_NEXT;

	// 最初の選択は「ゲームに戻る」
	m_vecButton[NEXT_BUTTON]->SetPhase(B_PHASE_CHOICE);

	// デバッグ用
	m_pWnd->SetColor(D3DXVECTOR3(128,128,128));
	m_pWnd->SetAlpha(190);

	for(unsigned int i = 0;i < m_vecButton.size();i++)
		m_vecButton[i]->SetAlpha(190);
	
	m_pCamera			= NULL;
	m_cameraStartPos	= D3DXVECTOR2(0.0f, 0.0f);
	m_dirDist			= 0.0f;

	m_nPhase = PHASE_INIT_DIRECTION;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : 
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameClear::Uninit()
{
	// ----- グラフィックス設定を初期化
	CGraphics::SetDraw2D();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : いろんな更新
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameClear::Update()
{
	switch(m_nPhase)
	{
	case PHASE_INIT:
	case PHASE_FADEIN_DIRECTION:
		break;
	case PHASE_INIT_DIRECTION:
		InitDirection();
		break;
	case PHASE_DIRECTION:
		Direction();
		break;
	case PHASE_UNINIT_DIRECTION:
		UninitDirection();
		break;
	case PHASE_WAIT:
		Wait();
		break;
	case PHASE_ENTER:
		Enter();
		break;
	case PHASE_UNINIT:
		m_nPhase = PHASE_END;
		break;
	case PHASE_END:
		break;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : いろんな描画
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameClear::Draw()
{
	switch(m_nPhase)
	{
	case PHASE_INIT:
	case PHASE_INIT_DIRECTION:
	case PHASE_DIRECTION:
	case PHASE_UNINIT_DIRECTION:
		break;

	case PHASE_FADEIN_DIRECTION:
		FadeinDirection();
		break;

	case PHASE_WAIT:
	case PHASE_ENTER:
		m_pWnd->DrawScreenAlpha();

		for(unsigned int i = 0;i < m_vecButton.size();i++){
			m_vecButton[i]->DrawScreen();
		}
		break;

	case PHASE_UNINIT:
		m_nPhase = PHASE_END;
		break;

	case PHASE_END:
		break;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 演出準備
//	Description : ゲームクリア演出の初期化処理
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CGameClear::InitDirection()
{
	if(!m_pCamera)
		return false;

	// ----- カメラの移動処理準備
	CGraphics::SetDraw3D();
	m_pCamera->SetEyeX(m_cameraStartPos.x);
	m_pCamera->SetEyeY(m_cameraStartPos.y);

	// ----- フェードイン準備
	D3DXVECTOR3 fadePos(m_cameraStartPos.x, m_cameraStartPos.y, 0.0f);
	CChangeScene::SetNormalFadePos(fadePos);
	CChangeScene::SetNormalFadeAlpha(255);

	m_nPhase = PHASE_FADEIN_DIRECTION;
	
	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 演出処理フェードイン
//	Description : 演出処理に入るためのフェードイン処理
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameClear::FadeinDirection()
{
	if(CChangeScene::NormalFadeOut(0.0f, 1))
		m_nPhase = PHASE_DIRECTION;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 演出処理
//	Description : カメラが画面を引いたり等、演出を行う
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameClear::Direction()
{
	// ----- ステージを俯瞰する
	float z = m_pCamera->GetEye().z;
	z -= DIRECTION_CAMERA_SPD.z;
	m_pCamera->SetEyeZ(z);

	// ----- 俯瞰完了(演出終了)
	if(m_dirDist <= tanf(D3DXToRadian(FOVY)) * -z)
		m_nPhase = PHASE_UNINIT_DIRECTION;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 演出事後処理
//	Description : ゲームクリア演出の事後処理
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameClear::UninitDirection()
{
	m_nPhase = PHASE_WAIT;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 決定待ち
//	Description : 
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameClear::Wait()
{
	// 選択　左右キー
	if(GetTrgKey(DIK_RIGHT)){
		if(m_nCurButton < MAX_BUTTON - 1)
			m_nCurButton++;
	}
	if(GetTrgKey(DIK_LEFT)){
		if(m_nCurButton > 0)
			m_nCurButton--;
	}

	// 現在選択されてるボタン
	for(unsigned int i = 0;i < m_vecButton.size();i++){

		if(i == m_nCurButton)
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
		m_vecButton[m_nCurButton]->SetPhase(B_PHASE_ENTER);
		m_nPhase = PHASE_ENTER;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 決定
//	Description : 
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameClear::Enter()
{
	for(unsigned int i = 0;i < m_vecButton.size();i++){
		if(m_vecButton[i]->GetPhase() == B_PHASE_ENTER){
			switch(i)
			{
			case NEXT_BUTTON:
				m_nGo = GO_NEXT;
				break;
			case GOSELECT_BUTTON:
				m_nGo = GO_SELECT;
				break;
			}
		}
	}
	
	m_nPhase = PHASE_UNINIT;
}

//========================================================================================
//	End of File
//========================================================================================