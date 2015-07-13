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
#include "../../h/System/ChangeScene.h"
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
	_T("res/img/GameScene/BG/dark.png"),		// 背景テクスチャファイル名
	_T("res/img/GameScene/BG/light.png"),		// 背景テクスチャファイル名
	_T("res/img/GameScene/Object/player_0.png"),	// プレイヤーテクスチャ名
	_T("res/img/GameScene/Object/block.png"),	// ブロックテクスチャ名
	_T("res/img/GameScene/Object/flower_0.png"),
	_T("res/img/GameScene/Object/kuki.png"),
	_T("res/img/GameScene/Object/turu_1.png"),
	_T("res/img/GameScene/Object/Stone.png"),
	_T("res/img/GameScene/Object/Clip.png"),
	_T("res/img/GameScene/Object/player_0.png"),
};
const D3DXVECTOR3 CGame::INIT_TEXTURE_POS[MAX_TEXLIST] = {	// テクスチャの初期位置
	D3DXVECTOR3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, FAR_CLIP),	// 背景
	D3DXVECTOR3(0.0f, 0.0f, 0.0f),							// フィルター
};

// フェード関連
const float CGame::FADE_POSZ = -100.0f;	// フェード用テクスチャのZ座標
const int CGame::FADEIN_TIME = 5;		// フェードイン間隔(アルファ値:0～255)
const int CGame::FADEOUT_TIME = 10;		// フェードアウト間隔(アルファ値:0～255)

D3DXVECTOR2	CGame::CLIP_SIZE		= D3DXVECTOR2(500.0f, 500.0f);			// クリッピングサイズ
D3DXVECTOR3	CGame::CLIP_INITPOS		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// クリッピング初期位置
float		CGame::CLIP_SCALING_SPD	= 7.1f;									// クリッピング拡大速度
float		CGame::CLIP_LATEST_SPD	= 0.1f;									// クリッピング最遅速度

float	CGame::SCROLL_EFFECT_SPD	= 0.001f;		// スクロールエフェクト移動速度

