//========================================================================================
//		File        : Select.cpp
//		Program     : 楽曲選択クラス
//
//		Description : 楽曲選択クラスの定義
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
#include <tchar.h>
#include "../../h/System/System.h"
#include "../../h/System/Input.h"
#include "../../h/Scene/GameMain.h"
#include "../../h/Scene/Select.h"

//――――――――――――――――――――――――――――――――――――――――――――
// using宣言
//――――――――――――――――――――――――――――――――――――――――――――
using namespace Input;
using namespace System;

//――――――――――――――――――――――――――――――――――――――――――――
// メンバ実体宣言
//――――――――――――――――――――――――――――――――――――――――――――
// ----- メンバ定数
// private:
const LPCTSTR CSelect::TEX_FILENAME[MAX_TEXLIST] =	// テクスチャファイル名
{
	_T("res/img/BG.jpg"),			// 背景テクスチャファイル名
	_T("res/img/Fade.jpg"),			// フェード用テクスチャファイル名
	_T("res/img/GameScene/Object/player_0.png"),
	_T("res/img/SelectArrow.png"),
	_T("res/img/SelectStage1.png"),
	_T("res/img/SelectStage2.png"),
	_T("res/img/SelectVisual.png"),
};
const D3DXVECTOR3 CSelect::INIT_CAMERA_EYE(0, 0, -1000);	// カメラの初期視点
const D3DXVECTOR3 CSelect::INIT_CAMERA_LOOK(0, 0, 0);		// カメラの初期注視点
const D3DXVECTOR3 CSelect::INIT_TEXTURE_POS[MAX_TEXLIST] =	// テクスチャの初期位置
{
	D3DXVECTOR3(0.0f, 0.0f, FAR_CLIP),	// 背景
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// フィルター
};

// ----- フェード関連
const int CSelect::FADEIN_TIME	= 10;	// フェードイン間隔(アルファ値:0～255)
const int CSelect::FADEOUT_TIME	= 10;	// フェードアウト間隔(アルファ値:0～255)


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CSelect::CSelect()
{
	m_pCamera		= NULL;
	m_pFilter		= NULL;

	m_phase			= MAX_PHASE;
	
	for (int i = 0; i < MAX_OBJECTLIST; i++)
		m_pSelectPlayer[MAX_OBJECTLIST] = NULL;

	m_nStatus		= S_STATUS_WAIT;
	m_nStage		= S_STAGE_1;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CSelect::~CSelect()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : 楽曲選択を初期化する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CSelect::Init(void)
{
	// ----- カメラ初期化
	m_pCamera->Init();
	D3DXVECTOR3 eye = INIT_CAMERA_EYE;
	D3DXVECTOR3 look = INIT_CAMERA_LOOK;
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	m_pCamera->SetParameter(eye, look, up);

	// ----- テクスチャ初期化
	m_pBG->Init(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT), INIT_TEXTURE_POS[TL_BG]);			// 背景
	m_pFilter->Init(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT), INIT_TEXTURE_POS[TL_FADE]);		// フィルター

	m_pSelectPlayer[OL_PLAYER]->Init(D3DXVECTOR2(192, 192), D3DXVECTOR3((float)PLAYER_INIT_POS_X, (float)PLAYER_INIT_POS_Y, 0));
	m_pSelectPlayer[OL_ARROW_LEFT]->Init(D3DXVECTOR2(192, 192), D3DXVECTOR3((float)LEFT_ARROW_INIT_POS_X, (float)LEFT_ARROW_INIT_POS_Y, 0));
	m_pSelectPlayer[OL_ARROW_RIGHT]->Init(D3DXVECTOR2(192, 192), D3DXVECTOR3((float)RIGHT_ARROW_INIT_POS_X, (float)RIGHT_ARROW_INIT_POS_Y, 0));
	m_pSelectPlayer[OL_ROGO1]->Init(D3DXVECTOR2(384, 192), D3DXVECTOR3((float)ROGO_INIT_POS_X, (float)ROGO_INIT_POS_Y, 0));
	m_pSelectPlayer[OL_ROGO2]->Init(D3DXVECTOR2(384, 192), D3DXVECTOR3((float)ROGO_SCREEN_OUT_POS_X, (float)ROGO_SCREEN_OUT_POS_Y, 0));
	m_pSelectPlayer[OL_STAGE]->Init(D3DXVECTOR2(SCREEN_RIGHT * 2, SCREEN_RIGHT * 2), D3DXVECTOR3((float)STAGE_INIT_POS_X, (float)STAGE_INIT_POS_Y, 0));

	// ----- BGM再生
