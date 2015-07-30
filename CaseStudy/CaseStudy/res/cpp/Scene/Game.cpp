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
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <process.h>
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
// public:
const float	CGame::OBJ_GAME_PRIORITIES[MAX_OBJECTLIST] = {
	-1.f,	// OL_LB_DARK,
	0.0f,	// OL_PLAYER_DARK,
	0.3f,	// OL_TACTILE_DARK,
	3.0f,	// OL_FLOWER_DARK,
	4.0f,	// OL_STONE_DARK,
	5.0f,	// OL_JACK_DARK,
	6.0f,	// OL_SCROLL_DARK,
	7.0f,	// OL_BG_DARK,
	8.0f,	// OL_LAYOUT_OBJECT,
	9.0f,	// OL_LB_LIGHT,
	10.0f,	// OL_PLAYER_LIGHT,
	10.3f,	// OL_TACTILE_LIGHT,
	13.0f,	// OL_FLOWER_LIGHT,
	14.0f,	// OL_STONE_LIGHT,
	15.0f,	// OL_JACK_LIGHT,
	16.0f,	// OL_SCROLL_LIGHT,
	17.0f,	// OL_BG_LIGHT,	
};

const float	CGame::OBJ_CLEAR_PRIORITIES[MAX_OBJECTLIST] = {
	-3.0f,	// OL_LB_DARK,
	0.0f,	// OL_PLAYER_DARK,
	3.0f,	// OL_TACTILE_DARK,
	6.0f,	// OL_FLOWER_DARK,
	9.0f,	// OL_STONE_DARK,
	12.0f,	// OL_JACK_DARK,
	15.0f,	// OL_SCROLL_DARK,
	18.0f,	// OL_BG_DARK,
	21.0f,	// OL_LAYOUT_OBJECT,
	24.0f,	// OL_LB_LIGHT,
	27.0f,	// OL_PLAYER_LIGHT,
	30.0f,	// OL_TACTILE_LIGHT,
	33.0f,	// OL_FLOWER_LIGHT,
	36.0f,	// OL_STONE_LIGHT,
	39.0f,	// OL_JACK_LIGHT,
	42.0f,	// OL_SCROLL_LIGHT,
	45.0f,	// OL_BG_LIGHT,	
};

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
	_T("res/img/GameScene/Object/Effect.png"),
	_T("res/img/Fade.jpg"),
	_T("res/img/NowLoading/nl.png"),
	_T("res/img/NowLoading/o.png"),
	_T("res/img/NowLoading/w.png"),
	_T("res/img/NowLoading/l.png"),
	_T("res/img/NowLoading/a.png"),
	_T("res/img/NowLoading/d.png"),
	_T("res/img/NowLoading/i.png"),
	_T("res/img/NowLoading/ns.png"),
	_T("res/img/NowLoading/g.png"),
};
const D3DXVECTOR3 CGame::INIT_TEXTURE_POS[MAX_TEXLIST] = {	// テクスチャの初期位置
	D3DXVECTOR3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, FAR_CLIP),	// 背景
};

const D3DXVECTOR2 CGame::BG_SIZE = D3DXVECTOR2((float)SCREEN_WIDTH * 2.0f, (float)SCREEN_HEIGHT * 2.0f);		// 背景サイズ

