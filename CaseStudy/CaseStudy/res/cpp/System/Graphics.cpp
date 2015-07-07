//========================================================================================
//		File        : Graphics.cpp
//		Program     : 描画クラス
//
//		Description : 描画クラスの実装
//
//		History     : 2014/04/17	作成開始
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
#include "../../h/System/Graphics.h"

//――――――――――――――――――――――――――――――――――――――――――――
// マクロ定義
//――――――――――――――――――――――――――――――――――――――――――――
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)			if(x){ x->Release(); x=NULL; }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)			if(x){ delete x; x=NULL; }
#endif

//――――――――――――――――――――――――――――――――――――――――――――
// メンバ実体宣言
//――――――――――――――――――――――――――――――――――――――――――――
LPDIRECT3D9				CGraphics::m_pD3D		= NULL;		// Direct3D9 オブジェクト
LPDIRECT3DDEVICE9		CGraphics::m_pDevice	= NULL;		// Direct3DDevice9 オブジェクト
D3DPRESENT_PARAMETERS	CGraphics::m_D3DPP;					// PRESENT PARAMETERS
LPD3DXFONT				CGraphics::m_pFont		= NULL;		// D3DXFont オブジェクト

D3DXMATRIX	CGraphics::m_MatView;	// ビュー マトリックス
D3DXMATRIX	CGraphics::m_MatProj;	// 射影マトリックス