// ----- メンバ変数
// private:
int CGame::m_stageID;		// 選択したステージのID


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CGame::CGame()
{
	m_pDarkBG = NULL;
	m_pLightBG = NULL;
	m_pCamera = NULL;

	m_pStage = NULL;
	m_stageID = 0;

	m_pGameStop = NULL;
	m_pGameOver = NULL;
	m_pGameClear = NULL;
	m_pPlayersGroup = NULL;

	m_phase = MAX_PHASE;
	m_pNextScene = SID_RESULT;

	m_pClipCircle = NULL;
	m_clipInfoList.reserve(100);

	m_pScrollEffect = NULL;

	srand((unsigned)time(NULL));
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
	m_pDarkBG->Init(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT), INIT_TEXTURE_POS[TL_BG_DARK]);			// 背景
	m_pLightBG->Init(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT), INIT_TEXTURE_POS[TL_BG_LIGHT]);		// 背景
	m_pLightBG->TranslationZ(1.0f);

	m_pClipCircle->Init(CLIP_SIZE, CLIP_INITPOS);

	// ----- カメラ初期化
	m_pCamera->Init();

	// ----- 次のフェーズへ
	m_phase = PHASE_FADEIN;		// フェードイン開始
	m_pNextScene = SID_RESULT;

	// ステージ初期化
	m_pStage->Init(m_stageID);

	m_pPlayersGroup->Init();
	m_pPlayersGroup->SetStage(m_pStage);

	m_pGameStop->Init();
	m_pGameOver->Init();
	m_pGameClear->Init();

	// ----- クリッピング設定初期化
	m_clipInfoList.clear();
	m_clipEasingList.clear();
	
	// ----- スクロールエフェクト設定初期化
	D3DXVECTOR2 size = m_pStage->GetLayoutBlock(1)->GetSize();
	D3DXVECTOR3 pos  = m_pStage->GetLayoutBlock(1)->GetPosition();
	pos.z -= 0.5f;
	m_pScrollEffect->Init(size, pos);

	// ----- フェード設定
	CChangeScene::SetNormalFadeAlpha(255);

	// ----- BGM再生
	CGameMain::PlayBGM(BGM_RESULT, DSBPLAY_LOOPING);
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
	m_pDarkBG->Uninit();			// 背景
	m_pLightBG->Uninit();			// 背景

	// ----- オブジェクト後始末
	m_pCamera->Uninit();			// カメラ

	m_pStage->Uninit();

	m_pGameStop->Uninit();
	m_pGameOver->Uninit();
	m_pGameClear->Uninit();

	m_pPlayersGroup->Uninit();

	m_pClipCircle->Uninit();

	m_pScrollEffect->Uninit();

	// ----- BGM停止
	CGameMain::StopBGM(BGM_RESULT);

	for (unsigned int i = 0; i < m_listFlower.size(); i++){
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

	if (m_pPlayersGroup->GetPlayer(m_pPlayersGroup->GetPlayNo())){
		m_pCamera->SetNextEye(m_pPlayersGroup->GetPlayer(m_pPlayersGroup->GetPlayNo())->GetPosition());
	}
	else{

	}

	// カメラのフェイズを同期
	m_pCamera->SetPhase(m_phase);

	m_pCamera->Update();

	switch (m_phase)
	{
		// フェードイン
	case PHASE_FADEIN:
		if (CChangeScene::NormalFadeOut(FADE_POSZ, FADEIN_TIME))
			m_phase = PHASE_MAIN;		// 開始準備
		break;

		// 次のシーンへフェードアウト
	case PHASE_FADEOUT:
		if (CChangeScene::NormalFadeIn(FADE_POSZ, FADEOUT_TIME))
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
	case PHASE_CLEAR:
		Clear();
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
	m_pLightBG->DrawScreen();		// 背景
	m_pDarkBG->DrawScreen();		// 背景

	switch (m_phase)
	{
		// フェードイン・アウト
	case PHASE_FADEIN:
	case PHASE_FADEOUT:
		CChangeScene::DrawNormalFade();
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
	case PHASE_CLEAR:
		DrawClear();
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
	if (pGame)
	{
		if (!pGame->Initialize())
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
	m_pDarkBG = CObject2D::Create(TEX_FILENAME[TL_BG_DARK]);
	if (m_pDarkBG == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::DarkBGの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}
	m_pLightBG = CObject2D::Create(TEX_FILENAME[TL_BG_LIGHT]);
	if (m_pLightBG == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::LightBGの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}

	// ----- オブジェクト生成
	// カメラ
	m_pCamera = CGameCamera::Create();
	if (m_pCamera == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::Cameraの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}

	// クリッピング用の円
	m_pClipCircle = CCharacter::Create(TEX_FILENAME[TL_CLIP]);
	if (m_pClipCircle == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::ClipCircleの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}

	// 背景でスクロールするエフェクト
	m_pScrollEffect = CCharacter::Create(TEX_FILENAME[TL_SCROLL_EFFECT]);
	if (m_pScrollEffect == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::ScrollEffectの生成に失敗しました。"), _T("error"), MB_OK);
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

	m_pGameClear = CGameClear::Create();
	m_pGameClear->Initialize();

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
	SAFE_RELEASE(m_pDarkBG);	// 背景
	SAFE_RELEASE(m_pLightBG);	// 背景

	SAFE_RELEASE(m_pStage);		// ブロック

	SAFE_RELEASE(m_pGameStop);
	SAFE_RELEASE(m_pGameOver);
	SAFE_RELEASE(m_pGameClear);

	SAFE_RELEASE(m_pPlayersGroup);

	SAFE_RELEASE(m_pClipCircle);
	SAFE_RELEASE(m_pScrollEffect);
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
	if (GetTrgKey(DIK_RETURN)) {
		m_phase = PHASE_FADEOUT;	// 次のシーンへフェードアウト
	}

	if (GetTrgKey(DIK_RSHIFT)) {
		CGameMain::PlaySE(SE_POSE);
		m_phase = PHASE_STOP;	// 次のシーンへフェードアウト
	}

	// ゲームオーバ
	if (m_pPlayersGroup->GetOver()){
		m_phase = PHASE_OVER;
	}

	// ゲームクリア
	bool Clear = false;
	for (int i = 0; i < m_pStage->GetMaxFieldBlock(); i++){
		CFieldBlock* pFieldBlock = m_pStage->GetFieldBlock(i);
		if (pFieldBlock->GetType() == CMapData::BT_CLEAR){
			if (pFieldBlock->GetFloawerNum())
				Clear = true;
			else
				Clear = false;
		}
	}

	// ゲームクリア演出開始
	if (Clear || GetTrgKey(DIK_Q)) {	// デバッグ用
		m_phase = PHASE_CLEAR;

		// カメラを引く距離を算出
		float widthDist = CMapData::GetRightLimit() - CMapData::GetLeftLimit();
		float heightDist = CMapData::GetTopLimit() - CMapData::GetBottomLimit();
		if (widthDist > heightDist)
			m_pGameClear->SetDirectionDistance(widthDist);
		else
			m_pGameClear->SetDirectionDistance(heightDist);

		// カメラセット
		D3DXVECTOR2 cameraPos(CMapData::GetRightLimit() - widthDist * 0.5f, CMapData::GetTopLimit() - heightDist * 0.5f);
		m_pGameClear->SetCameraStartPos(cameraPos);
		m_pGameClear->SetCamera(m_pCamera);

	}

	// リスト内全部更新
	m_pStage->Update();

	// プレイヤーの更新
	m_pPlayersGroup->Update();

	// プレイヤーのコールに周りの奴は対応する
	for (int i = 0; i < m_pPlayersGroup->GetGroupSize(); i++){
		if (m_pPlayersGroup->GetPlayer(i)){
			if (m_pPlayersGroup->GetPlayer(i)->GetStatus() & ST_CALL){
				for (int j = 0; j < m_pPlayersGroup->GetGroupSize(); j++){
					if (m_pPlayersGroup->GetPlayer(j)->GetType() == P_TYPE_WAIT){
						float length = D3DXVec3Length(&(m_pPlayersGroup->GetPlayer(j)->GetPosition() - m_pPlayersGroup->GetPlayer(i)->GetPosition()));
						if (length < 300){
							m_pPlayersGroup->GetPlayer(j)->SetType(P_TYPE_OTHER);
						}
					}
				}
				m_pPlayersGroup->GetPlayer(i)->SubStatus(ST_CALL);
			}
		}
	}
	// プレイヤーが花状態になったら花咲かす
	for(int i = 0;i < m_pPlayersGroup->GetGroupSize();i++){
		if(m_pPlayersGroup->GetPlayer(i)){
			if(m_pPlayersGroup->GetPlayer(i)->GetType() == P_TYPE_FLOWER){
				D3DXVECTOR3 pos = D3DXVECTOR3(m_pPlayersGroup->GetPlayer(i)->GetLastColLinePos().x,m_pPlayersGroup->GetPlayer(i)->GetLastColLinePos().y,m_pPlayersGroup->GetPlayer(i)->GetPosZ() + 10);
				D3DXVECTOR3 dir;
				D3DXVECTOR3 vec = D3DXVECTOR3(m_pPlayersGroup->GetPlayer(i)->GetLastColLine().x,m_pPlayersGroup->GetPlayer(i)->GetLastColLine().y,0);
				D3DXVec3Cross(&dir,&vec,&D3DXVECTOR3(0,0,1));
				D3DXVec3Normalize(&dir,&dir);

				switch(m_pPlayersGroup->GetPlayer(i)->GetGrane()){
				case PLAYER_ARROW:
				case PLAYER_NORMAL:
					{
						CreateFlower(pos,dir);

						TCLIPINFO clipInfo;
						clipInfo.pos = pos;
						clipInfo.size = D3DXVECTOR2(0.0f, 0.0f);
						m_clipInfoList.push_back(clipInfo);

					float easing = CLIP_SCALING_SPD;		// 減速
					m_clipEasingList.push_back(easing);

					break;
				}
				case PLAYER_JACK:
					{
						for(int i = 0;;i++){
							if(m_pStage->GetFieldBlock(i)->GetType() == CMapData::BT_NORMAL){
								m_pStage->GetFieldBlock(i)->SetElement(CreateJack(pos,dir));
								break;
							}
						}
						
						break;
					}
				case PLAYER_STONE:
					{
						for(int i = 0;;i++){
							if(m_pStage->GetFieldBlock(i)->GetType() == CMapData::BT_NORMAL){
								m_pStage->GetFieldBlock(i)->SetElement(CreateStone(pos,dir));
								break;
							}
						}
						break;
					}

				}
				(m_pPlayersGroup)->GetPlayer(i)->EnableDelete();
			}
		}
	}

	for (unsigned int i = 0; i < m_listFlower.size(); i++)
	{
		m_listFlower[i]->Update();

		// 花状態のときに種を増やす
		if (m_listFlower[i]->GetPhase() == FLOWER_PHASE_FLOWER){
			D3DXVECTOR3 move;
			D3DXVECTOR3 pos1;
			D3DXVec3Cross(&move, &m_listFlower[i]->GetPosition(), &D3DXVECTOR3(0, 0, 1));
			D3DXVec3Normalize(&move, &move);
			pos1 = m_listFlower[i]->GetPosition() + (move * (FLOWER_SIZE_X / 2));
			pos1.z = pos1.z - 10;
			D3DXVECTOR3 pos2;
			D3DXVec3Cross(&move, &m_listFlower[i]->GetPosition(), &D3DXVECTOR3(0, 0, -1));
			D3DXVec3Normalize(&move, &move);
			pos2 = m_listFlower[i]->GetPosition() + (move * (FLOWER_SIZE_X / 2));
			pos2.z = pos2.z - 10;

			if(m_pPlayersGroup->GetGroupSize() < 8)
				m_pPlayersGroup->AddPlayer(pos1);
			if(m_pPlayersGroup->GetGroupSize() < 9)
				m_pPlayersGroup->AddPlayer(pos2);
			m_listFlower[i]->SetPhase(FLOWER_PHASE_WAIT);
		}
	}

	// ----- 緑化クリッピング領域拡大
	for (unsigned int i = 0; i < m_clipInfoList.size(); ++i) {
		if (m_clipInfoList[i].size.x >= CLIP_SIZE.x)
			continue;
/*
		m_clipEasingList[i] += CLIP_SCALING_SPD;
		m_clipInfoList[i].size.x += m_clipEasingList[i];
		m_clipInfoList[i].size.y += m_clipEasingList[i];
		if (m_clipInfoList[i].size.x > CLIP_SIZE.x)
			m_clipInfoList[i].size = CLIP_SIZE;
*/

		if (m_clipEasingList[i] > CLIP_LATEST_SPD)
			m_clipEasingList[i] -= CLIP_LATEST_SPD;
		m_clipInfoList[i].size.x += m_clipEasingList[i];
		m_clipInfoList[i].size.y += m_clipEasingList[i];
		if (m_clipInfoList[i].size.x > CLIP_SIZE.x)
			m_clipInfoList[i].size = CLIP_SIZE;

	}

	// ----- スクロールエフェクト移動処理
	m_pScrollEffect->UVScroll(SCROLL_EFFECT_SPD, 0.0f);


	//const float SPD = 10.0f;
	//const float SCALE = 0.1f;
	//float spd = SPD;
	//float scale = SCALE;
	//if (GetPrsKey(DIK_LSHIFT) || GetPrsKey(DIK_RSHIFT)) {
	//	spd = 1.0f;
	//	scale = 0.01f;
	//}
	//if (GetPrsKey(DIK_RIGHT)) {
	//	m_pClipCircle->TranslationX(spd);
	//}
	//if (GetPrsKey(DIK_LEFT)) {
	//	m_pClipCircle->TranslationX(-spd);
	//}
	//if (GetPrsKey(DIK_UP)) {
	//	m_pClipCircle->TranslationY(spd);
	//}
	//if (GetPrsKey(DIK_DOWN)) {
	//	m_pClipCircle->TranslationY(-spd);
	//}

	//if (GetPrsKey(DIK_Q)) {
	//	m_pClipCircle->Scaling(D3DXVECTOR3(scale, scale, 0.0f));
	//}
	//if (GetPrsKey(DIK_W)) {
	//	m_pClipCircle->Scaling(D3DXVECTOR3(-scale, -scale, 0.0f));
	//}
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : メイン
//	Description : ゲーム本編のメイン描画
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::DrawMain()
{
	CGraphics::StencilRegionBegin();


	for (CLIPINFO_ARRAY_IT it = m_clipInfoList.begin(); it != m_clipInfoList.end(); ++it) {
		m_pClipCircle->Translate((*it).pos);
		m_pClipCircle->Resize ((*it).size);
		m_pClipCircle->DrawAlpha();
	}

	CGraphics::StencilRegionEnd();

	CGraphics::StencilDrawBegin();

	// ステージ描画
	m_pStage->DrawFieldBlock();
	m_pStage->DrawLayoutBlock(0);

	CGraphics::StencilDrawEnd();

	m_pStage->DrawLayoutBlock(1);
	m_pScrollEffect->DrawAlpha();



	// プレイヤー描画
	m_pPlayersGroup->Draw();

	// 花の描画
	for (std::vector<CFlower*>::iterator it = m_listFlower.begin(); it != m_listFlower.end(); ++it)
	{
		(*it)->DrawAlpha();
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

	if (m_pGameStop->GetPhase() == GAME_STOP_PHASE_END){
		switch (m_pGameStop->GetGo())
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

	if (m_pGameOver->GetPhase() == GAME_STOP_PHASE_END){
		switch (m_pGameOver->GetGo())
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
//	Name        : ゲームクリアメイン
//	Description : ゲームクリア時のメイン処理
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::Clear()
{
	m_pGameClear->Update();

	if (m_pGameClear->GetPhase() == CGameClear::PHASE_END){
		switch (m_pGameClear->GetGo())
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
		m_pGameClear->Init();
	}

	CGameMain::EnableStageClear(m_stageID);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : ゲームクリア描画
//	Description : ゲームクリア時の描画処理
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::DrawClear()
{
	DrawMain();
	m_pGameClear->Draw();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 花の生成
//	Description :　
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::CreateFlower(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	CFlower* flower;
	flower = CFlower::Create(TEX_FILENAME[TL_FLOWER_0]);
	flower->Init(pos, dir, TEX_FILENAME[TL_FLOWER_1]);

	m_listFlower.push_back(flower);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 花の生成
//	Description :　
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CCharacter* CGame::CreateJack(D3DXVECTOR3 pos,D3DXVECTOR3 dir)
{
	CJack* flower;
	flower = CJack::Create(TEX_FILENAME[TL_JACK_0]);
	flower->Init(pos, dir);

	m_listFlower.push_back(flower);

	return flower->GetCol();
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 花の生成
//	Description :　
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CCharacter* CGame::CreateStone(D3DXVECTOR3 pos,D3DXVECTOR3 dir)
{
	CStone* flower;
	flower = CStone::Create(TEX_FILENAME[TL_STONE_0]);
	flower->Init(pos,dir);

	m_listFlower.push_back(flower);

	return flower->GetCol();
}


//========================================================================================
//	End of File
//========================================================================================