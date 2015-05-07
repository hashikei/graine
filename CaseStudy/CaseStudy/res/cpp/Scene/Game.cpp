//========================================================================================
//		File        : Game.cpp
//		Program     : ゲーム本編クラス
//
//		Description : ゲーム本編クラスの実装
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
#include <math.h>
#include "../../h/System/System.h"
#include "../../h/System/Input.h"
#include "../../h/Scene/GameMain.h"
#include "../../h/Scene/Game.h"

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
const LPCTSTR CGame::TEX_FILENAME[MAX_TEXLIST] = {
	_T("res/img/GameScene/BG/pantsu.jpg"),		// 背景テクスチャファイル名
	_T("res/img/GameScene/Object/player.png"),	// プレイヤーテクスチャ名
	_T("res/img/GameScene/Object/block.png"),	// ブロックテクスチャ名
	_T("res/img/Fade.jpg"),		// フェード用テクスチャファイル名
};
const D3DXVECTOR3 CGame::INIT_CAMERA_EYE(0, 0, -1000);		// カメラの初期視点
const D3DXVECTOR3 CGame::INIT_CAMERA_LOOK(0, 0, 0);			// カメラの初期注視点
const D3DXVECTOR3 CGame::INIT_TEXTURE_POS[MAX_TEXLIST] = {	// テクスチャの初期位置
	D3DXVECTOR3(0.0f, 0.0f, FAR_CLIP),						// 背景
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// フィルター
};