HWND	CGraphics::m_hWnd;		// ウィンドウ ハンドル
int		CGraphics::m_nWidth;	// 表示領域幅
int		CGraphics::m_nHeight;	// 表示領域高さ


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成処理
//	Description : 描画クラスを生成する
//	Arguments   : hWnd    / ウィンドウハンドル
//				  nWidth  / ゲーム画面の幅
//				  nHeight / ゲーム画面の高さ
//				  bWindow / ウィンドウモード
//	Returns     : 描画クラス
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CGraphics* CGraphics::Create(HWND hWnd, int nWidth, int nHeight, bool bWindow)
{
	CGraphics* pGraph = new CGraphics();
	if (pGraph) {
		if (!pGraph->Initialize(hWnd, nWidth, nHeight, bWindow)) {
			SAFE_DELETE(pGraph);
		}
	}
	return pGraph;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 解放処理
//	Description : 描画クラスを解放する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGraphics::Release()
{
	Finalize();
	delete this;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : レンダリング開始
//	Description : レンダリングを開始する
//	Arguments   : None.
//	Returns     : 成否
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CGraphics::Begin()
{
	// ビューポートをクリア
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
					 D3DCOLOR_XRGB(128, 128, 255), 1.0f, 0);

	return SUCCEEDED(m_pDevice->BeginScene());	// シーン開始
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : レンダリング終了
//	Description : レンダリングを終了する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGraphics::End()
{
	m_pDevice->EndScene();						// シーン終了
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : バックバッファ入れ替え
//	Description : バックバッファに描画した内容を表示する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGraphics::SwapBuffer()
{
	// バックバッファに描画した内容を表示
	if (FAILED(m_pDevice->Present(NULL, NULL, NULL, NULL))) {
		m_pDevice->Reset(&m_D3DPP);
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デバッグ文字列描画
//	Description : デバッグ文字列を描画する
//	Arguments   : nX      / X座標
//				  nY      / Y座標
//				  pszText / デバッグ文字列
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGraphics::DrawText(int nX, int nY, LPCTSTR pszText)
{
	RECT rcStr = {nX, nY, m_nWidth, m_nHeight};
	m_pFont->DrawText(NULL, pszText, -1, &rcStr, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 2D描画準備
//	Description : 左手座標系正射影行列を作成する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGraphics::SetDraw2D(void)
{
	D3DXMatrixOrthoLH(&m_MatProj, (float)m_nWidth,
		(float)m_nHeight, NEAR_CLIP, FAR_CLIP);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_MatProj);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 3D描画準備
//	Description : 左手座標系パースペクティブ射影行列を作成する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGraphics::SetDraw3D(void)
{
	D3DXMatrixPerspectiveFovLH(&m_MatProj, D3DXToRadian(FOVY),
		(float)m_nWidth / m_nHeight, NEAR_CLIP, FAR_CLIP);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_MatProj);
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : グラフィック環境を初期化する
//	Arguments   : hWnd    / ウィンドウハンドル
//				  nWidth  / ゲーム画面の幅
//				  nHeight / ゲーム画面の高さ
//				  bWindow / ウィンドウモード
//	Returns     : 成否(true:成功)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CGraphics::Initialize(HWND hWnd, int nWidth, int nHeight, bool bWindow)
{
	// デバイス情報テーブル
	struct TCreateDevice {
		D3DDEVTYPE	type;			// デバイスの種類
		DWORD		behavior;		// デバイスの動作
	};
	const int c_nMaxDevice = 3;
	const TCreateDevice device[c_nMaxDevice] = {
		{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING },	// ハードウェアシェーダを使う
		{ D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// HAL
		{ D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// REF
	};
	int		nDev;			// デバイス種別カウンタ

	// Direct3Dオブジェクトを生成
	if ((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		MessageBox(hWnd, _T("Direct3Dオブジェクトの作成に失敗しました"), _T("error"), MB_OK | MB_ICONERROR);
#endif
		return false;
	}
	// Direct3D 初期化パラメータの設定
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	ZeroMemory(&m_D3DPP, sizeof(D3DPRESENT_PARAMETERS));
	m_D3DPP.BackBufferWidth        = (UINT)nWidth;				// ゲーム画面サイズ
	m_D3DPP.BackBufferHeight       = (UINT)nHeight;
	m_D3DPP.Windowed               = bWindow;					// ウィンドウモードか?
	m_D3DPP.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	m_D3DPP.EnableAutoDepthStencil = true;						// Ｚバッファ有効
	m_D3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;				// Ｚバッファフォーマット
	if (bWindow) {
		// ウィンドウモード
		m_D3DPP.BackBufferFormat           = D3DFMT_UNKNOWN;				// バックバッファ
		m_D3DPP.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		m_D3DPP.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	} else {
		// フルスクリーンモード
		m_D3DPP.BackBufferFormat           = D3DFMT_X8R8G8B8;				// バックバッファ
		m_D3DPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
		m_D3DPP.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル
	}

	// デバイスオブジェクトを作成
	for (nDev = 0; nDev < c_nMaxDevice; nDev++) {
		HRESULT hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, device[nDev].type, hWnd,	// デバイスを作成
					  device[nDev].behavior, &m_D3DPP, &m_pDevice);
		if(SUCCEEDED(hr))
			break;
		
		switch(hr)
		{
			case D3DERR_DEVICELOST:
#ifdef _DEBUG_MESSAGEBOX
				MessageBox(hWnd, _T("D3DERR_DEVICELOST"), _T("error"), MB_OK | MB_ICONERROR);
#endif
				break;
			case D3DERR_INVALIDCALL:
#ifdef _DEBUG_MESSAGEBOX
				MessageBox(hWnd, _T("D3DERR_INVALIDCALL"), _T("error"), MB_OK | MB_ICONERROR);
#endif
				break;
			case D3DERR_NOTAVAILABLE:
#ifdef _DEBUG_MESSAGEBOX
				MessageBox(hWnd, _T("D3DERR_NOTAVAILABLE"), _T("error"), MB_OK | MB_ICONERROR);
#endif
				break;
			case D3DERR_OUTOFVIDEOMEMORY:
#ifdef _DEBUG_MESSAGEBOX
				MessageBox(hWnd, _T("D3DERR_OUTOFVIDEOMEMORY"), _T("error"), MB_OK | MB_ICONERROR);
#endif
				break;
			default:
				break;
		}
	}
	if (nDev >= c_nMaxDevice) {
#ifdef _DEBUG_MESSAGEBOX
		MessageBox(hWnd, _T("デバイスの作成に失敗しました"), _T("error"), MB_OK | MB_ICONERROR);
#endif
		return false;
	}
	
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);								// Zバッファを使用
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);					// αブレンドを行う
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);				// αソースカラーの指定
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);			// αデスティネーションカラーの指定
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		// アルファチャンネル有効に

	// 情報表示用フォントを設定
	D3DXCreateFont(m_pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);

	//-------- ここから３Ｄ関連
	// 3D描画設定
	SetDraw3D();

	// マテリアルの設定
	D3DMATERIAL9	mtrl;				// マテリアルオブジェクト
	ZeroMemory(&mtrl, sizeof(mtrl));	// いったんゼロでクリア
	mtrl.Diffuse.r = mtrl.Diffuse.g = mtrl.Diffuse.b = mtrl.Diffuse.a = 1.0f;
	mtrl.Ambient = mtrl.Diffuse;
	m_pDevice->SetMaterial(&mtrl);

	// ライトの設定
	D3DXVECTOR3	vecDir;					// 方向を指定するベクトル
	D3DLIGHT9	light;					// ライトオブジェクト
	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;	// 平行光源
	light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = light.Diffuse.a = 1.0f;
	light.Ambient.r = light.Ambient.g = light.Ambient.b = light.Ambient.a = 0.2f;
	light.Specular = light.Diffuse;
	vecDir = D3DXVECTOR3(1.0f, -1.0f, 1.0f);					// 光の向きを決める
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);	// 正規化したベクトルを返す
	m_pDevice->SetLight(0, &light);
	m_pDevice->LightEnable(0, TRUE);	// ライト0を有効

	m_pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
	
	//m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// 背面カリングを設定
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// 背面カリングを無効

	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : グラフィック環境をクリーンアップする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CGraphics::Finalize()
{
	SAFE_RELEASE(m_pFont);		// D3DXFont オブジェクトを解放
	SAFE_RELEASE(m_pDevice);	// Direct3DDevice オブジェクトを解放
	SAFE_RELEASE(m_pD3D);		// Direct3D オブジェクトを解放
}


//========================================================================================
//	End of File
//========================================================================================