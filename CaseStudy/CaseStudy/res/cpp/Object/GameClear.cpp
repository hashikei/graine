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
#include "../../h/System/Timer.h"
#include "../../h/Scene/GameMain.h"
#include "../../h/Scene/Game.h"
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
	_T("res/img/GameScene/Popup/WindowS.png"),		// ウィンドウテクスチャファイル名
	_T("res/img/GameScene/Popup/Next.png"),			// ボタンテクスチャファイル名
	_T("res/img/GameScene/Popup/Select.png"),		// ボタンテクスチャファイル名
	_T("res/img/Fade.jpg"),
	_T("res/img/GameScene/Popup/GameClear.png"),
};

const D3DXVECTOR2 CGameClear::FILTER_SIZE((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
const D3DXVECTOR3 CGameClear::FILTER_POS((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.0f);

const D3DXVECTOR2 CGameClear::W_0_DEFAULET_SIZE		= D3DXVECTOR2(330,351);
const D3DXVECTOR3 CGameClear::W_0_DEFAULET_POS		= D3DXVECTOR3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.0f);

const D3DXVECTOR2 CGameClear::TEXT_SIZE(399.0f * 0.6f - 5.0f, 109.0f * 0.6f);
const D3DXVECTOR3 CGameClear::TEXT_POS((float)SCREEN_WIDTH * 0.5f, 240.0f, 0.0f);

const float CGameClear::B_0_POS_INTERVAL_Y = 40;

const D3DXVECTOR2 CGameClear::B_0_DEFAULET_SIZE		= D3DXVECTOR2(200,73);
const D3DXVECTOR3 CGameClear::B_0_DEFAULET_POS		=  D3DXVECTOR3(SCREEN_WIDTH / 2 - 5.0f,
														SCREEN_HEIGHT / 2 - B_0_POS_INTERVAL_Y + 40.0f,0);

const D3DXVECTOR2 CGameClear::B_1_DEFAULET_SIZE		= D3DXVECTOR2(200,73);
const D3DXVECTOR3 CGameClear::B_1_DEFAULET_POS		=  D3DXVECTOR3(SCREEN_WIDTH / 2 - 5.0f,
														SCREEN_HEIGHT / 2 + B_0_POS_INTERVAL_Y + 40.0f,0);

const D3DXVECTOR3 CGameClear::DIRECTION_CAMERA_SPD	= D3DXVECTOR3(0.0f, 0.0f, 8.0f);	// 演出時のカメラ移動速度
const float CGameClear::DIRECTION_ADJUST_DIST		= 600.0f;							// 演出時のカメラ俯瞰距離の調整値

const int CGameClear::FADEIN_TIME = 5;		// フェードイン間隔(アルファ値:0～255)
const int CGameClear::FADEOUT_TIME = 10;		// フェードアウト間隔(アルファ値:0～255)

const int CGameClear::DRAWTEX_FADEIN_TIME = 10;

const double CGameClear::SELECT_ANIME_TIME = 0.5;
const D3DXVECTOR3 CGameClear::SELECT_BUTTON_SCALE_L = D3DXVECTOR3(1.1f, 1.1f, 1.0f);
const D3DXVECTOR3 CGameClear::SELECT_BUTTON_SCALE_S = D3DXVECTOR3(0.95f, 0.95f, 1.0f);

const float CGameClear::LAYOUTOBJ_SCALING_SPD	= 15.0f;	// レイアウトオブジェクト拡大速度
const float CGameClear::LAYOUTOBJ_LATEST_SPD	= 0.8f;		// レイアウトオブジェクト最遅速度


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

	m_pFilter			= NULL;
	m_pWnd				= NULL;
	m_pText				= NULL;

	m_pButtonNext		= NULL;
	m_pButtonGoSelect	= NULL;

	m_pCamera			= NULL;
	m_cameraStartPos	= D3DXVECTOR2(0.0f, 0.0f);
	m_dirDist			= 0.0f;

	m_bLastStage		= false;

	m_selectAnimeTimer	= 0.0;
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
	m_pFilter = CObject2D::Create(TEX_FILENAME[TEX_FILTER]);

	// ウィンドウ作成
	m_pWnd	= CObject2D::Create(TEX_FILENAME[TEX_WND_0]);
	m_pWnd->Init(W_0_DEFAULET_SIZE,W_0_DEFAULET_POS);
	
	m_pText = CObject2D::Create(TEX_FILENAME[TEX_TEXT]);
	
	// ボタン作成
	m_pButtonNext		= CButton::Create(TEX_FILENAME[TEX_NEXT]);
	m_pButtonGoSelect	= CButton::Create(TEX_FILENAME[TEX_SELECT]);

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
	if(m_pFilter){
		m_pFilter->Uninit();
		SAFE_RELEASE(m_pFilter)
	}

	if(m_pWnd){
		m_pWnd->Uninit();
		SAFE_RELEASE(m_pWnd)
	}

	if(m_pText){
		m_pText->Uninit();
		SAFE_RELEASE(m_pText)
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
//	Arguments   : stageID / ステージID
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameClear::Init(int stageID)
{
	m_nPhase = PHASE_INIT;
	if(stageID < CMapData::MAX_STAGEID - 1) {
		m_nCurButton	= NEXT_BUTTON;
		m_nGo			= GO_NEXT;
		m_bLastStage	= false;
	} else {
		m_nCurButton	= GOSELECT_BUTTON;
		m_nGo			= GO_SELECT;
		m_bLastStage	= true;
	}
	
	m_pFilter->Init(FILTER_SIZE, FILTER_POS);

	m_pText->Init(TEXT_SIZE, TEXT_POS);

	m_vecButton[m_nCurButton]->SetPhase(B_PHASE_CHOICE);

	// ----- レイアウトオブジェクト設定
	m_pLayoutObjects.clear();
	m_layoutObjEasingList.clear();
	CMapData::GetLayoutObjectList(&m_pLayoutObjects);
	m_layoutObjSizes.resize(m_pLayoutObjects.size());
	m_layoutObjEasingList.resize(m_pLayoutObjects.size());
	for(unsigned int i = 0; i < m_pLayoutObjects.size(); ++i) {
		m_pLayoutObjects[i]->TranslateZ(CGame::OBJ_CLEAR_PRIORITIES[CGame::OL_LAYOUT_OBJECT]);
		m_layoutObjSizes[i] = m_pLayoutObjects[i]->GetSize();
		m_pLayoutObjects[i]->Resize(D3DXVECTOR2(0.0f, 0.0f));
		m_layoutObjEasingList[i] = LAYOUTOBJ_SCALING_SPD;
	}
	
	m_pCamera			= NULL;
	m_cameraStartPos	= D3DXVECTOR2(0.0f, 0.0f);
	m_dirDist			= 0.0f;

	m_pWnd->SetAlpha(0);
	m_pText->SetAlpha(0);
	for (unsigned int i = 0;i < m_vecButton.size(); ++i) {
		m_vecButton[i]->SetAlpha(0);
		m_vecButton[i]->SetColor(D3DXVECTOR3(255,255,255));
	}

	m_nPhase = PHASE_INIT_DIRECTION;

	m_selectAnimeTimer = 0.0;
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
//	CGraphics::SetDraw2D();
	m_pFilter->Uninit();
	m_pText->Uninit();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : いろんな更新
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameClear::Update(CObject2D* pDark, CObject2D* pLight, std::vector<D3DXVECTOR2>* pClipSizeList)
{
	switch(m_nPhase)
	{
	case PHASE_INIT:
		break;
	case PHASE_FADEIN_DIRECTION:
		FadeinDirection();
		break;
	case PHASE_INIT_DIRECTION:
		InitDirection();
		pDark->TranslateX(m_cameraStartPos.x);
		pDark->TranslateY(m_cameraStartPos.y);
		pLight->TranslateX(m_cameraStartPos.x);
		pLight->TranslateY(m_cameraStartPos.y);
		break;
	case PHASE_DIRECTION:
		Direction(pDark, pLight, pClipSizeList);
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
	for(LPCHARACTER_ARRAY_IT it = m_pLayoutObjects.begin(); it != m_pLayoutObjects.end(); ++it) {
		(*it)->DrawAlpha();
	}

	switch(m_nPhase)
	{
	case PHASE_DIRECTION:
	case PHASE_UNINIT_DIRECTION:
		break;

	case PHASE_INIT:
	case PHASE_INIT_DIRECTION:
	case PHASE_FADEIN_DIRECTION:
		m_pFilter->DrawScreenAlpha();
		break;

	case PHASE_WAIT:
	case PHASE_ENTER:
		m_pWnd->DrawScreenAlpha();
		m_pText->DrawScreenAlpha();

		for(unsigned int i = 0;i < m_vecButton.size();i++){
			m_vecButton[i]->DrawScreenAlpha();
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
//	CGraphics::SetDraw3D();
	m_pCamera->SetEyeX(m_cameraStartPos.x);
	m_pCamera->SetEyeY(m_cameraStartPos.y);

	// ----- フェードイン準備
	//D3DXVECTOR3 fadePos(m_cameraStartPos.x, m_cameraStartPos.y, 0.0f);
	//CChangeScene::SetNormalFadePos(fadePos);
	//CChangeScene::SetNormalFadeAlpha(255);

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
	if(m_pFilter->FadeOutAlpha(FADEIN_TIME)) {
		CGameMain::PlayBGM(BGM_CLEAR, DSBPLAY_LOOPING);
		m_nPhase = PHASE_DIRECTION;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 演出処理
//	Description : カメラが画面を引いたり等、演出を行う
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameClear::Direction(CObject2D* pDark, CObject2D* pLight,  std::vector<D3DXVECTOR2>* pClipSizeList)
{
	// ----- ステージを俯瞰する
	float z = m_pCamera->GetEye().z;
	z -= DIRECTION_CAMERA_SPD.z;
	m_pCamera->SetEyeZ(z);

	// ----- 背景サイズの調整
	const float SCALE = 0.004f;
	pDark->ScalingX(SCALE);
	pDark->ScalingY(SCALE);
	pLight->ScalingX(SCALE);
	pLight->ScalingY(SCALE);
	
	// ----- クリップサイズの調整
	const float CLIP_SCALE = 20.0f;
	for(std::vector<D3DXVECTOR2>::iterator it = pClipSizeList->begin(); it != pClipSizeList->end(); ++it) {
		(*it).x += CLIP_SCALE;
		(*it).y += CLIP_SCALE;
	}

	// ----- レイアウトオブジェクト出現処理	
	for(unsigned int i = 0; i < m_pLayoutObjects.size(); ++i) {
		D3DXVECTOR2 size = m_pLayoutObjects[i]->GetSize();
		if (size.x >= m_layoutObjSizes[i].x)
			continue;

		if (m_layoutObjEasingList[i] > LAYOUTOBJ_LATEST_SPD)
			m_layoutObjEasingList[i] -= LAYOUTOBJ_LATEST_SPD;
		size.x += m_layoutObjEasingList[i];
		size.y += m_layoutObjEasingList[i];
		if (size.x > m_layoutObjSizes[i].x)
			size = m_layoutObjSizes[i];
		m_pLayoutObjects[i]->Resize(size);
	}

	// ----- 俯瞰完了(演出終了)
	if(m_dirDist <= tanf(D3DXToRadian(FOVY)) * -z + DIRECTION_ADJUST_DIST)
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
	if(!m_bLastStage) {
		// 選択　上下キー
		if(GetTrgKey(DIK_DOWN)){
			CGameMain::PlaySE(SE_CHOICE);
			if(m_nCurButton < MAX_BUTTON - 1) {
				m_nCurButton++;
				m_selectAnimeTimer = 0.0;
			}
		}
		if(GetTrgKey(DIK_UP)){
			CGameMain::PlaySE(SE_CHOICE);
			if(m_nCurButton > 0) {
				m_nCurButton--;
				m_selectAnimeTimer = 0.0;
			}
		}
	}

	// 現在選択されてるボタン
	for(unsigned int i = 0;i < m_vecButton.size();i++){

		if(i == m_nCurButton)
			m_vecButton[i]->SetPhase(B_PHASE_CHOICE);
		else
			m_vecButton[i]->SetPhase(B_PHASE_WAIT);
		
		if(m_vecButton[i]->GetPhase() == B_PHASE_CHOICE){
//			m_vecButton[i]->SetColor(D3DXVECTOR3(255,255,0));
			if(CTimer::GetTime() - m_selectAnimeTimer > SELECT_ANIME_TIME) {
				m_selectAnimeTimer = CTimer::GetTime();

				if(m_vecButton[i]->GetScale().x >= SELECT_BUTTON_SCALE_L.x) {
					m_vecButton[i]->Scale(SELECT_BUTTON_SCALE_S);
				} else {
					m_vecButton[i]->Scale(SELECT_BUTTON_SCALE_L);
				}
			}
		}else{
//			m_vecButton[i]->SetColor(D3DXVECTOR3(255,255,255));
			if(m_bLastStage) {
				m_vecButton[i]->SetColor(D3DXVECTOR3(96,96,96));
				m_vecButton[i]->Scale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			}
			else
				m_vecButton[i]->Scale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		}
	}

	// 決定
	if(GetTrgKey(DIK_RETURN)){
		CGameMain::PlaySE(SE_ENTER);
		m_vecButton[m_nCurButton]->SetPhase(B_PHASE_ENTER);
		m_nPhase = PHASE_ENTER;
	}

	// ----- テクスチャ透過
	m_pWnd->FadeInAlpha(DRAWTEX_FADEIN_TIME);
	m_pText->FadeInAlpha(DRAWTEX_FADEIN_TIME);
	for(unsigned int i = 0; i < m_vecButton.size(); ++i)
		m_vecButton[i]->FadeInAlpha(DRAWTEX_FADEIN_TIME);
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
	
	CGameMain::StopAllSE();
	CGameMain::StopBGM(BGM_CLEAR);
	
	m_nPhase = PHASE_UNINIT;
}

//========================================================================================
//	End of File
//========================================================================================