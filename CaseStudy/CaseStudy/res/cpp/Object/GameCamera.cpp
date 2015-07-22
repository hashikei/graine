//========================================================================================
//		File        : Camera.cpp
//		Program     : カメラクラス
//
//		Description : カメラクラスの実装
//
//		History     : 2013/11/26	作成開始
//					  2014/05/23	ファイル名変更
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
#include "../../h/System/Graphics.h"
#include "../../h/Scene/Game.h"
#include "../../h/Object/GameCamera.h"

//――――――――――――――――――――――――――――――――――――――――――――
// メンバ実体宣言
//――――――――――――――――――――――――――――――――――――――――――――
const float CGameCamera::DEFAULT_CAMERA_POS_Z = -2000.0f;
const D3DXVECTOR3 CGameCamera::CAMERA_DEFAULT_POS = D3DXVECTOR3(0.0f,0.0f,DEFAULT_CAMERA_POS_Z);
const float CGameCamera::C_LENGTH_0 = 100;
const float CGameCamera::C_LENGTH_1 = 50;
const float CGameCamera::C_LENGTH_2 = 250;
const float CGameCamera::C_MOVE_SPD = 5;


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CGameCamera::CGameCamera()
{
	m_nPhase = 0;
	m_bMove = false;
	m_eye	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vNextEye	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_look	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_up	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CGameCamera::~CGameCamera()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : カメラデータを初期化する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameCamera::Init(void)
{
	m_nPhase = PHASE_MAIN;
	m_eye = CAMERA_DEFAULT_POS;
	m_vNextEye = m_eye;
	SyncEyeLook();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : カメラデータを解放する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameCamera::Uninit(void)
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : カメラデータを更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameCamera::Update()
{
	switch (m_nPhase)
	{
	case PHASE_FADEIN:
	case PHASE_MAIN:
		cameraControllMain();
		break;
	}
	SyncEyeLook();
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動操作
//	Description : カメラデータを更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameCamera::cameraControllMain()
{
	float move;
	move = m_vNextEye.x - m_eye.x;

	if(abs(move) > C_LENGTH_0){
		m_bMove = true;
	}
	if(abs(move) < C_LENGTH_1){
		m_bMove = false;
	}
	if(abs(move) > C_LENGTH_2){
		m_bMove = false;
		if(move > 0 && m_eye.x < CMapData::GetRightWallX() - SCREEN_WIDTH / 1.25)
			m_eye.x = m_vNextEye.x - C_LENGTH_2;
	//	else if(move > 0)
	//		m_eye.x = CMapData::GetRightWallX() - SCREEN_WIDTH / 1.25;
		if(move < 0 && m_eye.x > CMapData::GetLeftWallX() + SCREEN_WIDTH / 1.25)
			m_eye.x = m_vNextEye.x + C_LENGTH_2;
		else if(move < 0)
			m_eye.x = CMapData::GetLeftWallX() + SCREEN_WIDTH / 1.25f;
	}

	if(m_bMove){
		if(move > 0 && m_eye.x < CMapData::GetRightWallX() - SCREEN_WIDTH / 1.25)
			m_eye.x += C_MOVE_SPD;
		if(move < 0 && m_eye.x > CMapData::GetLeftWallX() + SCREEN_WIDTH / 1.25)
			m_eye.x -= C_MOVE_SPD;
	}

	if(m_vNextEye.y < CMapData::GetTopWallY() - SCREEN_HEIGHT * 0.5f && m_vNextEye.y > CMapData::GetBottomWallY() + SCREEN_HEIGHT * 0.5f)
		m_eye.y = m_vNextEye.y;
	else if(m_vNextEye.y > CMapData::GetTopWallY() - SCREEN_HEIGHT * 0.5f)
		m_eye.y = CMapData::GetTopWallY() - SCREEN_HEIGHT * 0.5f;
	else if(m_vNextEye.y < CMapData::GetBottomWallY() + SCREEN_HEIGHT * 0.5f)
		m_eye.y = CMapData::GetBottomWallY() + SCREEN_HEIGHT * 0.5f;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : カメラを描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameCamera::Draw(void)
{	
	// ----- 存在していなければ未処理
	if(!m_bExist)
		return;

	// ----- 変数宣言
	D3DXMATRIX	matView;		// ビュー マトリックス

	// ----- カメラ位置更新
	D3DXMatrixLookAtLH(&matView,
						&m_eye,		// 視点座標
						&m_look,	// 注視点座標
						&m_up);		// アップベクトル
	CGraphics::GetDevice()->SetTransform(D3DTS_VIEW, &matView);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 解放
//	Description : カメラデータを解放する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameCamera::Release(void)
{
	Finalize();
	delete this;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成処理
//	Description : カメラデータを生成する
//	Arguments   : None.
//	Returns     : カメラデータ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CGameCamera* CGameCamera::Create(void)
{
	// ----- 変数宣言
	CGameCamera* pCamera;

	// ----- 初期化処理
	pCamera = new CGameCamera();
	if(pCamera)
	{
		if(!pCamera->Initialize())
		{
			SAFE_DELETE(pCamera);
		}
	}

	return pCamera;
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 位置と注視点の同期
//	Description : 注視点を位置と同じ座標にする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameCamera::SyncEyeLook(void)
{
	SetLook(m_eye);
	SetLookZ(0);
	SetUp(D3DXVECTOR3(0,1,0));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : ブロック情報を初期化する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CGameCamera::Initialize(void)
{
	m_bExist = true;

	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : ブロック情報の事後処理をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGameCamera::Finalize(void)
{
}

//========================================================================================
//	End of File
//========================================================================================