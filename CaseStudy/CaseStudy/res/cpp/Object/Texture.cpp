//========================================================================================
//		File        : Texture.cpp
//		Program     : テクスチャクラス
//
//		Description : テクスチャクラスの実装
//					  原点：テクスチャの左上頂点
//
//		History     : 2014/06/18	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include "../../h/Object/Texture.h"

//――――――――――――――――――――――――――――――――――――――――――――
// メンバ実体宣言
//――――――――――――――――――――――――――――――――――――――――――――
CTexture::TEX_INFO* CTexture::m_pTexInfoTop = NULL;


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CTexture::CTexture()
{
	m_pDevice	= NULL;
	m_pTexture	= NULL;
	m_pTexInfo	= NULL;

	// 頂点データ初期化
	for(int i = 0; i < 4; ++i)
	{
		m_vtx[i].vtx = DEFAULT_VTX2D_VTX[i];	// 画面全体
		m_vtx[i].col = DEFAULT_VTX_COL_W;		// 白色
		m_vtx[i].uv  = DEFAULT_VTX_UV[i];		// UV全体
	}
	
	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size		= D3DXVECTOR2(0.0f, 0.0f);
	m_offset	= D3DXVECTOR2(0.0f, 0.0f);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CTexture::~CTexture()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : テクスチャデータをデフォルト値で初期化する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::Init()
{
	// 頂点データ初期化
	for(int i = 0; i < 4; ++i)
	{
		m_vtx[i].vtx = DEFAULT_VTX2D_VTX[i];	// 画面全体
		m_vtx[i].col = DEFAULT_VTX_COL_W;		// 白色
		m_vtx[i].uv  = DEFAULT_VTX_UV[i];		// UV全体
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : テクスチャデータを初期化する
//	Arguments   : size / テクスチャサイズ
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::Init(const D3DXVECTOR2& size)
{
	// 頂点データ初期化
	Init();

	// サイズ設定
	Resize(size);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : テクスチャデータを初期化する
//	Arguments   : width  / テクスチャ幅
//				  height / テクスチャ高さ
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::Init(const float width, const float height)
{
	// 頂点データ初期化
	Init(D3DXVECTOR2(width, height));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : テクスチャデータを初期化する
//	Arguments   : size / テクスチャサイズ
//				  pos  / 出現位置(テクスチャの左上頂点)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::Init(const D3DXVECTOR2& size, const D3DXVECTOR3& pos)
{
	// 頂点データ初期化
	Init();

	// 描画位置設定
	SetPosition(pos);

	// サイズ設定
	Resize(size);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : テクスチャデータを初期化する
//	Arguments   : width  / テクスチャ幅
//				  height / テクスチャ高さ
//				  x      / 出現位置(X座標)
//				  y      / 出現位置(Y座標)
//				  z      / 出現位置(Z座標)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::Init(const float width, const float height, const float x, const float y, const float z)
{
	Init(D3DXVECTOR2(width, height), D3DXVECTOR3(x, y, z));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : テクスチャデータの後始末をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::Uninit(void)
{
	memset(m_vtx, 0, sizeof(m_vtx));	// 頂点情報クリア
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : テクスチャを透過無しで描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::Draw()
{
	// ----- デバイスが設定されていなければ未処理
	if(m_pDevice == NULL) return;

	// ----- 頂点フォーマットの設定
	m_pDevice->SetFVF(FVF_VERTEX2D);

	// ----- 背景テクスチャの設定及びポリゴンの描画
	m_pDevice->SetTexture(0, m_pTexture);
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_vtx, sizeof(VERTEX2D));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : テクスチャを透過有りで描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::DrawAlpha()
{
	// ----- デバイスが設定されていなければ未処理
	if(m_pDevice == NULL) return;

	// ----- 頂点フォーマットの設定
	m_pDevice->SetFVF(FVF_VERTEX2D);

	 // ----- アルファ ブレンド有効化
    m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			
	// ----- 背景テクスチャの設定及びポリゴンの描画
	m_pDevice->SetTexture(0, m_pTexture);
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_vtx, sizeof(VERTEX2D));
	
    // ----- アルファ ブレンド無効化
    m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成
//	Description : テクスチャデータを生成する
//	Arguments   : pszFName / 読み込みファイル名
//	Returns     : テクスチャデータ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CTexture* CTexture::Create(const LPCTSTR pszFName)
{
	// ----- 変数宣言
	CTexture* pTexture;

	// ----- 初期化処理
	pTexture = new CTexture();
	if(pTexture)
	{
		if(!pTexture->Initialize(pszFName))
		{
			SAFE_DELETE(pTexture);
		}
	}

	return pTexture;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 解放処理
//	Description : テクスチャデータを解放する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::Release()
{
	Finalize();
	delete this;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 頂点カラー設定
//	Description : テクスチャの頂点カラーを設定する
//	Arguments   : color / 頂点カラー(RGB)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::SetColor(const D3DXVECTOR3& color)
{
	// 頂点カラー設定
	for(int i = 0; i < 4; ++i)
		m_vtx[i].col = D3DCOLOR_RGBA((int)color.x, (int)color.y, (int)color.z, D3DCOLOR_RGBA_TO_ALPHA(m_vtx[i].col));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 透過度設定
//	Description : テクスチャの透過度を設定する
//	Arguments   : alpha / 透過度
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::SetAlpha(float alpha)
{
	// 透過度設定
	for(int i = 0; i < 4; ++i)
		m_vtx[i].col = D3DCOLOR_RGBA(D3DCOLOR_RGBA_TO_R(m_vtx[i].col),
									 D3DCOLOR_RGBA_TO_G(m_vtx[i].col),
									 D3DCOLOR_RGBA_TO_B(m_vtx[i].col),
									 (int)alpha);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 現在位置更新
//	Description : テクスチャの現在位置を更新する
//	Arguments   : pos / 出現位置
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::SetPosition(const D3DXVECTOR3& pos)
{
	// 現在位置更新
	m_pos = pos;

	// テクスチャに反映
	Update();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 現在位置更新
//	Description : テクスチャの現在位置を更新する
//	Arguments   : x / 出現位置(X座標)
//				  y / 出現位置(Y座標)
//				  z / 出現位置(Z座標)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::SetPosition(const float x, const float y, const float z)
{
	SetPosition(D3DXVECTOR3(x, y, z));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 現在位置更新
//	Description : テクスチャの現在位置(X座標)を更新する
//	Arguments   : x / 出現位置(X座標)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::SetPosX(const float x)
{
	SetPosition(D3DXVECTOR3(x, m_pos.y, m_pos.z));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 現在位置更新
//	Description : テクスチャの現在位置(Y座標)を更新する
//	Arguments   : y / 出現位置(Y座標)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::SetPosY(const float y)
{
	SetPosition(D3DXVECTOR3(m_pos.x, y, m_pos.z));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 現在位置更新
//	Description : テクスチャの現在位置(Z座標)を更新する
//	Arguments   : z / 出現位置(Z座標)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::SetPosZ(const float z)
{
	SetPosition(D3DXVECTOR3(m_pos.x, m_pos.y, z));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 現在位置更新
//	Description : テクスチャを現在位置から移動する
//	Arguments   : vec / 移動量
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::AddPosition(const D3DXVECTOR3& vec)
{
	// 現在位置更新
	m_pos += vec;

	// テクスチャに反映
	Update();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 現在位置更新
//	Description : テクスチャを現在位置から移動する
//	Arguments   : x / 移動量(X座標)
//				  y / 移動量(Y座標)
//				  z / 移動量(Z座標)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::AddPosition(const float x, const float y, const float z)
{
	AddPosition(D3DXVECTOR3(x, y, z));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 現在位置更新
//	Description : テクスチャを現在位置からX方向に移動する
//	Arguments   : x / 移動量(X座標)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::AddPosX(const float x)
{
	AddPosition(D3DXVECTOR3(x, 0.0f, 0.0f));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 現在位置更新
//	Description : テクスチャを現在位置からY方向に移動する
//	Arguments   : y / 移動量(Y座標)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::AddPosY(const float y)
{
	AddPosition(D3DXVECTOR3(0.0f, y, 0.0f));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 左右反転
//	Description : テクスチャを左右反転する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::HorizonFlip()
{
	// ----- 変数宣言
	float tmp;

	// ----- 反転処理
	tmp = m_vtx[0].uv.x;
	m_vtx[0].uv.x = m_vtx[1].uv.x;
	m_vtx[1].uv.x = tmp;
	tmp = m_vtx[2].uv.x;
	m_vtx[2].uv.x = m_vtx[3].uv.x;
	m_vtx[3].uv.x = tmp;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 上下反転
//	Description : テクスチャを上下反転する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::VerticalFlip()
{
	// ----- 変数宣言
	float tmp;

	// ----- 反転処理
	tmp = m_vtx[0].uv.y;
	m_vtx[0].uv.y = m_vtx[2].uv.y;
	m_vtx[2].uv.y = tmp;
	tmp = m_vtx[1].uv.y;
	m_vtx[1].uv.y = m_vtx[3].uv.y;
	m_vtx[3].uv.y = tmp;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 現在位置更新
//	Description : テクスチャを現在位置からZ方向に移動する
//	Arguments   : z / 移動量(Z座標)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::AddPosZ(const float z)
{
	AddPosition(D3DXVECTOR3(0.0f, 0.0f, z));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : フェード処理
//	Description : 対象のテクスチャを任意の間隔でフェード処理する
//	Arguments   : nFade / フェード間隔
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::FadeColor(int nFade)
{
	// ----- 変数宣言
	DWORD dwTempRed;		// r値格納バッファ
	DWORD dwTempGreen;		// g値格納バッファ
	DWORD dwTempBlue;		// b値格納バッファ

	// ----- 初期化処理
	dwTempRed	= 0;
	dwTempGreen	= 0;
	dwTempBlue	= 0;

	// ----- エラー処理
	if(nFade < 0)
		nFade = 0;
	if(nFade > 255)
		nFade = 255;
	
	// ----- フェードイン
	for(int nCntVtx = 0; nCntVtx < 4; nCntVtx ++)
	{
		dwTempRed	= ((m_vtx + nCntVtx)->col & 0x00ff0000);					// r値抽出
		dwTempGreen	= ((m_vtx + nCntVtx)->col & 0x0000ff00);					// g値抽出
		dwTempBlue	= ((m_vtx + nCntVtx)->col & 0x000000ff);					// b値抽出
		(m_vtx + nCntVtx)->col ^= (dwTempRed | dwTempGreen | dwTempBlue);		// アルファ値クリア
		dwTempRed	>>= 16;														// 整数へ変換
		dwTempGreen	>>= 8;														// 整数へ変換
		dwTempRed = nFade;
		dwTempGreen = nFade;
		dwTempBlue = nFade;
		dwTempRed	<<= 16;														// r位置へ戻す
		dwTempGreen	<<= 8;														// g位置へ戻す
		(m_vtx + nCntVtx)->col |= (dwTempRed | dwTempGreen | dwTempBlue);		// フェード反映
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : フェードイン
//	Description : 対象のテクスチャを任意の間隔でフェードインする
//	Arguments   : nFade / フェード間隔
//	Returns     : 完了フラグ(true:完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CTexture::FadeInColor(int nFade)
{
	// ----- 変数宣言
	DWORD dwTempRed;		// r値格納バッファ
	DWORD dwTempGreen;		// g値格納バッファ
	DWORD dwTempBlue;		// b値格納バッファ
	bool  complete;			// 完了フラグ

	// ----- 初期化処理
	dwTempRed	= 0;
	dwTempGreen	= 0;
	dwTempBlue	= 0;
	complete	= false;

	// ----- エラー処理
	if(nFade < 0)
		nFade = 0;
	if(nFade > 255)
		nFade = 255;
	
	// ----- フェードイン
	for(int nCntVtx = 0; nCntVtx < 4; nCntVtx ++)
	{
		dwTempRed	= ((m_vtx + nCntVtx)->col & 0x00ff0000);					// r値抽出
		dwTempGreen	= ((m_vtx + nCntVtx)->col & 0x0000ff00);					// g値抽出
		dwTempBlue	= ((m_vtx + nCntVtx)->col & 0x000000ff);					// b値抽出
		(m_vtx + nCntVtx)->col ^= (dwTempRed | dwTempGreen | dwTempBlue);		// アルファ値クリア
		dwTempRed	>>= 16;														// 整数へ変換
		dwTempGreen	>>= 8;														// 整数へ変換
		if(dwTempRed < 255)														// フェード更新
			dwTempRed += nFade;
		if(dwTempGreen < 255)													// フェード更新
			dwTempGreen += nFade;
		if(dwTempBlue < 255)													// フェード更新
			dwTempBlue += nFade;
		if(dwTempRed >= 255)													// エラー処理
		{
			dwTempRed = 255;
			complete  = true;
		}
		if(dwTempGreen >= 255)													// エラー処理
		{
			dwTempGreen = 255;
			complete    = true;
		}
		if(dwTempBlue >= 255)													// エラー処理
		{
			dwTempBlue = 255;
			complete   = true;
		}
		dwTempRed	<<= 16;														// r位置へ戻す
		dwTempGreen	<<= 8;														// g位置へ戻す
		(m_vtx + nCntVtx)->col |= (dwTempRed | dwTempGreen | dwTempBlue);		// フェード反映
	}

	return complete;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : フェードアウト
//	Description : 対象のテクスチャを任意の間隔でフェードアウトする
//	Arguments   : nFade / フェード間隔
//	Returns     : 完了フラグ(true:完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CTexture::FadeOutColor(int nFade)
{
	// ----- 変数宣言
	DWORD dwTempRed;		// r値格納バッファ
	DWORD dwTempGreen;		// g値格納バッファ
	DWORD dwTempBlue;		// b値格納バッファ
	int   nRed;				// r値格納バッファ
	int   nGreen;			// g値格納バッファ
	int   nBlue;			// b値格納バッファ
	bool  complete;			// 完了フラグ

	// ----- 初期化処理
	dwTempRed	= 0;
	dwTempGreen	= 0;
	dwTempBlue	= 0;
	nRed		= 0;
	nGreen		= 0;
	nBlue		= 0;
	complete	= false;

	// ----- エラー処理
	if(nFade < 0)
		nFade = 0;
	if(nFade > 255)
		nFade = 255;
	
	// ----- フェードアウト
	for(int nCntVtx = 0; nCntVtx < 4; nCntVtx ++)
	{
		dwTempRed	= ((m_vtx + nCntVtx)->col & 0x00ff0000);					// r値抽出
		dwTempGreen	= ((m_vtx + nCntVtx)->col & 0x0000ff00);					// g値抽出
		dwTempBlue	= ((m_vtx + nCntVtx)->col & 0x000000ff);					// b値抽出
		(m_vtx + nCntVtx)->col ^= (dwTempRed | dwTempGreen | dwTempBlue);		// アルファ値クリア
		nRed	= dwTempRed   >> 16;											// 整数へ変換
		nGreen	= dwTempGreen >> 8;												// 整数へ変換
		nBlue	= dwTempBlue;													// 整数へ変換
		if(nRed > 0)															// フェード更新
			nRed -= nFade;
		if(nGreen > 0)															// フェード更新
			nGreen -= nFade;
		if(nBlue > 0)															// フェード更新
			nBlue -= nFade;
		if(nRed <= 0)															// エラー処理
		{
			nRed	 = 0;
			complete = true;
		}
		if(nGreen <= 0)															// エラー処理
		{
			nGreen	 = 0;
			complete = true;
		}
		if(nBlue <= 0)															// エラー処理
		{
			nBlue	 = 0;
			complete = true;
		}
		dwTempRed	= nRed   << 16;												// r位置へ戻す
		dwTempGreen	= nGreen << 8;												// g位置へ戻す
		dwTempBlue	= nBlue;													// b位置へ戻す
		(m_vtx + nCntVtx)->col |= (dwTempRed | dwTempGreen | dwTempBlue);		// フェード反映
	}

	return complete;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 透過フェード処理
//	Description : 対象のテクスチャを任意の間隔で透過的にフェード処理する
//	Arguments   : nFade / フェード間隔
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::FadeAlpha(int nFade)
{
	// ----- 変数宣言
	DWORD dwTempAlpha;		// アルファ値格納バッファ

	// ----- 初期化処理
	dwTempAlpha = 0;

	// ----- エラー処理
	if(nFade < 0)
		nFade = 0;
	if(nFade > 255)
		nFade = 255;
	
	// ----- フェードイン
	for(int nCntVtx = 0; nCntVtx < 4; nCntVtx ++)
	{
		dwTempAlpha = ((m_vtx + nCntVtx)->col & 0xff000000);	// アルファ値抽出
		(m_vtx + nCntVtx)->col ^= dwTempAlpha;					// アルファ値クリア
		dwTempAlpha >>= 24;										// 整数へ変換
		dwTempAlpha = nFade;
		dwTempAlpha <<= 24;										// アルファ位置へ戻す
		(m_vtx + nCntVtx)->col |= dwTempAlpha;					// フェード反映
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 透過フェードイン
//	Description : 対象のテクスチャを任意の間隔で透過的にフェードインする
//	Arguments   : nFade / フェード間隔
//	Returns     : 完了フラグ(true:完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CTexture::FadeInAlpha(int nFade)
{
	// ----- 変数宣言
	DWORD dwTempAlpha;		// アルファ値格納バッファ
	bool  complete;			// 完了フラグ

	// ----- 初期化処理
	dwTempAlpha = 0;
	complete	= false;

	// ----- エラー処理
	if(nFade < 0)
		nFade = 0;
	if(nFade > 255)
		nFade = 255;
	
	// ----- フェードイン
	for(int nCntVtx = 0; nCntVtx < 4; nCntVtx ++)
	{
		dwTempAlpha = ((m_vtx + nCntVtx)->col & 0xff000000);	// アルファ値抽出
		(m_vtx + nCntVtx)->col ^= dwTempAlpha;					// アルファ値クリア
		dwTempAlpha >>= 24;										// 整数へ変換
		if(dwTempAlpha < 255)									// フェード更新
			dwTempAlpha += nFade;
		if(dwTempAlpha >= 255)									// エラー処理
		{
			dwTempAlpha	= 255;
			complete	= true;
		}
		dwTempAlpha <<= 24;										// アルファ位置へ戻す
		(m_vtx + nCntVtx)->col |= dwTempAlpha;					// フェード反映
	}

	return complete;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 透過フェードアウト
//	Description : 対象のテクスチャを任意の間隔で透過的にフェードアウトする
//	Arguments   : nFade / フェード間隔
//	Returns     : 完了フラグ(true:完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CTexture::FadeOutAlpha(int nFade)
{
	// ----- 変数宣言
	DWORD dwTempAlpha;		// アルファ値格納バッファ
	int   nAlpha;			// アルファ値格納バッファ
	bool  complete;			// 完了フラグ

	// ----- 初期化処理
	dwTempAlpha = 0;
	nAlpha		= 0;
	complete	= false;

	// ----- エラー処理
	if(nFade < 0)
		nFade = 0;
	if(nFade > 255)
		nFade = 255;
	
	// ----- フェードアウト
	for(int nCntVtx = 0; nCntVtx < 4; nCntVtx ++)
	{
		dwTempAlpha = ((m_vtx + nCntVtx)->col & 0xff000000);	// アルファ値抽出
		(m_vtx + nCntVtx)->col ^= dwTempAlpha;					// アルファ値クリア
		nAlpha = dwTempAlpha >> 24;								// 整数へ変換
		if(nAlpha > 0)											// フェード更新
			nAlpha -= nFade;
		if(nAlpha <= 0)											// エラー処理
		{
			nAlpha	= 0;
			complete	= true;
		}
		dwTempAlpha = nAlpha << 24;								// アルファ位置へ戻す
		(m_vtx + nCntVtx)->col |= dwTempAlpha;					// フェード反映
	}

	return complete;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : テクスチャサイズ変更
//	Description : テクスチャのサイズを変更する
//	Arguments   : size / テクスチャサイズ
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::Resize(const D3DXVECTOR2& size)
{
	// サイズ設定
	m_size = size;

	// テクスチャに反映
	Update();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : テクスチャサイズ変更
//	Description : テクスチャのサイズを変更する
//	Arguments   : width  / テクスチャ幅
//				  height / テクスチャ高さ
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::Resize(const float width, const float height)
{
	Resize(D3DXVECTOR2(width, height));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : UV値を分割
//	Description : UV値を任意の数に分割し、現在の数値のUV値を設定する
//				  ※左上から右下に向かって数値を数える
//	Arguments   : num    / 参照数値
//				  width  / 横分割数
//				  height / 縦分割数
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::UVDivision(const int num, const int width, const int height)
{
	// ----- UV値を算出
	float left		= (1.0f / width) * ((num) % width);
	float right		= (1.0f / width) * ((num) % width) + (1.0f / width);
	float top		= (1.0f / height) * ((num) / width);
	float bottom	= (1.0f / height) * ((num) / width) + (1.0f / height);

	// ----- UV値を設定
	m_vtx[0].uv = D3DXVECTOR2(left,  top);
	m_vtx[1].uv = D3DXVECTOR2(right, top);
	m_vtx[2].uv = D3DXVECTOR2(left,  bottom);
	m_vtx[3].uv = D3DXVECTOR2(right, bottom);
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : テクスチャデータを初期化する
//	Arguments   : pszFName / 読み込みファイル名
//	Returns     : 成否
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CTexture::Initialize(const LPCTSTR pszFName)
{
	// ----- テクスチャ読み込み
	if(FAILED(Load(CGraphics::GetDevice(), pszFName)))
	{
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CTexture::Initialize テクスチャの読み込みに失敗しました。"), _T("error"), MB_OK);
#endif
		return false;
	}

	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : テクスチャデータの後始末をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::Finalize(void)
{
	if (m_pTexInfo) {
		if (--(m_pTexInfo->dwReference) == 0) {
			SAFE_RELEASE(m_pTexInfo->pTexture);
			free((void*)m_pTexInfo->pszFName);
			for (TEX_INFO* p = m_pTexInfoTop; p; p = p->pNext) {
				if (m_pTexInfo == p) {
					if (p->pPrev)
						m_pTexInfo->pPrev->pNext = p->pNext;
					else
						m_pTexInfoTop = p->pNext;
					if (p->pNext)
						p->pNext->pPrev = p->pPrev;
					break;
				}
			}
			delete m_pTexInfo;
		}
		m_pTexture = NULL;
		m_pTexInfo = NULL;
	}
	m_pDevice = NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : テクスチャ読み込み
//	Description : テクスチャデータを読み込む
//	Arguments   : pDevice  / デバイス
//				  pszFName / テクスチャファイル名
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
HRESULT CTexture::Load(const LPDIRECT3DDEVICE9 pDevice, const LPCTSTR pszFName)
{
	if (pDevice == NULL) {
		return E_POINTER;
	}

	// デバイス保存
	m_pDevice = pDevice;

//	Finalize();
	
	// ファイル名変換
	int strLen = lstrlen(pszFName);
	char* fname = new char[strLen + 1];
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, 0, pszFName, strLen, fname, strLen + 1, NULL, NULL);
	fname[strLen] = '\0';
#else
	lstrcpy(fname, pszFName);
	fname[strLen] = '\0';
#endif

	// ファイル名が無い場合も正常終了
	if (fname == NULL || *fname == '\0') {
		m_pTexture = NULL;
		delete fname;
		return S_OK;
	}

	// テクスチャプールをチェック
	for (TEX_INFO* p = m_pTexInfoTop; p; p = p->pNext) {
		if (_stricmp(p->pszFName, fname) == 0) {
			++(p->dwReference);
			m_pTexture = p->pTexture;
			m_pTexInfo = p;
			delete fname;
			return S_OK;
		}
	}

	// ファイル名の複製を作成
	LPSTR pszDup = _strdup(fname);
	if (pszDup == NULL) {
		delete fname;
		return E_OUTOFMEMORY;
	}

	// テクスチャファイルを読み込む
	D3DXIMAGE_INFO info;
	HRESULT hr = D3DXCreateTextureFromFileExA(pDevice, fname,
		D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, &info, NULL, &m_pTexture);
	if (FAILED(hr)) {
		free(pszDup);
		delete fname;
		return hr;
	}

	// テクスチャ プールに追加
	m_pTexInfo = new TEX_INFO;
	if (m_pTexInfo == NULL) {
		free(pszDup);
		delete fname;
		return E_OUTOFMEMORY;
	}

	m_pTexInfo->pPrev = NULL;
	m_pTexInfo->pNext = m_pTexInfoTop;
	if (m_pTexInfoTop)
		m_pTexInfoTop->pPrev = m_pTexInfo;
	m_pTexInfoTop = m_pTexInfo;

	m_pTexInfo->pTexture = m_pTexture;
	m_pTexInfo->pszFName = pszDup;
	m_pTexInfo->info = info;
	m_pTexInfo->dwReference = 1;
	m_offset = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2((float)info.Width, (float)info.Height);
	
	delete fname;

	return hr;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : テクスチャデータを更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CTexture::Update(void)
{
	m_vtx[0].vtx.x = m_pos.x;
	m_vtx[0].vtx.y = m_pos.y;
	m_vtx[0].vtx.z = m_pos.z;
	m_vtx[1].vtx.x = m_pos.x + m_size.x;
	m_vtx[1].vtx.y = m_pos.y;
	m_vtx[1].vtx.z = m_pos.z;
	m_vtx[2].vtx.x = m_pos.x;
	m_vtx[2].vtx.y = m_pos.y + m_size.y;
	m_vtx[2].vtx.z = m_pos.z;
	m_vtx[3].vtx.x = m_pos.x + m_size.x;
	m_vtx[3].vtx.y = m_pos.y + m_size.y;
	m_vtx[3].vtx.z = m_pos.z;
}


//========================================================================================
//	End of File
//========================================================================================