//========================================================================================
//		File        : Graphics.h
//		Program     : 描画クラス
//
//		Description : 描画クラスの定義
//
//		History     : 2014/04/17	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================

#pragma once

//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../System/PreInclude.h"
#
#include <d3d9.h>
#include <d3dx9.h>

//――――――――――――――――――――――――――――――――――――――――――――
// 定数・マクロ定義
//――――――――――――――――――――――――――――――――――――――――――――
const float FOVY		= 30.0f;		// 視野角
const float NEAR_CLIP	= 1.0f;			// ニアクリップを行う距離
const float FAR_CLIP	= 10000.0f;		// ファークリップを行う距離

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CGraphics
{
// ===== メンバ変数
private:
	static LPDIRECT3D9				m_pD3D;				// Direct3D9 オブジェクト
	static LPDIRECT3DDEVICE9		m_pDevice;			// Direct3DDevice9 オブジェクト
	static D3DPRESENT_PARAMETERS	m_D3DPP;			// PRESENT PARAMETERS
	static LPD3DXFONT				m_pFont;			// D3DXFont オブジェクト

	static D3DXMATRIX				m_MatView;			// ビュー マトリックス
	static D3DXMATRIX				m_MatProj;			// 射影マトリックス

	static HWND						m_hWnd;				// ウィンドウ ハンドル
	static int						m_nWidth;			// 表示領域幅
	static int						m_nHeight;			// 表示領域高さ

	static DWORD					m_curZTest;			// 既存のZテストの有無
	static DWORD					m_curZFunc;			// 既存のZテスト比較関数
	
// ===== メンバ関数
public:
	CGraphics();
	virtual ~CGraphics();

	static LPDIRECT3DDEVICE9 GetDevice() {return m_pDevice;}

	static CGraphics* Create(HWND hWnd, int nWidth, int nHeight, bool bWindow);
	void Release();
	bool Begin();
	void End();
	void SwapBuffer();
	void DrawText(int nX, int nY, LPCTSTR pszText);

	static bool StencilRegionBegin();	// ステンシルクリッピング領域作成開始
	static bool StencilRegionEnd();		// ステンシルクリッピング領域作成終了
	static bool StencilDrawBegin();		// ステンシルクリッピング描画開始
	static bool StencilDrawEnd();		// ステンシルクリッピング描画終了
	
	static void SetDraw2D();		// 2D描画準備
	static void SetDraw3D();		// 3D描画準備

private:
	bool Initialize(HWND hWnd, int nWidth, int nHeight, bool bWindow);
	void Finalize();
};


//========================================================================================
//	End of File
//========================================================================================