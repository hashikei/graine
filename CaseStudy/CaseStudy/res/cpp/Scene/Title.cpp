//========================================================================================
//		File        : Title.cpp
//		Program     : タイトルクラス
//
//		Description : タイトルクラスの定義
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
#include "../../h/Scene/Title.h"

//――――――――――――――――――――――――――――――――――――――――――――
// using宣言
//――――――――――――――――――――――――――――――――――――――――――――
using namespace Input;
using namespace System;

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
// ----- リソース情報
const LPCTSTR CTitle::TEX_FILENAME[MAX_TEXLIST] =	// テクスチャファイル名
{
	_T("res/img/BG.jpg"),			// 背景テクスチャファイル名
	_T("res/img/Fade.jpg"),			// フェード用テクスチャファイル名
};
const D3DXVECTOR3 CTitle::INIT_CAMERA_EYE(0, 0, -1000);		// カメラの初期視点
const D3DXVECTOR3 CTitle::INIT_CAMERA_LOOK(0, 0, 0);		// カメラの初期注視点
const D3DXVECTOR3 CTitle::INIT_TEXTURE_POS[MAX_TEXLIST] = {	// テクスチャの初期位置
	D3DXVECTOR3(0.0f, 0.0f, FAR_CLIP),	// 背景
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// フィルター
};

