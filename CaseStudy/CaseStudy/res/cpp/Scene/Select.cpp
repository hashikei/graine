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
#include "../../h/System/ChangeScene.h"
#include "../../h/Scene/GameMain.h"
#include "../../h/Scene/Select.h"
#include "../../h/Scene/Game.h"

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
	_T("res/img/Select/BG/saisyo_haikei.png"),			// 背景テクスチャファイル名
	_T("res/img/Select/BG/saisyo_haikei_2.png"),			// 背景テクスチャファイル名
	_T("res/img/Select/Object/gamen_nasi_0_100.png"),
	_T("res/img/Select/Object/gamen_nasi_2_100.png"),
	_T("res/img/GameScene/Object/Tactile_1.png"),
	_T("res/img/GameScene/Object/player_0.png"),
	_T("res/img/Select/Object/yajirusi.png"),
	_T("res/img/Select/Object/stage1.png"),
	_T("res/img/Select/Object/stage2.png"),
	_T("res/img/Select/Object/stage3.png"),
	_T("res/img/Select/Object/stage4.png"),
	_T("res/img/Select/Object/stage5.png"),
	_T("res/img/Select/Object/serekuto_1.png"),
	_T("res/img/Select/Object/serekuto_2.png"),
	_T("res/img/Select/Object/serekuto_3.png"),
};
const D3DXVECTOR3 CSelect::INIT_CAMERA_EYE(0, 0, -1000);	// カメラの初期視点
const D3DXVECTOR3 CSelect::INIT_CAMERA_LOOK(0, 0, 0);		// カメラの初期注視点
const D3DXVECTOR3 CSelect::INIT_TEXTURE_POS[MAX_TEXLIST] =	// テクスチャの初期位置
{
	D3DXVECTOR3(0.0f, 0.0f, FAR_CLIP),	// 背景
};

