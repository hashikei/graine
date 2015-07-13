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
	_T("res/img/rotate.png"),		// 背景2テクスチャファイル名
	_T("res/img/Start.png"),		// スタートアイコン(仮)
	_T("res/img/rogo.png"),			// タイトルロゴ(仮)
};
const D3DXVECTOR3 CTitle::INIT_CAMERA_EYE(0, 0, -1000);		// カメラの初期視点
const D3DXVECTOR3 CTitle::INIT_CAMERA_LOOK(0, 0, 0);		// カメラの初期注視点
const D3DXVECTOR3 CTitle::INIT_TEXTURE_POS[MAX_TEXLIST] = {	// テクスチャの初期位置
	D3DXVECTOR3(0.0f, 0.0f, FAR_CLIP),	// 背景
	D3DXVECTOR3(420.0f, 200.0f, FAR_CLIP),	// 背景
	D3DXVECTOR3(440.0f, 350.0f, 255),	// スタートアイコン画像
	D3DXVECTOR3(150.0f, 50.0f, 255),	// タイトルロゴ
};

// ----- フェード関連
const float CTitle::FADE_POSZ	= -100.0f;	// フェード用テクスチャのZ座標
const int CTitle::FADEIN_TIME	= 10;		// フェードイン間隔(アルファ値:0～255)
const int CTitle::FADEOUT_TIME	= 10;		// フェードアウト間隔(アルファ値:0～255)


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
	m_pBG2		= NULL;
	m_pStart	= NULL;
	m_pTitle	= NULL;

	m_phase = MAX_PHASE;
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
	m_pBG->Init(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT), D3DXVECTOR3(568, 320, 1));			// 背景
	m_pBG2->Init(D3DXVECTOR2((float)300, (float)300), D3DXVECTOR3(568, 320, 255));							// 背景2
	m_pStart->Init(D3DXVECTOR2((float)250, (float)100), D3DXVECTOR3(568, 500, 255));						// スタートアイコン画像
	m_pTitle->Init(D3DXVECTOR2((float)450, (float)250), D3DXVECTOR3(568, 125, 0));							// タイトル画像

	// ----- フェード設定
	CChangeScene::SetNormalFadeAlpha(255);
	
	// ----- BGM再生
	CGameMain::PlayBGM(BGM_TITLE, DSBPLAY_LOOPING);

	// ----- 次のフェーズへ
	m_phase = PHASE_FADEIN;		// フェードイン開始


	m_pPlayer->Init(D3DXVECTOR2(100.0f, 1.0f), D3DXVECTOR3(-200.0f, 0.0f, -100.0f));
	m_pEnemy->Init(D3DXVECTOR2(100.0f, 100.0f), D3DXVECTOR3(200.0f, 0.0f, -100.0f));
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
	if (m_phase != PHASE_FADEOUT)
		m_phase = PHASE_FADEOUT;

	// ----- テクスチャ後始末
	m_pBG->Uninit();		// 背景
	m_pBG2->Uninit();		// 背景2
	m_pStart->Uninit();		// スタート
	m_pTitle->Uninit();		// タイトル

	// ----- BGM停止
	CGameMain::StopBGM(BGM_TITLE);
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

	switch (m_phase)
	{
		// フェードイン
		case PHASE_FADEIN:
			if (CChangeScene::NormalFadeOut(FADE_POSZ, FADEIN_TIME)) {
				m_phase = PHASE_MAIN;							// タイトル本編開始
			}
			break;

			// フェードアウト
		case PHASE_FADEOUT:
			if (CChangeScene::NormalFadeIn(FADE_POSZ, FADEOUT_TIME))
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
	m_pBG->DrawScreenAlpha();			// 背景

	m_pBG2->DrawScreenAlpha();		// 背景2
	m_pBG2->RotationZ(0.4f);
	m_pTitle->DrawScreenAlpha();

	m_pStart -> DrawScreenAlpha();		// スタート

	m_pTitle->DrawScreenAlpha();		// タイトル

	switch (m_phase)
	{
		// フェードイン・アウト
		case PHASE_FADEIN:
		case PHASE_FADEOUT:
			CChangeScene::DrawNormalFade();
			break;

		// タイトル本編
		case PHASE_MAIN:
			break;

		default:
			break;
	}



	m_pPlayer->DrawAlpha();
	m_pEnemy->DrawAlpha();
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
	if (pTitle)
	{
		if (!pTitle->Initialize())
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
	if (m_pCamera == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CTitle::Cameraの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}

	// ----- テクスチャ生成
	// 背景
	m_pBG = CObject2D::Create(TEX_FILENAME[TL_BG]);
	if (m_pBG == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CTitle::BGの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}

	//背景2
	m_pBG2 = CCharacter::Create(TEX_FILENAME[TL_BG2]);
	if (m_pBG2 == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CTitle::BG2の生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}
	// スタート
	m_pStart = CCharacter::Create(TEX_FILENAME[TL_START]);
	if (m_pStart == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CTitle::Startの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}
	// タイトル
	m_pTitle = CObject2D::Create(TEX_FILENAME[TL_TITLE]);
	if (m_pTitle == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CTitle::Titleの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}



	m_pPlayer = CCharacter::Create(TEX_FILENAME[3]);
	m_pEnemy = CCharacter::Create(TEX_FILENAME[0]);

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
	SAFE_RELEASE(m_pBG);		// 背景
	SAFE_RELEASE(m_pBG2);		// 背景2
	SAFE_RELEASE(m_pStart);		// スタート
	SAFE_RELEASE(m_pTitle);		// タイトル


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
	if (GetTrgKey(DIK_RETURN)) {
		CGameMain::PlaySE(SE_ENTER);
		m_phase = PHASE_FADEOUT;		// フェードアウト開始
	}
	
	static bool UpDown = false;
	D3DXVECTOR2 Pos(m_pStart->GetPosX(), m_pStart->GetPosY());
	if (!UpDown)
	{
		m_pStart->TranslationY(1.8f);
		if (Pos.y > 510)
		{
			UpDown = true;

		}
	}
	else{
		m_pStart->TranslationY(-0.6f);
		if (Pos.y < 490)
		{
			UpDown = false;

		}
	}


	const float SPD = 10.0f;
	const float ROT = 10.0f;
	float spd = SPD;
	float rot = ROT;
	if (GetPrsKey(DIK_LSHIFT) || GetPrsKey(DIK_RSHIFT)) {
		spd = 1.0f;
		rot = 1.0f;
	}
	if (GetPrsKey(DIK_RIGHT)) {
		m_pPlayer->TranslationX(spd);
	}
	if (GetPrsKey(DIK_LEFT)) {
		m_pPlayer->TranslationX(-spd);
	}
	if (GetPrsKey(DIK_UP)) {
		m_pPlayer->TranslationY(spd);
	}
	if (GetPrsKey(DIK_DOWN)) {
		m_pPlayer->TranslationY(-spd);
	}

	if (GetPrsKey(DIK_Q)) {
		m_pPlayer->RotationZ(rot);
	}
	if (GetPrsKey(DIK_W)) {
		m_pPlayer->RotationZ(-rot);
	}
	if (GetPrsKey(DIK_A)) {
		m_pEnemy->RotationZ(rot);
	}
	if (GetPrsKey(DIK_S)) {
		m_pEnemy->RotationZ(-rot);
	}

	D3DXVECTOR2 start = D3DXVECTOR2(-m_pPlayer->GetHalfWidth(), 0.0f);
	D3DXVec2TransformCoord(&start, &start, &m_pPlayer->GetMatrix());
	D3DXVECTOR2 end = D3DXVECTOR2(m_pPlayer->GetHalfWidth(), 0.0f);
	D3DXVec2TransformCoord(&end, &end, &m_pPlayer->GetMatrix());
	m_pPlayer->SetColStartLine(start);
	m_pPlayer->SetColEndLine(end);

	if (m_pPlayer->CollisionEnter(COL2D_LINESQUARE, m_pEnemy)) {
		printf("hit!\n");
	}
}


//========================================================================================
//	End of File
//========================================================================================