// ----- フェード関連
const int CTitle::FADEIN_TIME	= 10;	// フェードイン間隔(アルファ値:0～255)
const int CTitle::FADEOUT_TIME	= 10;	// フェードアウト間隔(アルファ値:0～255)


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CTitle::CTitle()
{
	m_pCamera	= NULL;
	m_pBG		= NULL;
	m_pFilter	= NULL;

	m_phase		= MAX_PHASE;



	m_pPlayer	= NULL;
	m_pEnemy	= NULL;

	m_pFieldObj.reserve(CMapData::INIT_OBJECT_NUM);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CTitle::~CTitle()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : タイトルを初期化する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTitle::Init(void)
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

	// ----- BGM再生
//	CGameMain::PlayBGM(BGM_TITLE, DSBPLAY_LOOPING);

	// ----- 次のフェーズへ
	m_phase = PHASE_FADEIN;		// フェードイン開始



	m_pPlayer->Init(D3DXVECTOR2(128, 1), D3DXVECTOR3(-256, 0, 0));
	m_pEnemy->Init(D3DXVECTOR2(128, 128), D3DXVECTOR3(256, 0, 0));

	CMapData::LoadData(CMapData::ID_STAGE1);	// マップデータ読み込み
	CMapData::GetFieldObjList(&m_pFieldObj);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : タイトルの終了処理をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTitle::Uninit(void)
{
	// ----- フェードアウト開始
	if(m_phase != PHASE_FADEOUT)
		m_phase = PHASE_FADEOUT;

	// ----- テクスチャ後始末
	m_pBG->Uninit();			// 背景
	m_pFilter->Uninit();	// フィルター

	// ----- BGM停止
//	CGameMain::StopBGM(BGM_TITLE);




	m_pPlayer->Uninit();
	m_pEnemy->Uninit();

	for(LPFIELDOBJECT_ARRAY_IT it = m_pFieldObj.begin(); it != m_pFieldObj.end(); ++it) {
		(*it)->Uninit();
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : タイトルを更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTitle::Update(void)
{
	// ----- オブジェクト更新
	m_pCamera->Update();		// カメラ
	
	switch(m_phase)
	{
		// フェードイン
		case PHASE_FADEIN:
			if(m_pFilter->FadeOutAlpha(FADEIN_TIME)) {
//			if(ChangeScene::SideSliderIn(m_pFilter, 3.0f)) {
//				m_pFilter->SetPosX(-m_pFilter->GetWidth());		// フェードアウト用準備
				m_phase = PHASE_MAIN;							// タイトル本編開始
			}
			break;

		// フェードアウト
		case PHASE_FADEOUT:
			if(m_pFilter->FadeInAlpha(FADEOUT_TIME))
//			if(ChangeScene::SideSliderOut(m_pFilter, 3.0f))
			{	// 次のシーンへ
				Uninit();							// 後始末
				CGameMain::SetScene(SID_SELECT);	// 楽曲選択画面へ
			}
			break;

		// タイトル本編
		case PHASE_MAIN:
			Main();
			break;
			
		default:
			break;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : タイトル画面を描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTitle::Draw(void)
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

		// タイトル本編
		case PHASE_MAIN:
			break;
			
		default:
			break;
	}




	m_pPlayer->DrawAlpha();
	m_pEnemy->DrawAlpha();

	for(LPFIELDOBJECT_ARRAY_IT it = m_pFieldObj.begin(); it != m_pFieldObj.end(); ++it) {
  		(*it)->DrawAlpha();
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成処理
//	Description : タイトルデータを生成する
//	Arguments   : None.
//	Returns     : タイトルデータ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CTitle* CTitle::Create()
{
	// ----- 変数宣言
	CTitle* pTitle;

	// ----- 初期化処理
	pTitle = new CTitle();
	if(pTitle)
	{
		if(!pTitle->Initialize())
		{
			SAFE_DELETE(pTitle);
		}
	}

	return pTitle;
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : タイトルを初期化する
//	Arguments   : None.
//	Returns     : 成否(true:成功)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CTitle::Initialize()
{
	// カメラ生成
	m_pCamera = CCamera::Create();
	if(m_pCamera == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CTitle::Cameraの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}

	// ----- テクスチャ生成
	// 背景
	m_pBG = CTexture::Create(TEX_FILENAME[TL_BG]);
	if(m_pBG == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CTitle::BGの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}
	// フェード用フィルター
	m_pFilter = CTexture::Create(TEX_FILENAME[TL_FADE]);
	if(m_pFilter == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CTitle::Filterの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}



	m_pPlayer = CCharacter::Create(TEX_FILENAME[TL_BG]);
	m_pEnemy = CCharacter::Create(TEX_FILENAME[TL_FADE]);



	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : タイトルの終了処理をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTitle::Finalize(void)
{
	// ----- テクスチャ解放
	SAFE_RELEASE(m_pFilter);	// フィルター
	SAFE_RELEASE(m_pBG);		// 背景

	// ----- カメラ解放
	SAFE_RELEASE(m_pCamera);

	SAFE_RELEASE(m_pPlayer);
	SAFE_RELEASE(m_pEnemy);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : メイン
//	Description : タイトルのメイン処理
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTitle::Main()
{
	// ----- 次のシーンへ
	if(GetTrgKey(DIK_RETURN)) {
		m_phase = PHASE_FADEOUT;		// フェードアウト開始
	}

	m_pPlayer->Update();
	m_pEnemy->Update();

	const float SPD = 10.0f;

	if(GetPrsKey(DIK_RIGHT))
		m_pPlayer->TranslationX(SPD);
	if(GetPrsKey(DIK_LEFT))
		m_pPlayer->TranslationX(-SPD);
	if(GetPrsKey(DIK_UP))
		m_pPlayer->TranslationY(SPD);
	if(GetPrsKey(DIK_DOWN))
		m_pPlayer->TranslationY(-SPD);

	if(GetPrsKey(DIK_O))
		m_pPlayer->RotationZ(1.0f);
	
	if(GetPrsKey(DIK_D))
		m_pEnemy->TranslationX(SPD);
	if(GetPrsKey(DIK_A))
		m_pEnemy->TranslationX(-SPD);
	if(GetPrsKey(DIK_W))
		m_pEnemy->TranslationY(SPD);
	if(GetPrsKey(DIK_S))
		m_pEnemy->TranslationY(-SPD);
	
	if(GetPrsKey(DIK_P))
		m_pEnemy->RotationZ(-1.0f);

	D3DXVECTOR2 pos = D3DXVECTOR2(m_pPlayer->GetPosX(), m_pPlayer->GetPosY());
	pos.x -= 64.0f;
	m_pPlayer->SetColStartLine(pos);
	pos.x += 128.0f;
	m_pPlayer->SetColEndLine(pos);
	int id = COL2D_LINESQUARE;
	if(m_pPlayer->CollisionEnter(id, m_pEnemy)) {
		printf("true\n");
		if(id == COL2D_LINESQUARE || id == COL2D_SQUARELINE) {
			printf("X座標:%f\n", m_pPlayer->GetLastColLinePos().x);
			printf("Y座標:%f\n", m_pPlayer->GetLastColLinePos().y);
			printf("\n");
		}
	}
	
	for(LPFIELDOBJECT_ARRAY_IT it = m_pFieldObj.begin(); it != m_pFieldObj.end(); ++it) {
		(*it)->Update();
	}
	// マップデータ描画(情報のみ)
	if(GetTrgKey(DIK_SPACE)) {
		for(LPFIELDOBJECT_ARRAY_IT it = m_pFieldObj.begin(); it != m_pFieldObj.end(); ++it) {
			printf("X座標         :%f\n", (*it)->GetPosX());
			printf("Y座標         :%f\n", (*it)->GetPosY());
			printf("Z座標         :%f\n", (*it)->GetPosZ());
			printf("幅            :%f\n", (*it)->GetWidth());
			printf("高さ          :%f\n", (*it)->GetHeight());
			printf("回転角度      :%f\n", (*it)->GetRotZ());
			printf("当たり判定有無:%d\n", (*it)->GetCol());
			printf("種別          :%d\n", (*it)->GetType());
			printf("\n");
		}
	}
}


//========================================================================================
//	End of File
//========================================================================================