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
#include "../../h/System/System.h"
#include "../../h/System/Graphics.h"
#include "../../h/Object/Camera.h"

//――――――――――――――――――――――――――――――――――――――――――――
// 定数・マクロ定義
//――――――――――――――――――――――――――――――――――――――――――――
// カメラ種別
enum _eCameraType {
	CT_FIXED = 0,		// 定点
	CT_FIRSTPERSON,		// 一人称
	CT_THIRDPERSON,		// 三人称

	MAX_CAMERATYPE		// カメラ種別数
};


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CCamera::CCamera()
{
	m_eye	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_look	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_up	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type	= MAX_CAMERATYPE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CCamera::~CCamera()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : カメラデータを初期化する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCamera::Init(void)
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : カメラデータを解放する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCamera::Uninit(void)
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : カメラデータを更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCamera::Update()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : カメラを描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCamera::Draw(void)
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
void CCamera::Release(void)
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
CCamera* CCamera::Create(void)
{
	// ----- 変数宣言
	CCamera* pCamera;

	// ----- 初期化処理
	pCamera = new CCamera();
	if(pCamera)
	{
		if(!pCamera->Initialize())
		{
			SAFE_DELETE(pCamera);
		}
	}

	return pCamera;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : カメラパラメータセット
//	Description : カメラのパラメータをセットする
//	Arguments   : eye	/ 視点座標
//				  look	/ 注視点座標
//				  up	/ 上方ベクトル
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCamera::SetParameter(D3DXVECTOR3& eye, D3DXVECTOR3& look, D3DXVECTOR3& up)
{
	// 視点座標セット
	if(eye != NULL) {
		SetEye(eye);
	} else {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CCamera::SetParameterのeyeがNULLです。"), _T("error"), MB_OK);
#endif
	}

	// 注視点座標セット
	if(look != NULL) {
		SetLook(look);
	} else {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CCamera::SetParameterのlookがNULLです。"), _T("error"), MB_OK);
#endif
	}
	
	// 上方ベクトルセット
	if(up != NULL) {
		SetUp(up);
	} else {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CCamera::SetParameterのupがNULLです。"), _T("error"), MB_OK);
#endif
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : カメラパラメータセット
//	Description : カメラのパラメータをセットする
//	Arguments   : eye	/ 視点座標
//				  look	/ 注視点座標
//				  up	/ 上方ベクトル
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCamera::SetParameter(const D3DXVECTOR3& eye, const D3DXVECTOR3& look, const D3DXVECTOR3& up)
{
	// 視点座標セット
	if(eye != NULL) {
		SetEye(eye);
	} else {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CCamera::SetParameterのeyeがNULLです。"), _T("error"), MB_OK);
#endif
	}

	// 注視点座標セット
	if(look != NULL) {
		SetLook(look);
	} else {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CCamera::SetParameterのlookがNULLです。"), _T("error"), MB_OK);
#endif
	}

	// 上方ベクトルセット
	if(up != NULL) {
		SetUp(up);
	} else {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CCamera::SetParameterのupがNULLです。"), _T("error"), MB_OK);
#endif	
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 視点座標セット
//	Description : カメラの視点座標をセットする
//	Arguments   : eye / 視点座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCamera::SetEye(D3DXVECTOR3& eye)
{
	SetEyeX(eye.x);
	SetEyeY(eye.y);
	SetEyeZ(eye.z);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 視点座標セット
//	Description : カメラの視点座標をセットする
//	Arguments   : eye / 視点座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCamera::SetEye(const D3DXVECTOR3& eye)
{
	SetEyeX(eye.x);
	SetEyeY(eye.y);
	SetEyeZ(eye.z);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 注視点座標セット
//	Description : カメラの注視点座標をセットする
//	Arguments   : look / 注視点座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCamera::SetLook(D3DXVECTOR3& look)
{
	SetLookX(look.x);
	SetLookY(look.y);
	SetLookZ(look.z);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 注視点座標セット
//	Description : カメラの注視点座標をセットする
//	Arguments   : look / 注視点座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCamera::SetLook(const D3DXVECTOR3& look)
{
	SetLookX(look.x);
	SetLookY(look.y);
	SetLookZ(look.z);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 上方ベクトルセット
//	Description : カメラの上方ベクトルをセットする
//	Arguments   : up / 上方ベクトル
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCamera::SetUp(D3DXVECTOR3& up)
{
	SetUpX(up.x);
	SetUpY(up.y);
	SetUpZ(up.z);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 上方ベクトルセット
//	Description : カメラの上方ベクトルをセットする
//	Arguments   : up / 上方ベクトル
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCamera::SetUp(const D3DXVECTOR3& up)
{
	SetUpX(up.x);
	SetUpY(up.y);
	SetUpZ(up.z);
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : ブロック情報を初期化する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CCamera::Initialize(void)
{
	m_bExist = true;
	m_type	= CT_FIXED;

	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : ブロック情報の事後処理をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCamera::Finalize(void)
{
}


//========================================================================================
//	End of File
//========================================================================================