//	CGameMain::PlayBGM(BGM_SELECT, DSBPLAY_LOOPING);

	// ----- 次のフェーズへ
	m_phase = PHASE_FADEIN;		// フェードイン開始
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : 楽曲選択の終了処理をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CSelect::Uninit(void)
{
	// ----- オブジェクト後始末
	m_pCamera->Uninit();		// カメラ

	// ----- テクスチャ後始末
	m_pBG->Uninit();			// 背景
	m_pFilter->Uninit();	// フィルター
	
	// ----- BGM停止
//	CGameMain::StopBGM(BGM_SELECT);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : 楽曲選択を更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CSelect::Update(void)
{
	// ----- オブジェクト更新
	m_pCamera->Update();		// カメラ
			
	switch(m_phase)
	{
		// フェードイン
		case PHASE_FADEIN:
			if(m_pFilter->FadeOutAlpha(FADEIN_TIME)) {
				m_phase = PHASE_MAIN;		// 楽曲選択本編開始
			}
			break;

		// 次のシーンへフェードアウト
		case PHASE_FADEOUT:
			if(m_pFilter->FadeInAlpha(FADEOUT_TIME)) {
				Uninit();						// 後始末
				CGameMain::SetScene(SID_GAME);	// ゲーム本編へ
			}
			break;

		// 楽曲選択本編
		case PHASE_MAIN:
			Main();
			break;
			
		default:
			break;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : 楽曲選択画面を描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CSelect::Draw(void)
{
	// ----- カメラ描画
	m_pCamera->Draw();

	// ----- テクスチャ描画
	m_pBG->DrawAlpha();		// 背景

	switch(m_phase)
	{
		// フェードイン・アウト
		case PHASE_FADEIN:
		case PHASE_FADEOUT:
			m_pFilter->DrawAlpha();
			break;

		// 楽曲選択本編
		case PHASE_MAIN:
			break;
			
		default:
			break;
	}
	for (int i = 0; i < MAX_OBJECTLIST; i++)
		m_pSelectPlayer[i]->DrawAlpha();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成処理
//	Description : 楽曲選択データを生成する
//	Arguments   : None.
//	Returns     : 楽曲選択データ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CSelect* CSelect::Create()
{
	// ----- 変数宣言
	CSelect* pSelect;

	// ----- 初期化処理
	pSelect = new CSelect();
	if(pSelect)
	{
		if(!pSelect->Initialize())
		{
			SAFE_DELETE(pSelect);
		}
	}

	return pSelect;
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : 楽曲選択を初期化する
//	Arguments   : None.
//	Returns     : 成否(true:成功)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CSelect::Initialize()
{
	// ----- カメラ生成
	m_pCamera = CCamera::Create();
	if(m_pCamera == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CSelect::Cameraの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}

	// ----- テクスチャ生成
	// 背景
	m_pBG = CTexture::Create(TEX_FILENAME[TL_BG]);
	if(m_pBG == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CSelect::BGの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}
	// フェード用フィルター
	m_pFilter = CTexture::Create(TEX_FILENAME[TL_FADE]);
	if(m_pFilter == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CSelect::Filterの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}

	m_pSelectPlayer[OL_PLAYER] = CSelectObject::Create(TEX_FILENAME[TL_PLAYER]);
	m_pSelectPlayer[OL_ARROW_LEFT] = CSelectObject::Create(TEX_FILENAME[TL_ARROW]);
	m_pSelectPlayer[OL_ARROW_RIGHT] = CSelectObject::Create(TEX_FILENAME[TL_ARROW]);
	m_pSelectPlayer[OL_ROGO1] = CSelectObject::Create(TEX_FILENAME[TL_ROGO1]);
	m_pSelectPlayer[OL_ROGO2] = CSelectObject::Create(TEX_FILENAME[TL_ROGO2]);
	m_pSelectPlayer[OL_STAGE] = CSelectObject::Create(TEX_FILENAME[TL_STAGE]);

	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : 楽曲選択の終了処理をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CSelect::Finalize(void)
{
	// ----- テクスチャ解放
	SAFE_RELEASE(m_pFilter);	// フィルター
	SAFE_RELEASE(m_pBG);		// 背景

	// ----- カメラ解放
	SAFE_RELEASE(m_pCamera)
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : メイン
//	Description : 楽曲選択のメイン処理
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CSelect::Main()
{
	if (GetTrgKey(DIK_LEFT))		// 左
	{
		m_nStatus = S_STATUS_LEFT;
		if (m_nStage == S_STAGE_1)
			m_nStage = S_STAGE_2;
		else
			m_nStage = S_STAGE_1;
	}
	if (GetTrgKey(DIK_RIGHT))		// 右
	{
		m_nStatus = S_STATUS_RIGHT;
		if (m_nStage == S_STAGE_1)
			m_nStage = S_STAGE_2;
		else
			m_nStage = S_STAGE_1;
	}

	m_nStatus = m_pSelectPlayer[OL_PLAYER]->PlayerUpdate(m_nStatus);
	m_pSelectPlayer[OL_ROGO1]->RogoUpdate(1,m_nStatus,m_nStage);
	m_pSelectPlayer[OL_ROGO2]->RogoUpdate(2,m_nStatus,m_nStage);
	m_nStatus = m_pSelectPlayer[OL_STAGE]->StageUpdate(m_nStatus,m_nStage);
	m_nStatus = m_pSelectPlayer[OL_ARROW_LEFT]->ArrowUpdate(1,m_nStatus);
	m_nStatus = m_pSelectPlayer[OL_ARROW_RIGHT]->ArrowUpdate(2,m_nStatus);

	// ----- 次のシーンへ
	if(GetTrgKey(DIK_RETURN)) {
		m_phase = PHASE_FADEOUT;		// 次のシーンへフェードアウト開始
	}
}


//========================================================================================
//	End of File
//========================================================================================