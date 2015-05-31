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
	_T("res/img/GameScene/Object/player_0.png"),	// プレイヤーテクスチャ名
	_T("res/img/GameScene/Object/block.png"),	// ブロックテクスチャ名
	_T("res/img/GameScene/Object/flower_0.png"),
	_T("res/img/Fade.jpg"),		// フェード用テクスチャファイル名
};
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

	m_pStage		= NULL;
	m_pGameStop		= NULL;
	m_pPlayersGroup = NULL;

	m_phase		= MAX_PHASE;
	m_pNextScene	= SID_RESULT;

	srand((unsigned) time(NULL));
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

	// ----- 次のフェーズへ
	m_phase = PHASE_FADEIN;		// フェードイン開始
	m_pNextScene = SID_RESULT;

	// ステージ初期化
	m_pStage->Init();

	m_pPlayersGroup->Init();
	m_pPlayersGroup->SetStage(m_pStage);
	
	m_pGameStop->Init();
	m_pGameOver->Init();
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

	m_pStage->Uninit();

	m_pGameStop->Uninit();
	m_pGameOver->Uninit();

	m_pPlayersGroup->Uninit();

	for(unsigned int i = 0;i < m_listFlower.size();i++){
		m_listFlower[i]->Uninit();
		SAFE_RELEASE(m_listFlower[i])
	}
	m_listFlower.clear();
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

	// カメラ
				
	if(m_pPlayersGroup->GetPlayer(m_pPlayersGroup->GetPlayNo())){
		m_pCamera->SetNextEye(m_pPlayersGroup->GetPlayer(m_pPlayersGroup->GetPlayNo())->GetPosition());
	}else{
		
	}

	m_pCamera->SetPhase(m_phase);
	m_pCamera->Update();

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
				CGameMain::SetScene(m_pNextScene);	// リザルトへ
			}
			break;

		// ゲーム本編
		case PHASE_MAIN:
			Main();
			break;
		case PHASE_STOP:
			Stop();
			break;
		case PHASE_OVER:
			Over();
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
			DrawMain();
			break;

		case PHASE_STOP:
			DrawStop();
			break;
		case PHASE_OVER:
			DrawOver();
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
	m_pCamera = CGameCamera::Create();
	if(m_pCamera == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::Cameraの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}

	/*　いくみくんが追加したよ　*/
	// ブロック
	m_pStage = CStage::Create();
	m_pStage->SetColBoxTexture(TEX_FILENAME[TL_BLOCK_0]);

	m_pGameStop = CGameStop::Create();
	m_pGameStop->Initialize();

	m_pGameOver = CGameOver::Create();
	m_pGameOver->Initialize();

	// プレイヤー
	m_pPlayersGroup = CPlayersGroup::Create(TEX_FILENAME[TL_PLAYER_0]);
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

	SAFE_RELEASE(m_pStage);		// ブロック
	delete m_pGameStop;
	delete m_pGameOver;
	SAFE_RELEASE(m_pPlayersGroup);
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

	if(GetTrgKey(DIK_RSHIFT)) {
		m_phase = PHASE_STOP;	// 次のシーンへフェードアウト
	}

	// ゲームオーバ
	if(m_pPlayersGroup->GetOver()){
		m_phase = PHASE_OVER;
	}

	// ゲームクリア
	bool Clear = false;
	for(int i = 0;i < m_pStage->GetColBoxMax();i++){
		if(m_pStage->GetColBox(i)->GetType() == BLOCK_TYPE_CLEAR){
			if(m_pStage->GetColBox(i)->GetCol())
				Clear = true;
			else
				Clear =false;
		}
	}

	if(Clear){
		m_phase = PHASE_FADEOUT;
	}

	// リスト内全部更新
	m_pStage->Update();

	// プレイヤーの更新
	m_pPlayersGroup->Update();

	for(int i = 0;i < m_pPlayersGroup->GetGroupSize();i++){
		if(m_pPlayersGroup->GetPlayer(i)){
			if(m_pPlayersGroup->GetPlayer(i)->GetType() == P_TYPE_FLOWER){
				D3DXVECTOR3 pos = D3DXVECTOR3(m_pPlayersGroup->GetPlayer(i)->GetLastColLinePos().x,m_pPlayersGroup->GetPlayer(i)->GetLastColLinePos().y,-5);
				CreateFlower(pos,0);
				m_pPlayersGroup->GetPlayer(i)->EnableDelete();
			}
		}
	}

}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : メイン
//	Description : ゲーム本編のメイン描画
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::DrawMain()
{
	// ステージ描画
	m_pStage->Draw();

	// プレイヤー描画
	m_pPlayersGroup->Draw();

	// 花の描画
	for(unsigned int i = 0;i < m_listFlower.size();i++)
	{
		m_listFlower[i]->DrawAlpha();
	}
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 一時停止
//	Description : ゲーム本編の一時停止処理
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::Stop()
{
	m_pGameStop->Update();

	if(m_pGameStop->GetPhase() == GAME_STOP_PHASE_END){
		switch(m_pGameStop->GetGo())
		{
		case GO_GAME:
			m_phase = PHASE_MAIN;
			break;
		case GO_RESET:
			m_phase = PHASE_FADEOUT;
			m_pNextScene = SID_GAME;
			break;
		case GO_SELECT:
			m_phase = PHASE_FADEOUT;
			m_pNextScene = SID_SELECT;
			break;
		}
		m_pGameStop->Init();
	}
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 一時停止
//	Description : ゲーム本編の一時停止描画
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::DrawStop()
{
	DrawMain();
	m_pGameStop->Draw();
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 一時停止
//	Description : ゲーム本編の一時停止処理
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::Over()
{
	m_pGameOver->Update();

	if(m_pGameOver->GetPhase() == GAME_STOP_PHASE_END){
		switch(m_pGameOver->GetGo())
		{
		case 0:
			m_phase = PHASE_FADEOUT;
			m_pNextScene = SID_GAME;
			break;
		case 1:
			m_phase = PHASE_FADEOUT;
			m_pNextScene = SID_SELECT;
			break;
		}
		m_pGameOver->Init();
	}
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 一時停止
//	Description : ゲーム本編の一時停止描画
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::DrawOver()
{
	DrawMain();
	m_pGameOver->Draw();
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 花の生成
//	Description :　
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::CreateFlower(D3DXVECTOR3 pos,float angle)
{
	CFlower* flower;
	flower = CFlower::Create(TEX_FILENAME[TL_FLOWER_0]);
	flower->Init(pos,angle);

	m_listFlower.push_back(flower);
}

//========================================================================================
//	End of File
//========================================================================================