// ----- フェード関連
const int CGame::FADEIN_TIME		= 5;	// フェードイン間隔(アルファ値:0～255)
const int CGame::FADEOUT_TIME		= 10;	// フェードアウト間隔(アルファ値:0～255)


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CGame::CGame()
{
	m_pBG			= NULL;
	m_pFilter		= NULL;
	m_pCamera		= NULL;

	m_pBlock		= NULL;

	m_pPlayer		= NULL;

	m_phase		= MAX_PHASE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CGame::~CGame()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : ゲーム本編を初期化する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::Init(void)
{
	// ----- テクスチャ初期化
	m_pBG->Init(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT), INIT_TEXTURE_POS[TL_BG]);				// 背景
	m_pFilter->Init(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT), INIT_TEXTURE_POS[TL_FADE]);			// フィルター

	// ----- カメラ初期化
	m_pCamera->Init();
	D3DXVECTOR3 eye = INIT_CAMERA_EYE;
	D3DXVECTOR3 look = INIT_CAMERA_LOOK;
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	m_pCamera->SetParameter(eye, look, up);

	// ----- 次のフェーズへ
	m_phase = PHASE_FADEIN;		// フェードイン開始

	// オブジェクト数
	vecFieldObj.reserve(MAX_OBJECT);

	// ----- ブロック初期化
	m_pBlock->Init(D3DXVECTOR2(512, 64), D3DXVECTOR3(-256, -256, 0));

	// ::::: リストに追加 ::::: //
	vecFieldObj.push_back(m_pBlock);

	m_pPlayer->Init(D3DXVECTOR2(PLAYER_SIZE_X, PLAYER_SIZE_Y), D3DXVECTOR3(-256, 0, 0));
	m_pPlayer->UVDivision(0, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y);

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : ゲーム本編の終了処理をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::Uninit(void)
{
	// ----- テクスチャ後始末
	m_pBG->Uninit();				// 背景
	m_pFilter->Uninit();		// フィルター

	// ----- オブジェクト後始末
	m_pCamera->Uninit();			// カメラ

	// リスト内全部後始末
	for (LPOBJECT2D_ARRAY_IT it = vecFieldObj.begin(); it != vecFieldObj.end(); ++it)
		(*it)->Uninit();
	vecFieldObj.clear();		// オブジェクトリスト

	m_pPlayer->Uninit();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : ゲーム本編を更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::Update(void)
{
	// ----- オブジェクト更新
	m_pCamera->Update();			// カメラ
				
	switch(m_phase)
	{
		// フェードイン
		case PHASE_FADEIN:
			if(m_pFilter->FadeOutAlpha(FADEIN_TIME))
				m_phase = PHASE_MAIN;		// 開始準備
			break;

		// 次のシーンへフェードアウト
		case PHASE_FADEOUT:
			if(m_pFilter->FadeInAlpha(FADEOUT_TIME))
			{	// 次のシーンへ
				Uninit();							// 後始末
				CGameMain::SetScene(SID_RESULT);	// リザルトへ
			}
			break;

		// ゲーム本編
		case PHASE_MAIN:
			Main();
			break;
			
		default:
			break;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : ゲーム本編を描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::Draw(void)
{
	// ----- カメラ描画
	m_pCamera->Draw();

	// ----- テクスチャ描画
	m_pBG->DrawAlpha();			// 背景

	switch(m_phase)
	{
		// フェードイン・アウト
		case PHASE_FADEIN:
		case PHASE_FADEOUT:
			m_pFilter->DrawAlpha();
			break;

		// ゲーム本編
		case PHASE_MAIN:
			// オブジェクトリスト全部描画
			for (unsigned int i = 0; i < vecFieldObj.size(); i++)
				vecFieldObj[i]->DrawAlpha();
			
			m_pPlayer->DrawAlpha();
			break;
			
		default:
			break;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成処理
//	Description : ゲーム本編データを生成する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CGame* CGame::Create()
{
	// ----- 変数宣言
	CGame* pGame;

	// ----- 初期化処理
	pGame = new CGame();
	if(pGame)
	{
		if(!pGame->Initialize())
		{
			SAFE_DELETE(pGame);
		}
	}

	return pGame;
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : ゲーム本編を初期化する
//	Arguments   : None.
//	Returns     : 成否(true;成功)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CGame::Initialize()
{
	// ----- テクスチャ生成
	// 背景
	m_pBG = CTexture::Create(TEX_FILENAME[TL_BG]);
	if(m_pBG == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::BGの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}
	// フェード用フィルター
	m_pFilter = CTexture::Create(TEX_FILENAME[TL_FADE]);
	if(m_pFilter == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::Filterの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}

	// ----- オブジェクト生成
	// カメラ
	m_pCamera = CCamera::Create();
	if(m_pCamera == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::Cameraの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}

	/*　いくみくんが追加したよ　*/
	// ブロック
	m_pBlock = CFieldObject::Create(TEX_FILENAME[TL_BLOCK_0]);
	// プレイヤー
	m_pPlayer = CPlayer::Create(TEX_FILENAME[TL_PLAYER_0]);
	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : ゲーム本編の終了処理をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::Finalize(void)
{
	// ----- オブジェクト解放
	SAFE_RELEASE(m_pCamera);	// カメラデータ
	
	// ----- テクスチャ解放
	SAFE_RELEASE(m_pFilter);	// フィルター
	SAFE_RELEASE(m_pBG);		// 背景

	SAFE_RELEASE(m_pBlock);		// ブロック
	SAFE_RELEASE(m_pPlayer);	// プレイヤー
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : メイン
//	Description : ゲーム本編のメイン処理
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::Main()
{
	// ----- 次のシーンへ
	if(GetTrgKey(DIK_RETURN)) {
		m_phase = PHASE_FADEOUT;	// 次のシーンへフェードアウト
	}

	// リスト内全部更新
	for (unsigned int i = 0; i < vecFieldObj.size(); i++)
		vecFieldObj[i]->Update();

	// プレイヤーの更新
	m_pPlayer->Update();

	if (m_pPlayer->CollisionStay(COL2D_BOUNDINGBOX, m_pBlock)){
		m_pPlayer->SetGravity(GRAVITY_ZERO);

	}
	else{
		m_pPlayer->SetGravity(GRAVITY_CASE_2);
	}
}


//========================================================================================
//	End of File
//========================================================================================