// ----- フェード関連
const float CSelect::FADE_POSZ = -100.0f;	// フェード用テクスチャのZ座標
const int CSelect::FADEIN_TIME = 10;		// フェードイン間隔(アルファ値:0～255)
const int CSelect::FADEOUT_TIME = 10;		// フェードアウト間隔(アルファ値:0～255)


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CSelect::CSelect()
{
	m_pCamera = NULL;

	m_phase = MAX_PHASE;

	for (int i = 0; i < MAX_OBJECTLIST; i++)
		m_pSelectPlayer[i] = NULL;

	m_nClear = SC_CLEAR_CLEAR;
	m_nStatus = S_STATUS_WAIT;
	m_nStage = CMapData::ID_STAGE1;
	m_bEffect = false;

//	for (int i = 0; i < 5; i++)
//		m_bClear[i] = false;
	m_bClear[CMapData::ID_STAGE1] = false;
	m_bClear[CMapData::ID_STAGE2] = true;
	m_bClear[CMapData::ID_STAGE3] = false;
	m_bClear[CMapData::ID_STAGE4] = false;
	m_bClear[CMapData::ID_STAGE5] = false;

	m_bAnime = false;
	m_bBG1Anime = false;
	m_bBG2Anime = false;

	m_fRotationZ = 0.0f;
	m_nCount = 0;
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
	// アニメーション初期化
	m_pSelectPlayer[OL_TACTILE]->StartAnimation();
	m_pSelectPlayer[OL_TACTILE]->UVDivision(0, SELECT_ANIME_SIZE_X, SELECT_ANIME_SIZE_Y);
	m_pSelectPlayer[OL_TACTILE]->Init(D3DXVECTOR2(192, 192),D3DXVECTOR3(PLAYER_INIT_POS_X, PLAYER_INIT_POS_Y, 0));
	m_pSelectPlayer[OL_TACTILE]->SetColor(D3DXVECTOR3(128, 255, 128));
	m_pSelectPlayer[OL_TACTILE]->TactileUpdate(true, m_nStatus);

	m_pSelectPlayer[OL_PLAYER]->StartAnimation();
	m_pSelectPlayer[OL_PLAYER]->UVDivision(0, SELECT_ANIME_SIZE_X, SELECT_ANIME_SIZE_Y);
	m_pSelectPlayer[OL_PLAYER]->Init(D3DXVECTOR2(192, 192), D3DXVECTOR3((float)PLAYER_INIT_POS_X, (float)PLAYER_INIT_POS_Y, 0));
	m_pSelectPlayer[OL_PLAYER]->SetColor(D3DXVECTOR3(128, 255, 128));
	m_pSelectPlayer[OL_PLAYER]->PlayerUpdate(true, m_nStatus);

	// ----- カメラ初期化
	m_pCamera->Init();
	D3DXVECTOR3 eye = INIT_CAMERA_EYE;
	D3DXVECTOR3 look = INIT_CAMERA_LOOK;
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	m_pCamera->SetParameter(eye, look, up);

	if (m_bClear[m_nStage])
	{
		m_pSelectPlayer[OL_BG_1]->Init(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT), D3DXVECTOR3((float)SCREEN_RIGHT, SCREEN_HEIGHT / 2, 0));			// 背景
		m_pSelectPlayer[OL_BG_2]->Init(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT), D3DXVECTOR3((float)SCREEN_WIDTH + SCREEN_RIGHT, SCREEN_HEIGHT / 2, 0));			// 背景
		m_pSelectPlayer[OL_STAGE_1]->Init(D3DXVECTOR2(SCREEN_RIGHT * 2, SCREEN_RIGHT * 2), D3DXVECTOR3((float)STAGE_INIT_POS_X, (float)STAGE_INIT_POS_Y, 0));
		m_pSelectPlayer[OL_STAGE_3]->Init(D3DXVECTOR2(SCREEN_RIGHT * 2, SCREEN_RIGHT * 2), D3DXVECTOR3((float)SCREEN_WIDTH + SCREEN_RIGHT, (float)STAGE_INIT_POS_Y, 0));
		m_pSelectPlayer[OL_RING_TRUE]->Init(D3DXVECTOR2(1280, 1280), D3DXVECTOR3((float)SCREEN_RIGHT, (float)SCREEN_HEIGHT / 2 + 100, 0));
		m_pSelectPlayer[OL_RING_FALSE]->Init(D3DXVECTOR2(1280, 1280), D3DXVECTOR3((float)SCREEN_WIDTH * 2, (float)SCREEN_HEIGHT / 2 + 100, 0));
	}
	else
	{
		m_pSelectPlayer[OL_BG_2]->Init(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT), D3DXVECTOR3((float)SCREEN_RIGHT, SCREEN_HEIGHT / 2, 0));			// 背景
		m_pSelectPlayer[OL_BG_1]->Init(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT), D3DXVECTOR3((float)SCREEN_WIDTH + SCREEN_RIGHT, SCREEN_HEIGHT / 2, 0));			// 背景
		m_pSelectPlayer[OL_STAGE_1]->Init(D3DXVECTOR2(SCREEN_RIGHT * 2, SCREEN_RIGHT * 2), D3DXVECTOR3((float)SCREEN_WIDTH + SCREEN_RIGHT, (float)STAGE_INIT_POS_Y, 0));
		m_pSelectPlayer[OL_STAGE_3]->Init(D3DXVECTOR2(SCREEN_RIGHT * 2, SCREEN_RIGHT * 2), D3DXVECTOR3((float)STAGE_INIT_POS_X, (float)STAGE_INIT_POS_Y, 0));
		m_pSelectPlayer[OL_RING_TRUE]->Init(D3DXVECTOR2(1280, 1280), D3DXVECTOR3((float)SCREEN_WIDTH * 2, (float)SCREEN_HEIGHT / 2 + 100, 0));
		m_pSelectPlayer[OL_RING_FALSE]->Init(D3DXVECTOR2(1280, 1280), D3DXVECTOR3((float)SCREEN_RIGHT, (float)SCREEN_HEIGHT / 2 + 100, 0));
	}
	m_pSelectPlayer[OL_ARROW_LEFT]->Init(D3DXVECTOR2(100, 130), D3DXVECTOR3((float)LEFT_ARROW_INIT_POS_X, (float)LEFT_ARROW_INIT_POS_Y, 0));
	m_pSelectPlayer[OL_ARROW_RIGHT]->Init(D3DXVECTOR2(100, 130), D3DXVECTOR3((float)RIGHT_ARROW_INIT_POS_X, (float)RIGHT_ARROW_INIT_POS_Y, 0));
	m_pSelectPlayer[OL_ARROW_RIGHT]->ScaleX(-1.0f);
	switch (m_nStage)
	{
	case 0:
		m_pSelectPlayer[OL_ROGO_1]->Init(D3DXVECTOR2(473, 145), D3DXVECTOR3((float)ROGO_INIT_POS_X, (float)ROGO_INIT_POS_Y, 0));
		for (int i = OL_ROGO_2; i <= OL_ROGO_5; i++)
			m_pSelectPlayer[i]->Init(D3DXVECTOR2(473, 145), D3DXVECTOR3((float)ROGO_SCREEN_OUT_POS_X, (float)ROGO_SCREEN_OUT_POS_Y, 0));
		break;
	case 1:
		m_pSelectPlayer[OL_ROGO_2]->Init(D3DXVECTOR2(473, 145), D3DXVECTOR3((float)ROGO_INIT_POS_X, (float)ROGO_INIT_POS_Y, 0));
		m_pSelectPlayer[OL_ROGO_1]->Init(D3DXVECTOR2(473, 145), D3DXVECTOR3((float)ROGO_SCREEN_OUT_POS_X, (float)ROGO_SCREEN_OUT_POS_Y, 0));
		for (int i = OL_ROGO_3; i <= OL_ROGO_5; i++)
			m_pSelectPlayer[i]->Init(D3DXVECTOR2(473, 145), D3DXVECTOR3((float)ROGO_SCREEN_OUT_POS_X, (float)ROGO_SCREEN_OUT_POS_Y, 0));
		break;
	case 2:
		m_pSelectPlayer[OL_ROGO_3]->Init(D3DXVECTOR2(473, 145), D3DXVECTOR3((float)ROGO_INIT_POS_X, (float)ROGO_INIT_POS_Y, 0));
		for (int i = OL_ROGO_1; i < OL_ROGO_3; i++)
			m_pSelectPlayer[i]->Init(D3DXVECTOR2(473, 145), D3DXVECTOR3((float)ROGO_SCREEN_OUT_POS_X, (float)ROGO_SCREEN_OUT_POS_Y, 0));
		for (int i = OL_ROGO_4; i <= OL_ROGO_5; i++)
			m_pSelectPlayer[i]->Init(D3DXVECTOR2(473, 145), D3DXVECTOR3((float)ROGO_SCREEN_OUT_POS_X, (float)ROGO_SCREEN_OUT_POS_Y, 0));
		break;
	case 3:
		m_pSelectPlayer[OL_ROGO_4]->Init(D3DXVECTOR2(473, 145), D3DXVECTOR3((float)ROGO_INIT_POS_X, (float)ROGO_INIT_POS_Y, 0));
		for (int i = OL_ROGO_1; i < OL_ROGO_4; i++)
			m_pSelectPlayer[i]->Init(D3DXVECTOR2(473, 145), D3DXVECTOR3((float)ROGO_SCREEN_OUT_POS_X, (float)ROGO_SCREEN_OUT_POS_Y, 0));
		m_pSelectPlayer[OL_ROGO_5]->Init(D3DXVECTOR2(473, 145), D3DXVECTOR3((float)ROGO_SCREEN_OUT_POS_X, (float)ROGO_SCREEN_OUT_POS_Y, 0));
		break;
	case 4:
		m_pSelectPlayer[OL_ROGO_5]->Init(D3DXVECTOR2(473, 145), D3DXVECTOR3((float)ROGO_INIT_POS_X, (float)ROGO_INIT_POS_Y, 0));
		for (int i = OL_ROGO_1; i < OL_ROGO_5; i++)
			m_pSelectPlayer[i]->Init(D3DXVECTOR2(473, 145), D3DXVECTOR3((float)ROGO_SCREEN_OUT_POS_X, (float)ROGO_SCREEN_OUT_POS_Y, 0));
		break;
	}
	m_pSelectPlayer[OL_STAGE_2]->Init(D3DXVECTOR2(SCREEN_RIGHT * 2, SCREEN_RIGHT * 2), D3DXVECTOR3((float)SCREEN_WIDTH + SCREEN_RIGHT, (float)STAGE_INIT_POS_Y, 0));

	m_pSelectPlayer[OL_STAGE_1]->ClearSave(m_bClear[CMapData::ID_STAGE1],m_bClear[CMapData::ID_STAGE2],m_bClear[CMapData::ID_STAGE3],m_bClear[CMapData::ID_STAGE4],m_bClear[CMapData::ID_STAGE5]);
	m_pSelectPlayer[OL_STAGE_2]->ClearSave(m_bClear[CMapData::ID_STAGE1], m_bClear[CMapData::ID_STAGE2], m_bClear[CMapData::ID_STAGE3], m_bClear[CMapData::ID_STAGE4], m_bClear[CMapData::ID_STAGE5]);
	m_pSelectPlayer[OL_STAGE_3]->ClearSave(m_bClear[CMapData::ID_STAGE1], m_bClear[CMapData::ID_STAGE2], m_bClear[CMapData::ID_STAGE3], m_bClear[CMapData::ID_STAGE4], m_bClear[CMapData::ID_STAGE5]);
	m_pSelectPlayer[OL_RING_TRUE]->SetAlpha(90);
	m_pSelectPlayer[OL_RING_FALSE]->SetAlpha(90);
	// ----- フェード設定
	CChangeScene::SetNormalFadeAlpha(255);

	// ----- BGM再生
	CGameMain::PlayBGM(BGM_SELECT, DSBPLAY_LOOPING);

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

	for (int i = 0; i < MAX_OBJECTLIST; i++)
		m_pSelectPlayer[i]->Uninit();

	// ----- テクスチャ後始末