const D3DXVECTOR2 CGame::FILTER_SIZE((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
const D3DXVECTOR3 CGame::FILTER_POS((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.0f);

// フェード関連
const float CGame::FADE_POSZ = -100.0f;			// フェード用テクスチャのZ座標
const int CGame::FADEIN_TIME = 5;				// フェードイン間隔(アルファ値:0～255)
const int CGame::FADEOUT_TIME = 10;				// フェードアウト間隔(アルファ値:0～255)
const int CGame::STOP_FADEIN_TIME = 30;			// フェードイン間隔(アルファ値:0～255)
const int CGame::STOP_FADEOUT_TIME = 30;		// フェードアウト間隔(アルファ値:0～255)
const int CGame::NOWLOADING_FADEIN_TIME = 20;	// フェードイン間隔(アルファ値:0～255)
const int CGame::NOWLOADING_FADEOUT_TIME = 20;	// フェードアウト間隔(アルファ値:0～255)

const D3DXVECTOR3	CGame::CLIP_INITPOS			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// クリッピング初期位置
const D3DXVECTOR2	CGame::CLIP_SIZE			= D3DXVECTOR2(400.0f, 400.0f);			// クリッピングサイズ
const float		CGame::CLIP_SCALING_SPD			= 7.7f;									// クリッピング拡大速度
const float		CGame::CLIP_LATEST_SPD			= 0.07f;								// クリッピング最遅速度
const D3DXVECTOR2	CGame::CLIP_SIZE_JACK		= D3DXVECTOR2(600.0f, 600.0f);			// 蔦時のクリッピングサイズ
const float		CGame::CLIP_SCALING_SPD_JACK	= 7.7f;									// 蔦時のクリッピング拡大速度
const float		CGame::CLIP_LATEST_SPD_JACK		= 0.07f;								// 蔦時のクリッピング最遅速度

const float	CGame::SCROLL_EFFECT_SPD	= 0.001f;		// スクロールエフェクト移動速度

const int	CGame::WND_FILTER_ALPHA = 96;

const D3DXVECTOR3 CGame::DIRECTION_PLAYER_POS = D3DXVECTOR3(370.0f, -450.0f, 0.0f);
const D3DXVECTOR2 CGame::NOWLOADING_TEXT_SIZE = D3DXVECTOR2(71.0f * 1.5f, 82.0f * 1.5f);
const D3DXVECTOR3 CGame::NOWLOADING_TEXT_POS = D3DXVECTOR3(500.0f, -460.0f, 0.0f);
const float CGame::NOWLOADING_TEXT_INTERVAL = 30.0f * 1.5f;
const float CGame::NOWLOADING_TEXT_AMOUNT = NOWLOADING_TEXT_POS.y + 30.0f;
const float CGame::NOWLOADING_TEXT_SPD = 15.0f;

// ----- メンバ変数
// private:
CGameCamera*	CGame::m_pCamera;		// カメラ
CObject2D*		CGame::m_pDarkBG;		// 背景
CObject2D*		CGame::m_pLightBG;		// 背景
CObject2D*		CGame::m_pFilter;		// フィルター

// ----- プレイヤー　----- //
CPlayersGroup*	CGame::m_pPlayersGroup;

std::vector<CFlower*> CGame::m_listFlower;

CStage*		CGame::m_pStage;
int			CGame::m_stageID;		// 選択したステージのID

CCharacter*	CGame::m_pScrollEffectDark;	// 背景でスクロールするエフェクト
CCharacter*	CGame::m_pScrollEffectLight;	// 背景でスクロールするエフェクト

CGameStop*	CGame::m_pGameStop;
CGameOver*	CGame::m_pGameOver;
CGameClear*	CGame::m_pGameClear;

CCharacter*					CGame::m_pClipCircle;
CGame::CLIPINFO_ARRAY		CGame::m_clipInfoList;
std::vector<float>			CGame::m_clipEasingList;
std::vector<D3DXVECTOR2>	CGame::m_clearClipSizeList;

// ----- ゲームシステム
DWORD	CGame::m_phase;		// フェーズフラグ
DWORD	CGame::m_pNextScene;

HANDLE				CGame::m_hNowLoading;		// Now Loading用ハンドル
CRITICAL_SECTION	CGame::m_cs;				// クリティカルセクション
bool				CGame::m_bLoaded;			// リソースのロード完了フラグ
CCharacter*			CGame::m_pDirPlayer;		// 演出用たねぽん
CCharacter*			CGame::m_pDirTactile;		// 演出用たねぽんの触覚
LPCHARACTER_ARRAY	CGame::m_pLoadingTextes;	// Now Loadingのテキスト


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
	m_pFilter = NULL;

	m_pStage = NULL;
	m_stageID = 0;

	m_pGameStop = NULL;
	m_pGameOver = NULL;
	m_pGameClear = NULL;
	m_pPlayersGroup = NULL;

	m_phase = MAX_PHASE;
	m_pNextScene = SID_SELECT;

	m_pClipCircle = NULL;
	m_clipInfoList.reserve(100);

	m_pScrollEffectDark = NULL;
	m_pScrollEffectLight = NULL;

	m_hNowLoading = NULL;
	m_bLoaded = false;
	m_pDirPlayer = NULL;
	m_pDirTactile = NULL;

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
#ifdef _MULTI_THREAD_NOWLOADING
	// ----- スレッド準備
	m_bLoaded = false;		// リソースのロード準備
	EnterCriticalSection(&m_cs);

	// ----- Now Loadingスレッド処理開始
	m_hNowLoading = NULL;
	if(!m_hNowLoading)
		m_hNowLoading = (HANDLE)_beginthreadex(NULL, 0, NowLoading, NULL, 0, NULL);
	else
	{
		if(WaitForSingleObject(m_hNowLoading, 0) != WAIT_TIMEOUT)
		{
			CloseHandle(m_hNowLoading);
			m_hNowLoading = NULL;
		}
	}
#endif

	// ----- 次のフェーズへ
	m_phase = PHASE_LOADFADEIN;
	m_nStart = S_PHASE_INIT;

	// ----- カメラ初期化
	m_pCamera->Init();

	// ----- テクスチャ初期化
	m_pLightBG->Init(BG_SIZE, INIT_TEXTURE_POS[TL_BG_LIGHT]);		// 背景
	m_pLightBG->TranslateZ(OBJ_GAME_PRIORITIES[OL_BG_LIGHT]);
	m_pFilter->Init(FILTER_SIZE, FILTER_POS);

	// ----- 演出用たねぽん準備
	m_pDirPlayer->Init(D3DXVECTOR2(PLAYER_SIZE_X, PLAYER_SIZE_Y), DIRECTION_PLAYER_POS);
	m_pDirPlayer->UVDivision(0, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y);
	m_pDirPlayer->SetColor(D3DXVECTOR3(128, 255, 128));
	m_pDirPlayer->StartAnimation();
	m_pDirTactile->Init(m_pDirPlayer->GetSize(), m_pDirPlayer->GetPosition());
	m_pDirTactile->TranslationZ(1.0f);
	m_pDirTactile->UVDivision(0, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y);
	m_pDirTactile->SetColor(D3DXVECTOR3(128, 255, 128));
	m_pDirTactile->StartAnimation();

	// ----- テキスト準備
	int textCnt = 0;
	for(LPCHARACTER_ARRAY_IT it = m_pLoadingTextes.begin(); it != m_pLoadingTextes.end(); ++it) {
		(*it)->Init(NOWLOADING_TEXT_SIZE, NOWLOADING_TEXT_POS);
		(*it)->TranslationX(NOWLOADING_TEXT_INTERVAL * textCnt);

		++textCnt;
	}

	// ----- フェード設定
	CChangeScene::SetNormalFadeAlpha(255);

	// ----- 再生
	CGameMain::PlayBGM(BGM_GAME, DSBPLAY_LOOPING);
	
#ifdef _MULTI_THREAD_NOWLOADING
	LeaveCriticalSection(&m_cs);
#else
	// ----- ステージ初期化
	m_pStage->Init(m_stageID);

	// ----- メインリソースのロード
	MainResourceInit();

	// ----- 次のフェーズへ
	m_phase = PHASE_FADEIN;
#endif
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
	m_pFilter->Uninit();

	// ----- オブジェクト後始末
	m_pCamera->Uninit();			// カメラ

	m_pStage->Uninit();

	m_pGameStop->Uninit();
	m_pGameOver->Uninit();
	m_pGameClear->Uninit();

	m_pPlayersGroup->Uninit();

	m_pClipCircle->Uninit();

	m_pScrollEffectDark->Uninit();
	m_pScrollEffectLight->Uninit();

	// ----- BGM停止
//	CGameMain::StopBGM(BGM_GAME);

	for(std::vector<CFlower*>::iterator it = m_listFlower.begin(); it != m_listFlower.end(); ++it){
		(*it)->Uninit();
		SAFE_RELEASE((*it))
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
	if (m_pPlayersGroup->GetPlayer(m_pPlayersGroup->GetPlayNo())){
		m_pCamera->SetNextEye(m_pPlayersGroup->GetPlayer(m_pPlayersGroup->GetPlayNo())->GetPosition());
	}

	// カメラのフェイズを同期
	m_pCamera->SetPhase(m_phase);
	m_pCamera->Update();

	switch (m_phase)
	{
		// フェードイン
	case PHASE_FADEIN:
		Start();
		if(m_pFilter->FadeOutAlpha(FADEIN_TIME))
			m_phase = PHASE_START;		// 開始準備
		break;

		// 次のシーンへフェードアウト
	case PHASE_FADEOUT:
		if(m_pFilter->FadeInAlpha(FADEOUT_TIME))
		{	// 次のシーンへ
			Uninit();							// 後始末
			CGameMain::SetScene(m_pNextScene);	// リザルトへ
		}
		break;
	case PHASE_START:
		Start();
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

	case PHASE_STOPFADEIN:
		m_pFilter->FadeOutAlpha(STOP_FADEIN_TIME);
		if(m_pFilter->GetAlpha() <= 0) {
			m_phase = PHASE_START;
			m_pFilter->SetAlpha(0);
		}
		break;
	case PHASE_STOPFADEOUT:
		m_pFilter->FadeInAlpha(STOP_FADEOUT_TIME);
		if(m_pFilter->GetAlpha() > WND_FILTER_ALPHA) {
			m_phase = PHASE_STOP;
			m_pFilter->SetAlpha(WND_FILTER_ALPHA);
		}
		break;
		
	case PHASE_OVERFADEIN:
		m_pFilter->FadeOutAlpha(STOP_FADEIN_TIME);
		if(m_pFilter->GetAlpha() <= 0) {
			m_phase = PHASE_START;
			m_pFilter->SetAlpha(0);
		}
		break;
	case PHASE_OVERFADEOUT:
		m_pFilter->FadeInAlpha(STOP_FADEOUT_TIME);
		if(m_pFilter->GetAlpha() > WND_FILTER_ALPHA) {
			m_phase = PHASE_OVER;
			m_pFilter->SetAlpha(WND_FILTER_ALPHA);
		}
		break;
		
	case PHASE_CLEARFADEIN:
		//if(m_pFilter->FadeOutAlpha(FADEIN_TIME)) {
		//	m_phase = PHASE_CLEAR;
		//}
		break;
	case PHASE_CLEARFADEOUT:
		if(m_pFilter->FadeInAlpha(FADEOUT_TIME)) {
			m_phase = PHASE_CLEAR;
			m_pFilter->SetAlpha(0);

			// オブジェクトのプライオリティ調整
			m_pStage->GetLayoutBlock(0)->TranslateZ(OBJ_CLEAR_PRIORITIES[OL_LB_DARK]);
			m_pStage->GetLayoutBlock(1)->TranslateZ(OBJ_CLEAR_PRIORITIES[OL_LB_LIGHT]);
			m_pDarkBG->TranslateZ(OBJ_CLEAR_PRIORITIES[OL_BG_DARK]);
			m_pLightBG->TranslateZ(OBJ_CLEAR_PRIORITIES[OL_BG_LIGHT]);
			m_pScrollEffectDark->TranslateZ(OBJ_CLEAR_PRIORITIES[OL_SCROLL_DARK]);
			m_pScrollEffectLight->TranslateZ(OBJ_CLEAR_PRIORITIES[OL_SCROLL_LIGHT]);
			for (std::vector<CFlower*>::iterator it = m_listFlower.begin(); it != m_listFlower.end(); ++it) {
				if((*it)->GetStageType() == CStage::ST_DARK) {
					switch((*it)->GetType()) {
						case CFlower::TYPE_FLOWER:
							(*it)->TranslateZ(OBJ_CLEAR_PRIORITIES[OL_FLOWER_DARK]);
							break;
						case CFlower::TYPE_JACK:
							(*it)->TranslateZ(OBJ_CLEAR_PRIORITIES[OL_JACK_DARK]);
							break;
						case CFlower::TYPE_STONE:
							(*it)->TranslateZ(OBJ_CLEAR_PRIORITIES[OL_STONE_DARK]);
							break;
					}
				} else {
					switch((*it)->GetType()) {
						case CFlower::TYPE_FLOWER:
							(*it)->TranslateZ(OBJ_CLEAR_PRIORITIES[OL_FLOWER_LIGHT]);
							break;
						case CFlower::TYPE_JACK:
							(*it)->TranslateZ(OBJ_CLEAR_PRIORITIES[OL_JACK_LIGHT]);
							break;
						case CFlower::TYPE_STONE:
							(*it)->TranslateZ(OBJ_CLEAR_PRIORITIES[OL_STONE_LIGHT]);
							break;
					}
				}
			}
		}
		break;

	case PHASE_NOWLOADING:
	{
		// ----- Now Loading演出
		// たねぽんのアニメーション
		m_pDirPlayer->FrameAnimation(0, 11, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.1f);
		m_pDirTactile->FrameAnimation(0, 11, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.1f);

		// 文字のアニメーション
		static unsigned int animeText = 0;
		static float animeTextSpd = NOWLOADING_TEXT_SPD;
		static bool bAnimeReverse = false;
		static bool bAnimeTerm = false;
		static double animeWaitTime = 0.0;
		
		if(animeText >= m_pLoadingTextes.size()) {
			if(CTimer::GetTime() - animeWaitTime > 0.3) {
				animeText = 0;
			}
		} else {
			if(bAnimeTerm) {
				if(CTimer::GetTime() - animeWaitTime > 0.05)
					bAnimeTerm = false;
			} else {
				m_pLoadingTextes[animeText]->TranslationY(animeTextSpd);
			}

			if(bAnimeReverse) {
				if(m_pLoadingTextes[animeText]->GetPosY() < NOWLOADING_TEXT_POS.y) {
					m_pLoadingTextes[animeText]->TranslateY(NOWLOADING_TEXT_POS.y);
					animeTextSpd = -animeTextSpd;
					bAnimeReverse = false;
					++animeText;
					bAnimeTerm = true;
					animeWaitTime = CTimer::GetTime();
				}
			} else {
				if(m_pLoadingTextes[animeText]->GetPosY() > NOWLOADING_TEXT_AMOUNT) {
					m_pLoadingTextes[animeText]->TranslateY(NOWLOADING_TEXT_AMOUNT);
					animeTextSpd = -animeTextSpd;
					bAnimeReverse = true;
				}
			}
		}

		if(m_bLoaded) {
			m_phase = PHASE_MAINRESOURCEINIT;
		}

		break;
	}

	case PHASE_LOADFADEIN:
		if(m_pFilter->FadeOutAlpha(NOWLOADING_FADEIN_TIME))
			m_phase = PHASE_NOWLOADING;
		break;
	case PHASE_LOADFADEOUT:
		if(m_pFilter->FadeInAlpha(NOWLOADING_FADEOUT_TIME))
			m_phase = PHASE_FADEIN;
		break;

	case PHASE_MAINRESOURCEINIT:
		MainResourceInit();
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

	switch (m_phase)
	{
		// フェードイン・アウト
	case PHASE_FADEIN:
		DrawStart();
		m_pFilter->DrawScreenAlpha();
		break;
		
	case PHASE_FADEOUT:
		DrawClear();
		m_pFilter->DrawScreenAlpha();
		break;

	case PHASE_START:
		DrawStart();
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

	case PHASE_STOPFADEIN:
	case PHASE_STOPFADEOUT:
	case PHASE_OVERFADEIN:
	case PHASE_OVERFADEOUT:
	case PHASE_CLEARFADEIN:
	case PHASE_CLEARFADEOUT:
		DrawMain();
		m_pFilter->DrawScreenAlpha();
		break;

	case PHASE_NOWLOADING:
#ifdef _MULTI_THREAD_NOWLOADING
		EnterCriticalSection(&m_cs);
#endif
		m_pLightBG->Draw();
		m_pDirPlayer->DrawAlpha();
		m_pDirTactile->DrawAlpha();
		for(LPCHARACTER_ARRAY_IT it = m_pLoadingTextes.begin(); it != m_pLoadingTextes.end(); ++it) {
			(*it)->DrawAlpha();
		}
#ifdef _MULTI_THREAD_NOWLOADING
		LeaveCriticalSection(&m_cs);
#endif
		break;

	case PHASE_LOADFADEIN:
	case PHASE_LOADFADEOUT:
	case PHASE_MAINRESOURCEINIT:
		m_pLightBG->Draw();
		m_pDirPlayer->DrawAlpha();
		m_pDirTactile->DrawAlpha();
		for(LPCHARACTER_ARRAY_IT it = m_pLoadingTextes.begin(); it != m_pLoadingTextes.end(); ++it) {
			(*it)->DrawAlpha();
		}
		m_pFilter->DrawScreenAlpha();
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : Now Loading
//	Description : Now Loading
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
unsigned int CGame::NowLoading(void* arg)
{
//	EnterCriticalSection(&m_cs);

	// ステージ初期化
	m_pStage->Init(m_stageID);

	// ----- リソースのロード完了
	m_bLoaded = true;

//	LeaveCriticalSection(&m_cs);

	_endthreadex(0);	// スレッド終了通知
	return 0;
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
	m_pFilter = CObject2D::Create(TEX_FILENAME[TL_FILTER]);
	if (m_pFilter == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::Filterの生成に失敗しました。"), _T("error"), MB_OK);
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
	m_pScrollEffectDark = CCharacter::Create(TEX_FILENAME[TL_SCROLL_EFFECT]);
	if (m_pScrollEffectDark == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::ScrollEffectDarkの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}
	
	// 背景でスクロールするエフェクト
	m_pScrollEffectLight = CCharacter::Create(TEX_FILENAME[TL_SCROLL_EFFECT]);
	if (m_pScrollEffectLight == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::ScrollEffectLightの生成に失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}

	// 演出用たねぽん
	m_pDirPlayer = CCharacter::Create(TEX_FILENAME[TL_PLAYER_0]);
	if (m_pDirPlayer == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::DirectionPlayerの生成に失敗しました。"), _T("error"), MB_OK);
#endif
	}
	m_pDirTactile = CCharacter::Create(CPlayer::TACTILE_TEX_FILENAME[PLAYER_NORMAL]);
	if (m_pDirTactile == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::DirectionTactileの生成に失敗しました。"), _T("error"), MB_OK);
#endif
	}

	// Now Loading 文字列
	for(int i = 0; i < 10; ++i) {		// Now Loading の文字数
		CCharacter* pText = NULL;
		if(i < 4) {
			pText = CCharacter::Create(TEX_FILENAME[TL_TEXT_N_LARGE + i]);
			if (pText == NULL) {
#ifdef _DEBUG_MESSAGEBOX
				::MessageBox(NULL, _T("CGame::NowLoadingTextの生成に失敗しました。"), _T("error"), MB_OK);
#endif
			}
		} else if(i > 4) {
			pText = CCharacter::Create(TEX_FILENAME[TL_TEXT_N_LARGE + i - 1]);
			if (pText == NULL) {
#ifdef _DEBUG_MESSAGEBOX
				::MessageBox(NULL, _T("CGame::NowLoadingTextの生成に失敗しました。"), _T("error"), MB_OK);
#endif
			}
		} else {	// 2回目の o
			pText = CCharacter::Create(TEX_FILENAME[TL_TEXT_O]);
			if (pText == NULL) {
#ifdef _DEBUG_MESSAGEBOX
				::MessageBox(NULL, _T("CGame::NowLoadingTextの生成に失敗しました。"), _T("error"), MB_OK);
#endif
			}
		}

		m_pLoadingTextes.push_back(pText);
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

	InitializeCriticalSection(&m_cs);	// クリティカルセクション準備

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
	SAFE_RELEASE(m_pFilter);

	SAFE_RELEASE(m_pStage);		// ブロック

	SAFE_RELEASE(m_pGameStop);
	SAFE_RELEASE(m_pGameOver);
	SAFE_RELEASE(m_pGameClear);

	SAFE_DELETE(m_pPlayersGroup);

	SAFE_RELEASE(m_pClipCircle);
	SAFE_RELEASE(m_pScrollEffectDark);
	SAFE_RELEASE(m_pScrollEffectLight);

	SAFE_RELEASE(m_pDirPlayer);
	SAFE_RELEASE(m_pDirTactile);

	for(LPCHARACTER_ARRAY_IT it = m_pLoadingTextes.begin(); it != m_pLoadingTextes.end(); ++it) {
		SAFE_RELEASE((*it));
	}

	// ----- クリティカルセクション破棄
	DeleteCriticalSection(&m_cs);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 開始
//	Description : ゲーム本編の開始処理
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::Start()
{
	if(GetTrgKey(DIK_RETURN))
		m_phase = PHASE_MAIN;
	switch(m_nStart)
	{
	case S_PHASE_INIT:

		m_pStage->GetLayoutBlock(0)->SetAlpha(0);
		m_pStage->GetLayoutBlock(1)->SetAlpha(0);

		m_pDirPlayer->UVDivision(48,PLAYER_ANIME_SIZE_X,PLAYER_ANIME_SIZE_Y);
		m_pDirTactile->UVDivision(48,PLAYER_ANIME_SIZE_X,PLAYER_ANIME_SIZE_Y);
		m_pDirPlayer->TranslateX(CMapData::GetStartPoint().x);
		m_pDirPlayer->TranslateY(CMapData::GetStartPoint().y + 300);
		m_pDirPlayer->ScaleX(-m_pDirPlayer->GetScaleX());
		m_pDirTactile->ScaleX(-m_pDirTactile->GetScaleX());
		m_nStart = S_PHASE_START;
		
		break;
	case S_PHASE_START:
		m_pDirPlayer->TranslationY(-2);

		// ----- スクロールエフェクト移動処理
		m_pScrollEffectDark->UVScroll(-SCROLL_EFFECT_SPD, -abs(CMapData::GetStartPoint().y - m_pDirPlayer->GetPosY()) * 0.01f * SCROLL_EFFECT_SPD);
		m_pScrollEffectLight->UVScroll(SCROLL_EFFECT_SPD, abs(CMapData::GetStartPoint().y - m_pDirPlayer->GetPosY()) * 0.01f * SCROLL_EFFECT_SPD);

		if(m_pDirPlayer->GetPosY() < CMapData::GetStartPoint().y)
			m_nStart = S_PHASE_FLOWER;
		break;
	case S_PHASE_FLOWER:{

		m_pDirPlayer->SetAlpha(0);
		m_pDirTactile->SetAlpha(0);

		CreateFlower(D3DXVECTOR3(CMapData::GetStartPoint().x,CMapData::GetStartPoint().y,0),D3DXVECTOR3(0,1,0));

		TCLIPINFO clipInfo;
		clipInfo.type = CT_FLOWER;
		clipInfo.pos  = D3DXVECTOR3(CMapData::GetStartPoint().x,CMapData::GetStartPoint().y,0);
		clipInfo.size = D3DXVECTOR2(0.0f, 0.0f);
		m_clipInfoList.push_back(clipInfo);

		float easing = CLIP_SCALING_SPD;		// 減速
		m_clipEasingList.push_back(easing);

		m_nStart = S_PHASE_STAGE;

		break;
	}
	case S_PHASE_STAGE:
		if(m_pStage->GetLayoutBlock(0)->GetAlpha() < 255){
			m_pStage->GetLayoutBlock(0)->SetAlpha((int)m_pStage->GetLayoutBlock(0)->GetAlpha() + 1);
			m_pStage->GetLayoutBlock(1)->SetAlpha((int)m_pStage->GetLayoutBlock(1)->GetAlpha() + 1);
		}else
			m_nStart = S_PHASE_GOMAIN;

		// ----- スクロールエフェクト移動処理
		m_pScrollEffectDark->UVScroll(-SCROLL_EFFECT_SPD,0);
		m_pScrollEffectLight->UVScroll(SCROLL_EFFECT_SPD,0);
		break;
	case S_PHASE_GOMAIN:
		m_pDirPlayer->ScaleX(-m_pDirPlayer->GetScaleX());
		m_pDirTactile->ScaleX(-m_pDirTactile->GetScaleX());
		for(std::vector<CFlower*>::iterator it = m_listFlower.begin(); it != m_listFlower.end(); ++it)
		{
			(*it)->SetPhase(FLOWER_PHASE_WAIT);
		}
		m_phase = PHASE_MAIN;
		
		break;		
	}	
	for(std::vector<CFlower*>::iterator it = m_listFlower.begin(); it != m_listFlower.end(); ++it)
	{
		(*it)->Update();
	}
	
	// ----- 背景の位置を調整
	D3DXVECTOR3 cameraPos = m_pCamera->GetEye();
	m_pLightBG->TranslateX(cameraPos.x);
	m_pLightBG->TranslateY(cameraPos.y);
	m_pDarkBG->TranslateX(cameraPos.x);
	m_pDarkBG->TranslateY(cameraPos.y);

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
	if (GetTrgKey(DIK_RSHIFT)) {
		CGameMain::PlaySE(SE_PAUSE);
		CGameMain::StopBGM(BGM_GAME);
		m_phase = PHASE_STOPFADEOUT;
	}

	// ゲームオーバ
	if (m_pPlayersGroup->GetOver()){
		CGameMain::PlaySE(SE_GAMEOVER);
		CGameMain::StopBGM(BGM_GAME);
		m_phase = PHASE_OVERFADEOUT;
	}

	// ゲームクリア
	bool Clear = true;
	for (int i = 0; i < m_pStage->GetMaxFieldBlock(); i++){
		CFieldBlock* pFieldBlock = m_pStage->GetFieldBlock(i);
		if (pFieldBlock->GetType() == CMapData::BT_CLEAR){
			if (pFieldBlock->GetFloawerNum() <= 0) {
				Clear = false;
				break;
			}
		}
	}

	// ゲームクリア演出開始
	if (Clear || GetTrgKey(DIK_Q)) {	// デバッグ用
		CGameMain::PlaySE(SE_GAMECLEAR);
		CGameMain::StopBGM(BGM_GAME);
		CGameMain::EnableStageClear(m_stageID);
		m_phase = PHASE_CLEARFADEOUT;

		// カメラを引く距離を算出
		float widthDist = CMapData::GetRightWallX() - CMapData::GetLeftWallX();
		float heightDist = CMapData::GetTopWallY() - CMapData::GetBottomWallY();
		if (widthDist > heightDist)
			m_pGameClear->SetDirectionDistance(widthDist);
		else
			m_pGameClear->SetDirectionDistance(heightDist);

		// カメラセット
		D3DXVECTOR2 cameraPos(CMapData::GetRightWallX() - widthDist * 0.5f, CMapData::GetTopWallY() - heightDist * 0.5f);
		m_pGameClear->SetCameraStartPos(cameraPos);
		m_pGameClear->SetCamera(m_pCamera);

		// ステージ全体を緑化するクリップの準備
		for(int i = 0; i < m_pStage->GetMaxClearBlock(); ++i) {
			for(int j = 0; j < m_pStage->GetMaxFieldBlock(); ++j) {
				CFieldBlock* pBlock = m_pStage->GetFieldBlock(j);
				if(pBlock->GetType() == CMapData::BT_CLEAR) {
					float x = 0.0f;
					float y = 0.0f;
					for(int k = 0; k < pBlock->GetElementNum(); ++k) {
						CCharacter* pElem = pBlock->GetElement(k);
						x += pElem->GetPosX();
						y += pElem->GetPosY();
					}
					x /= pBlock->GetElementNum();
					y /= pBlock->GetElementNum();

					TCLIPINFO clipInfo;
					clipInfo.pos = D3DXVECTOR3(x, y, 0.0f);
					clipInfo.size = D3DXVECTOR2(0.0f, 0.0f);
					m_clipInfoList.push_back(clipInfo);
					m_clearClipSizeList.push_back(clipInfo.size);
				}
			}
		}
	}

	// リスト内全部更新
	m_pStage->Update();

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
				D3DXVECTOR3 pos = D3DXVECTOR3(m_pPlayersGroup->GetPlayer(i)->GetLastColLinePos().x,m_pPlayersGroup->GetPlayer(i)->GetLastColLinePos().y,m_pPlayersGroup->GetPlayer(i)->GetPosZ() - 1);
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
						clipInfo.type = CT_FLOWER;
						clipInfo.pos  = pos;
						clipInfo.size = D3DXVECTOR2(0.0f, 0.0f);
						m_clipInfoList.push_back(clipInfo);

						float easing = CLIP_SCALING_SPD;		// 減速
						m_clipEasingList.push_back(easing);

						break;
					}
					case PLAYER_JACK:
					{
						for(int i = 0;i < m_pStage->GetMaxFieldBlock();i++){
							if(m_pStage->GetFieldBlock(i)->GetType() == CMapData::BT_NORMAL){
								m_pStage->GetFieldBlock(i)->SetElement(CreateJack(pos,dir));
								
								TCLIPINFO clipInfo;
								clipInfo.type = CT_JACK;
								clipInfo.pos  = pos;
								clipInfo.size = D3DXVECTOR2(0.0f, 0.0f);
								m_clipInfoList.push_back(clipInfo);

								float easing = CLIP_SCALING_SPD_JACK;		// 減速
								m_clipEasingList.push_back(easing);

								break;
							}
						}
						
						break;
					}
					case PLAYER_STONE:
					{
						for(int i = 0;i < m_pStage->GetMaxFieldBlock();i++){
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

	for(std::vector<CFlower*>::iterator it = m_listFlower.begin(); it != m_listFlower.end(); ++it)
	{
		(*it)->Update();

		// 花状態のときに種を増やす
		if ((*it)->GetPhase() == FLOWER_PHASE_FLOWER){
			D3DXVECTOR3 move;
//			D3DXVECTOR3 pos1;
//			D3DXVec3Cross(&move, &m_listFlower[i]->GetPosition(), &D3DXVECTOR3(0, 0, 1));
//			D3DXVec3Normalize(&move, &move);
//			pos1 = m_listFlower[i]->GetPosition() + (move * (FLOWER_SIZE_X / 2));
//			pos1.z = pos1.z - 10;
			D3DXVECTOR3 pos2;
			D3DXVec3Cross(&move, &(*it)->GetPosition(), &D3DXVECTOR3(0, 0, -1));
			D3DXVec3Normalize(&move, &move);
			pos2 = (*it)->GetPosition() + (move * (FLOWER_SIZE_X / 2));
			pos2.z = pos2.z + 0.1f;

//			if(m_pPlayersGroup->GetGroupSize() < 8)
			//	m_pPlayersGroup->AddPlayer(pos1);
			if(m_pPlayersGroup->GetGroupSize() < 9)
				m_pPlayersGroup->AddPlayer(pos2);
			(*it)->SetPhase(FLOWER_PHASE_WAIT);
		}
	}

		// プレイヤーの更新
	m_pPlayersGroup->Update();

	// ----- 緑化クリッピング領域拡大
	int listCnt = 0;
	for (std::vector<float>::iterator it = m_clipEasingList.begin(); it != m_clipEasingList.end(); ++it, ++listCnt) {
		switch(m_clipInfoList[listCnt].type) {
			case CT_FLOWER:
				if (m_clipInfoList[listCnt].size.x >= CLIP_SIZE.x)
					continue;

				/*		m_clipEasingList[i] += CLIP_SCALING_SPD;
				m_clipInfoList[i].size.x += m_clipEasingList[i];
				m_clipInfoList[i].size.y += m_clipEasingList[i];
				if (m_clipInfoList[i].size.x > CLIP_SIZE.x)
					m_clipInfoList[i].size = CLIP_SIZE;
		*/

				if ((*it) > CLIP_LATEST_SPD)
					(*it) -= CLIP_LATEST_SPD;
				m_clipInfoList[listCnt].size.x += (*it);
				m_clipInfoList[listCnt].size.y += (*it);
				if (m_clipInfoList[listCnt].size.x > CLIP_SIZE.x)
					m_clipInfoList[listCnt].size = CLIP_SIZE;

				break;
				
			case CT_JACK:
				if (m_clipInfoList[listCnt].size.x >= CLIP_SIZE_JACK.x)
					continue;

				if ((*it) > CLIP_LATEST_SPD_JACK)
					(*it) -= CLIP_LATEST_SPD_JACK;
				m_clipInfoList[listCnt].size.x += (*it);
				m_clipInfoList[listCnt].size.y += (*it);
				if (m_clipInfoList[listCnt].size.x > CLIP_SIZE_JACK.x)
					m_clipInfoList[listCnt].size = CLIP_SIZE_JACK;

				break;
		}
	}

	// ----- スクロールエフェクト移動処理
	m_pScrollEffectDark->UVScroll(-SCROLL_EFFECT_SPD, 0.0f);
	m_pScrollEffectLight->UVScroll(SCROLL_EFFECT_SPD, 0.0f);
	
	// ----- 背景の位置を調整
	D3DXVECTOR3 cameraPos = m_pCamera->GetEye();
	m_pLightBG->TranslateX(cameraPos.x);
	m_pLightBG->TranslateY(cameraPos.y);
	m_pDarkBG->TranslateX(cameraPos.x);
	m_pDarkBG->TranslateY(cameraPos.y);


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
//	Name        : 開始
//	Description : ゲーム本編の開始描画
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::DrawStart(){
	
	// ----- クリッピング領域設定
	CGraphics::StencilRegionBegin();

	for (CLIPINFO_ARRAY_IT it = m_clipInfoList.begin(); it != m_clipInfoList.end(); ++it) {
		m_pClipCircle->Translate((*it).pos);
		m_pClipCircle->Resize ((*it).size);
		m_pClipCircle->DrawAlpha();
	}

	CGraphics::StencilRegionEnd();

	// ----- クリッピング対象を描画
	CGraphics::StencilDrawBegin();

	// ステージ描画
	m_pDarkBG->Draw();		// 背景
	m_pStage->DrawLayoutBlock(0);
	m_pScrollEffectDark->DrawAlpha();
	
	for (std::vector<CFlower*>::iterator it = m_listFlower.begin(); it != m_listFlower.end(); ++it)
	{
		if((*it)->GetStageType() == CStage::ST_DARK)
			(*it)->DrawAlpha();
	}

	m_pDirPlayer->DrawAlpha();
	m_pDirTactile->TranslateX(m_pDirPlayer->GetPosX());
	m_pDirTactile->TranslateY(m_pDirPlayer->GetPosY());
	m_pDirTactile->DrawAlpha();

	CGraphics::StencilDrawEnd();
	
	// ----- 通常描画
	m_pLightBG->Draw();		// 背景
	m_pStage->DrawLayoutBlock(1);
	m_pScrollEffectLight->DrawAlpha();
	
	// 花の描画
	for (std::vector<CFlower*>::iterator it = m_listFlower.begin(); it != m_listFlower.end(); ++it)
	{
		if((*it)->GetStageType() == CStage::ST_LIGHT)
			(*it)->DrawAlpha();
	}
	
	// プレイヤー描画
	m_pPlayersGroup->PlayersTranslateZ(OBJ_GAME_PRIORITIES[OL_PLAYER_LIGHT]);
	m_pPlayersGroup->TactilesTranslateZ(OBJ_GAME_PRIORITIES[OL_TACTILE_LIGHT]);
	m_pPlayersGroup->Draw();
	
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : メイン
//	Description : ゲーム本編のメイン描画
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::DrawMain()
{
	// ----- クリッピング領域設定
	CGraphics::StencilRegionBegin();

	for (CLIPINFO_ARRAY_IT it = m_clipInfoList.begin(); it != m_clipInfoList.end(); ++it) {
		m_pClipCircle->Translate((*it).pos);
		m_pClipCircle->Resize ((*it).size);
		m_pClipCircle->DrawAlpha();
	}

	CGraphics::StencilRegionEnd();

	// ----- クリッピング対象を描画
	CGraphics::StencilDrawBegin();

	// ステージ描画
	m_pDarkBG->Draw();		// 背景
	m_pStage->DrawLayoutBlock(0);
	m_pScrollEffectDark->DrawAlpha();
	
	for (std::vector<CFlower*>::iterator it = m_listFlower.begin(); it != m_listFlower.end(); ++it)
	{
		if((*it)->GetStageType() == CStage::ST_DARK)
			(*it)->DrawAlpha();
	}

	if(m_phase != PHASE_CLEAR) {
		m_pPlayersGroup->PlayersTranslateZ(OBJ_GAME_PRIORITIES[OL_PLAYER_DARK]);
		m_pPlayersGroup->TactilesTranslateZ(OBJ_GAME_PRIORITIES[OL_TACTILE_DARK]);
	} else {
		m_pPlayersGroup->PlayersTranslateZ(OBJ_CLEAR_PRIORITIES[OL_PLAYER_DARK]);
		m_pPlayersGroup->TactilesTranslateZ(OBJ_CLEAR_PRIORITIES[OL_TACTILE_DARK]);
	}
	m_pPlayersGroup->Draw();

	CGraphics::StencilDrawEnd();
	
	// ----- 通常描画
	m_pLightBG->Draw();		// 背景
	m_pStage->DrawLayoutBlock(1);
	m_pScrollEffectLight->DrawAlpha();
	
	// 花の描画
	for (std::vector<CFlower*>::iterator it = m_listFlower.begin(); it != m_listFlower.end(); ++it)
	{
		if((*it)->GetStageType() == CStage::ST_LIGHT)
			(*it)->DrawAlpha();
	}

	// プレイヤー描画
	if(m_phase != PHASE_CLEAR) {
		m_pPlayersGroup->PlayersTranslateZ(OBJ_GAME_PRIORITIES[OL_PLAYER_LIGHT]);
		m_pPlayersGroup->TactilesTranslateZ(OBJ_GAME_PRIORITIES[OL_TACTILE_LIGHT]);
	} else {
		m_pPlayersGroup->PlayersTranslateZ(OBJ_CLEAR_PRIORITIES[OL_PLAYER_LIGHT]);
		m_pPlayersGroup->TactilesTranslateZ(OBJ_CLEAR_PRIORITIES[OL_TACTILE_LIGHT]);
	}
	m_pPlayersGroup->Draw();
	
#ifdef _DEBUG
	static bool drawFlg = false;
	if (GetAsyncKeyState('C') & 1)
		drawFlg = !drawFlg;
	if (drawFlg) {
		for(int i = 0; i < m_pStage->GetMaxFieldBlock(); ++i) {
			CFieldBlock* pBlock = m_pStage->GetFieldBlock(i);
			for(int j = 0; j < pBlock->GetElementNum(); ++j) {
				pBlock->GetElement(j)->TranslateZ(OBJ_GAME_PRIORITIES[OL_LB_DARK] - 1.0f);
			}
		}
		m_pStage->DrawFieldBlock();
	}
#endif
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : ゲーム本編のリソースを初期化する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::MainResourceInit()
{
	m_pNextScene = SID_SELECT;

	// ----- テクスチャ初期化
	m_pDarkBG->Init(BG_SIZE, INIT_TEXTURE_POS[TL_BG_DARK]);			// 背景
	m_pClipCircle->Init(CLIP_SIZE, CLIP_INITPOS);

	m_pPlayersGroup->Init();
	m_pPlayersGroup->SetStage(m_pStage);

	m_pGameStop->Init();
	m_pGameOver->Init();
	m_pGameClear->Init(m_stageID);

	// ----- クリッピング設定初期化
	m_clipInfoList.clear();
	m_clipEasingList.clear();
	m_clearClipSizeList.clear();
	
	// ----- スクロールエフェクト設定初期化
	D3DXVECTOR2 size = m_pStage->GetLayoutBlock(1)->GetSize();
	D3DXVECTOR3 pos  = m_pStage->GetLayoutBlock(1)->GetPosition();
	m_pScrollEffectDark->Init(size, pos);
	m_pScrollEffectLight->Init(size, pos);
	m_pScrollEffectDark->ScaleX(-1.0f);
	m_pScrollEffectDark->ScaleY(-1.0f);

	// ----- プライオリティ調整
	m_pScrollEffectLight->TranslateZ(OBJ_GAME_PRIORITIES[OL_SCROLL_LIGHT]);
	m_pStage->GetLayoutBlock(1)->TranslateZ(OBJ_GAME_PRIORITIES[OL_LB_LIGHT]);
	m_pDarkBG->TranslateZ(OBJ_GAME_PRIORITIES[OL_BG_DARK]);
	m_pScrollEffectDark->TranslateZ(OBJ_GAME_PRIORITIES[OL_SCROLL_DARK]);
	m_pStage->GetLayoutBlock(0)->TranslateZ(OBJ_GAME_PRIORITIES[OL_LB_DARK]);

	m_pScrollEffectDark->SetColor(D3DXVECTOR3(200.0f, 200.0f, 200.0f));
	m_pScrollEffectLight->SetColor(D3DXVECTOR3(255.0f, 206.0f, 147.0f));

	m_phase = PHASE_LOADFADEOUT;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 一時停止
//	Description : ゲーム本編の一時停止処理
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGame::Stop()
{
	m_pFilter->Update();

	m_pGameStop->Update();

	if (m_pGameStop->GetPhase() == GAME_STOP_PHASE_END){
		switch (m_pGameStop->GetGo())
		{
		case GO_GAME:
			m_phase = PHASE_STOPFADEIN;
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

	m_pFilter->DrawScreenAlpha();

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
	m_pFilter->Update();

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

	m_pFilter->DrawScreenAlpha();

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
	m_pGameClear->Update(m_pDarkBG, m_pLightBG, &m_clearClipSizeList);

	for(unsigned int i = m_clipInfoList.size() - m_clearClipSizeList.size(), j = 0; i < m_clipInfoList.size(); ++i, ++j) {
		m_clipInfoList[i].size = m_clearClipSizeList[j];
	}

	// ----- たねぽんのクリア演出
	m_pPlayersGroup->ClearDirection();

	if (m_pGameClear->GetPhase() == CGameClear::PHASE_END){
		switch (m_pGameClear->GetGo())
		{
		case 0:
			m_phase = PHASE_FADEOUT;
			m_pNextScene = SID_GAME;
			++m_stageID;
			if(m_stageID > CMapData::MAX_STAGEID)
				m_stageID = CMapData::ID_STAGE1;
			break;
		case 1:
			m_phase = PHASE_FADEOUT;
			m_pNextScene = SID_SELECT;
			break;
		}
		m_pGameClear->Init(m_stageID);
	}
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
	// ----- 緑化前ステージ
	pos.z = OBJ_GAME_PRIORITIES[OL_FLOWER_DARK];

	CFlower* flower;
	flower = CFlower::Create(TEX_FILENAME[TL_FLOWER_0]);
	if (flower == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::flowerの生成に失敗しました。"), _T("error"), MB_OK);
#endif
	}
	flower->Init(pos, dir, TEX_FILENAME[TL_FLOWER_1]);
	flower->SetStageType(CStage::ST_DARK);
	flower->SetType(CFlower::TYPE_FLOWER);

	m_listFlower.push_back(flower);
	
	// ----- 緑化後ステージ
	pos.z = OBJ_GAME_PRIORITIES[OL_FLOWER_LIGHT];

	flower = CFlower::Create(TEX_FILENAME[TL_FLOWER_0]);
	if (flower == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::flowerの生成に失敗しました。"), _T("error"), MB_OK);
#endif
	}
	flower->Init(pos, dir, TEX_FILENAME[TL_FLOWER_1]);
	flower->SetStageType(CStage::ST_LIGHT);
	flower->SetType(CFlower::TYPE_FLOWER);

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
	// ----- 緑化前ステージ
	pos.z = OBJ_GAME_PRIORITIES[OL_JACK_DARK];

	CJack* flower;
	flower = CJack::Create(TEX_FILENAME[TL_JACK_0]);
	if (flower == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::Jackの生成に失敗しました。"), _T("error"), MB_OK);
#endif
	}
	flower->Init(pos, dir);
	flower->SetStageType(CStage::ST_DARK);
	flower->SetType(CFlower::TYPE_JACK);

	m_listFlower.push_back(flower);

	// ----- 緑化後ステージ
	pos.z = OBJ_GAME_PRIORITIES[OL_JACK_LIGHT];

	flower = CJack::Create(TEX_FILENAME[TL_JACK_0]);
	if (flower == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::Jackの生成に失敗しました。"), _T("error"), MB_OK);
#endif
	}
	flower->Init(pos, dir);
	flower->SetStageType(CStage::ST_LIGHT);
	flower->SetType(CFlower::TYPE_JACK);

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
	// ----- 緑化前ステージ
	pos.z = OBJ_GAME_PRIORITIES[OL_STONE_DARK];

	CStone* flower;
	flower = CStone::Create(TEX_FILENAME[TL_STONE_0]);
	if (flower == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::Stoneの生成に失敗しました。"), _T("error"), MB_OK);
#endif
	}
	flower->Init(pos,dir);
	flower->SetStageType(CStage::ST_DARK);
	flower->SetType(CFlower::TYPE_STONE);

	m_listFlower.push_back(flower);
	
	// ----- 緑化後ステージ
	pos.z = OBJ_GAME_PRIORITIES[OL_STONE_LIGHT];

	flower = CStone::Create(TEX_FILENAME[TL_STONE_0]);
	if (flower == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CGame::Stoneの生成に失敗しました。"), _T("error"), MB_OK);
#endif
	}
	flower->Init(pos,dir);
	flower->SetStageType(CStage::ST_LIGHT);
	flower->SetType(CFlower::TYPE_STONE);

	m_listFlower.push_back(flower);

	return flower->GetCol();
}


//========================================================================================
//	End of File
//========================================================================================