//	m_pBG->Uninit();			// 背景

	// ----- BGM停止
	CGameMain::StopBGM(BGM_SELECT);
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

	switch (m_phase)
	{
		// フェードイン
	case PHASE_FADEIN:
		if (CChangeScene::NormalFadeOut(FADE_POSZ, FADEIN_TIME)) {
			m_phase = PHASE_MAIN;		// 楽曲選択本編開始
		}
		break;

		// 次のシーンへフェードアウト
	case PHASE_FADEOUT:
		if (CChangeScene::NormalFadeIn(FADE_POSZ, FADEOUT_TIME)) {
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
	//m_pBG->DrawScreenAlpha();		// 背景
	for (int i = 0; i < MAX_OBJECTLIST; i++)
		m_pSelectPlayer[i]->DrawScreenAlpha();


	switch (m_phase)
	{
		// フェードイン・アウト
	case PHASE_FADEIN:
	case PHASE_FADEOUT:
		CChangeScene::DrawNormalFade();
		break;

		// 楽曲選択本編
	case PHASE_MAIN:
		m_pSelectPlayer[OL_TACTILE]->SetColor(D3DXVECTOR3(128, 255, 128));
		m_pSelectPlayer[OL_PLAYER]->SetColor(D3DXVECTOR3(128, 255, 128));
		break;

	default:
		break;
	}
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
	if (pSelect)
	{
		if (!pSelect->Initialize())
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
	if (m_pCamera == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CSelect::Cameraの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}

	// ----- テクスチャ生成
	// 背景
/*
	m_pBG = CObject2D::Create(TEX_FILENAME[TL_BG]);
	if (m_pBG == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CSelect::BGの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}
*/
	m_pSelectPlayer[OL_BG_2] = CSelectObject::Create(TEX_FILENAME[TL_BG_2]);
	m_pSelectPlayer[OL_BG_1] = CSelectObject::Create(TEX_FILENAME[TL_BG_1]);
	m_pSelectPlayer[OL_TACTILE] = CSelectObject::Create(TEX_FILENAME[TL_TACTILE]);
	m_pSelectPlayer[OL_PLAYER] = CSelectObject::Create(TEX_FILENAME[TL_PLAYER]);
	m_pSelectPlayer[OL_ARROW_LEFT] = CSelectObject::Create(TEX_FILENAME[TL_ARROW]);
	m_pSelectPlayer[OL_ARROW_RIGHT] = CSelectObject::Create(TEX_FILENAME[TL_ARROW]);
	m_pSelectPlayer[OL_ROGO_1] = CSelectObject::Create(TEX_FILENAME[TL_ROGO_1]);
	m_pSelectPlayer[OL_ROGO_2] = CSelectObject::Create(TEX_FILENAME[TL_ROGO_2]);
	m_pSelectPlayer[OL_ROGO_3] = CSelectObject::Create(TEX_FILENAME[TL_ROGO_3]);
	m_pSelectPlayer[OL_ROGO_4] = CSelectObject::Create(TEX_FILENAME[TL_ROGO_4]);
	m_pSelectPlayer[OL_ROGO_5] = CSelectObject::Create(TEX_FILENAME[TL_ROGO_5]);
	m_pSelectPlayer[OL_STAGE_1] = CSelectObject::Create(TEX_FILENAME[TL_STAGE_1]);
	m_pSelectPlayer[OL_STAGE_2] = CSelectObject::Create(TEX_FILENAME[TL_STAGE_2]);
	m_pSelectPlayer[OL_STAGE_3] = CSelectObject::Create(TEX_FILENAME[TL_STAGE_3]);
	m_pSelectPlayer[OL_RING_TRUE] = CSelectObject::Create(TEX_FILENAME[TL_RING_TRUE]);
	m_pSelectPlayer[OL_RING_FALSE] = CSelectObject::Create(TEX_FILENAME[TL_RING_FALSE]);

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
//	SAFE_RELEASE(m_pBG);		// 背景

	// ----- カメラ解放
	SAFE_RELEASE(m_pCamera);

	for (int i = 0; i < MAX_OBJECTLIST; i++)
		SAFE_RELEASE(m_pSelectPlayer[i]);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : メイン
//	Description : 楽曲選択のメイン処理
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CSelect::Main()
{
	if (m_bAnime)
	{
		if (GetTrgKey(DIK_LEFT))		// 左
		{
			CGameMain::PlaySE(SE_CHOICE);
			m_nStatus = S_STATUS_LEFT;
			switch (m_nStage)
			{
			case CMapData::ID_STAGE1:
				if (m_bClear[m_nStage])
				{
					if (m_bClear[CMapData::ID_STAGE5])
						m_nClear = SC_CLEAR_CLEAR;
					else
						m_nClear = SC_CLEAR_FALSE;
				}
				else
				{
					if (m_bClear[CMapData::ID_STAGE5])
						m_nClear = SC_FALSE_CLEAR;
					else
						m_nClear = SC_FALSE_FALSE;
				}

				m_nStage = CMapData::ID_STAGE5;
				break;
			case CMapData::ID_STAGE2:
				if (m_bClear[m_nStage])
				{
					if (m_bClear[CMapData::ID_STAGE1])
						m_nClear = SC_CLEAR_CLEAR;
					else
						m_nClear = SC_CLEAR_FALSE;
				}
				else
				{
					if (m_bClear[CMapData::ID_STAGE1])
						m_nClear = SC_FALSE_CLEAR;
					else
						m_nClear = SC_FALSE_FALSE;
				}

				m_nStage = CMapData::ID_STAGE1;
				break;
			case CMapData::ID_STAGE3:
				if (m_bClear[m_nStage])
				{
					if (m_bClear[CMapData::ID_STAGE2])
						m_nClear = SC_CLEAR_CLEAR;
					else
						m_nClear = SC_CLEAR_FALSE;
				}
				else
				{
					if (m_bClear[CMapData::ID_STAGE2])
						m_nClear = SC_FALSE_CLEAR;
					else
						m_nClear = SC_FALSE_FALSE;
				}

				m_nStage = CMapData::ID_STAGE2;
				break;
			case CMapData::ID_STAGE4:
				if (m_bClear[m_nStage])
				{
					if (m_bClear[CMapData::ID_STAGE3])
						m_nClear = SC_CLEAR_CLEAR;
					else
						m_nClear = SC_CLEAR_FALSE;
				}
				else
				{
					if (m_bClear[CMapData::ID_STAGE3])
						m_nClear = SC_FALSE_CLEAR;
					else
						m_nClear = SC_FALSE_FALSE;
				}

				m_nStage = CMapData::ID_STAGE3;
				break;
			case CMapData::ID_STAGE5:
				if (m_bClear[m_nStage])
				{
					if (m_bClear[CMapData::ID_STAGE4])
						m_nClear = SC_CLEAR_CLEAR;
					else
						m_nClear = SC_CLEAR_FALSE;
				}
				else
				{
					if (m_bClear[CMapData::ID_STAGE4])
						m_nClear = SC_FALSE_CLEAR;
					else
						m_nClear = SC_FALSE_FALSE;
				}

				m_nStage = CMapData::ID_STAGE4;
				break;
			}
		}
		if (GetTrgKey(DIK_RIGHT))		// 右
		{
			CGameMain::PlaySE(SE_CHOICE);
			m_nStatus = S_STATUS_RIGHT;
			switch (m_nStage)
			{
			case CMapData::ID_STAGE1:
				if (m_bClear[m_nStage])
				{
					if (m_bClear[CMapData::ID_STAGE2])
						m_nClear = SC_CLEAR_CLEAR;
					else
						m_nClear = SC_CLEAR_FALSE;
				}
				else
				{
					if (m_bClear[CMapData::ID_STAGE2])
						m_nClear = SC_FALSE_CLEAR;
					else
						m_nClear = SC_FALSE_FALSE;
				}

				m_nStage = CMapData::ID_STAGE2;
				break;
			case CMapData::ID_STAGE2:
				if (m_bClear[m_nStage])
				{
					if (m_bClear[CMapData::ID_STAGE3])
						m_nClear = SC_CLEAR_CLEAR;
					else
						m_nClear = SC_CLEAR_FALSE;
				}
				else
				{
					if (m_bClear[CMapData::ID_STAGE3])
						m_nClear = SC_FALSE_CLEAR;
					else
						m_nClear = SC_FALSE_FALSE;
				}

				m_nStage = CMapData::ID_STAGE3;
				break;
			case CMapData::ID_STAGE3:
				if (m_bClear[m_nStage])
				{
					if (m_bClear[CMapData::ID_STAGE4])
						m_nClear = SC_CLEAR_CLEAR;
					else
						m_nClear = SC_CLEAR_FALSE;
				}
				else
				{
					if (m_bClear[CMapData::ID_STAGE4])
						m_nClear = SC_FALSE_CLEAR;
					else
						m_nClear = SC_FALSE_FALSE;
				}

				m_nStage = CMapData::ID_STAGE4;
				break;
			case CMapData::ID_STAGE4:
				if (m_bClear[m_nStage])
				{
					if (m_bClear[CMapData::ID_STAGE5])
						m_nClear = SC_CLEAR_CLEAR;
					else
						m_nClear = SC_CLEAR_FALSE;
				}
				else
				{
					if (m_bClear[CMapData::ID_STAGE5])
						m_nClear = SC_FALSE_CLEAR;
					else
						m_nClear = SC_FALSE_FALSE;
				}

				m_nStage = CMapData::ID_STAGE5;
				break;
			case CMapData::ID_STAGE5:
				if (m_bClear[m_nStage])
				{
					if (m_bClear[CMapData::ID_STAGE1])
						m_nClear = SC_CLEAR_CLEAR;
					else
						m_nClear = SC_CLEAR_FALSE;
				}
				else
				{
					if (m_bClear[CMapData::ID_STAGE1])
						m_nClear = SC_FALSE_CLEAR;
					else
						m_nClear = SC_FALSE_FALSE;
				}

				m_nStage = CMapData::ID_STAGE1;
				break;
			}
		}
	}

	m_bBG2Anime = m_pSelectPlayer[OL_BG_2]->BGUpdate(2, m_nStatus, m_bClear[m_nStage], m_bBG2Anime);
	m_bBG1Anime = m_pSelectPlayer[OL_BG_1]->BGUpdate(1, m_nStatus, m_bClear[m_nStage], m_bBG1Anime);
	m_bAnime = m_pSelectPlayer[OL_STAGE_1]->StageUpdate(1,m_nStatus, m_nStage);
	m_bAnime = m_pSelectPlayer[OL_STAGE_2]->StageUpdate(2,m_nStatus, m_nStage);
	m_bAnime = m_pSelectPlayer[OL_STAGE_3]->StageUpdate(3,m_nStatus, m_nStage);
	m_pSelectPlayer[OL_RING_TRUE]->RingUpdate(1,m_nStatus,m_bClear[m_nStage]);
	m_pSelectPlayer[OL_RING_FALSE]->RingUpdate(2,m_nStatus,m_bClear[m_nStage]);
	m_pSelectPlayer[OL_TACTILE]->TactileUpdate(m_bAnime, m_nStatus);
	m_pSelectPlayer[OL_PLAYER]->PlayerUpdate(m_bAnime, m_nStatus);
	m_pSelectPlayer[OL_ROGO_1]->RogoUpdate(1, m_nStatus, m_nStage);
	m_pSelectPlayer[OL_ROGO_2]->RogoUpdate(2, m_nStatus, m_nStage);
	m_pSelectPlayer[OL_ROGO_3]->RogoUpdate(3, m_nStatus, m_nStage);
	m_pSelectPlayer[OL_ROGO_4]->RogoUpdate(4, m_nStatus, m_nStage);
	m_pSelectPlayer[OL_ROGO_5]->RogoUpdate(5, m_nStatus, m_nStage);
	m_nStatus = m_pSelectPlayer[OL_ARROW_LEFT]->ArrowUpdate(1, m_nStatus);
	m_nStatus = m_pSelectPlayer[OL_ARROW_RIGHT]->ArrowUpdate(2, m_nStatus);

	// ----- 次のシーンへ
	if (GetTrgKey(DIK_RETURN)) {
		CGameMain::PlaySE(SE_ENTER);
		CGame::SetStageID(m_nStage);
		m_phase = PHASE_FADEOUT;		// 次のシーンへフェードアウト開始
	}
}
//========================================================================================
//	End of File
//========================================================================================