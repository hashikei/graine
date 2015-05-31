//========================================================================================
//		File        : Texture.h
//		Program     : テクスチャクラス
//
//		Description : テクスチャクラスの定義
//					  原点：テクスチャの左上頂点
//
//		History     : 2014/06/18	作成開始
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
#include <tchar.h>
#include "../System/System.h"
#include "../System/Graphics.h"

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
// 頂点フォーマット( 頂点座標 / 反射光 / テクスチャ座標 )
#define FVF_VERTEX2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX3D	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

const D3DXVECTOR4	DEFAULT_VTX2D_VTX[4]		=	{	D3DXVECTOR4(               0.0f,                 0.0f, 0.0f, 1.0f),		// デフォルト頂点座標(画面全体)
														D3DXVECTOR4((float)SCREEN_WIDTH,                 0.0f, 0.0f, 1.0f),
														D3DXVECTOR4(               0.0f, (float)SCREEN_HEIGHT, 0.0f, 1.0f),
														D3DXVECTOR4((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, 1.0f),
													};
const D3DXVECTOR3	DEFAULT_VTX3D_VTX[4]		=	{	D3DXVECTOR3(               0.0f,                 0.0f, 0.0f),		// デフォルト頂点座標(画面全体)
														D3DXVECTOR3((float)SCREEN_WIDTH,                 0.0f, 0.0f),
														D3DXVECTOR3(               0.0f, (float)SCREEN_HEIGHT, 0.0f),
														D3DXVECTOR3((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f),
													};
const D3DCOLOR		DEFAULT_VTX_COL_W			=	D3DCOLOR_RGBA(255, 255, 255, 255);		// デフォルト反射光(白色無透明)
const D3DCOLOR		DEFAULT_VTX_COL_W_ALPHA		=	D3DCOLOR_RGBA(255, 255, 255, 0);		// デフォルト反射光(白色透明)
const D3DCOLOR		DEFAULT_VTX_COL_B			=	D3DCOLOR_RGBA(0, 0, 0, 255);			// デフォルト反射光(黒色無透明)
const D3DCOLOR		DEFAULT_VTX_COL_B_ALPHA		=	D3DCOLOR_RGBA(0, 0, 0, 0);				// デフォルト反射光(黒色透明)
const D3DXVECTOR2	DEFAULT_VTX_UV[4]			=	{	D3DXVECTOR2(0.0f, 0.0f),			// デフォルトテクスチャ座標(画像全体)
														D3DXVECTOR2(1.0f, 0.0f),
														D3DXVECTOR2(0.0f, 1.0f),
														D3DXVECTOR2(1.0f, 1.0f),
													};

//――――――――――――――――――――――――――――――――――――――――――――
// マクロ定義
//――――――――――――――――――――――――――――――――――――――――――――
#define D3DCOLOR_RGBA_TO_R(x)		((x >> 16) & 0xff)	// R値を抽出
#define D3DCOLOR_RGBA_TO_G(x)		((x >> 8) & 0xff)	// G値を抽出
#define D3DCOLOR_RGBA_TO_B(x)		(x & 0xff)			// B値を抽出
#define D3DCOLOR_RGBA_TO_RGB(x)		(D3DXVECTOR3((float)D3DCOLOR_RGBA_TO_R(x), (float)D3DCOLOR_RGBA_TO_G(x), (float)D3DCOLOR_RGBA_TO_B(x)))	// RGB値を抽出
#define D3DCOLOR_RGBA_TO_ALPHA(x)	(((x >> 24) & 0xff))	// アルファ値を抽出

//――――――――――――――――――――――――――――――――――――――――――――
// 構造体定義
//――――――――――――――――――――――――――――――――――――――――――――
typedef struct _tVertex2D
{
	D3DXVECTOR4	vtx;		// 頂点座標
	D3DCOLOR	col;		// 反射光
	D3DXVECTOR2	uv;			// テクスチャ座標
} VERTEX2D;

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CTexture
{
// ===== メンバ変数
protected:
	// テクスチャデータリスト
	static struct TEX_INFO
	{
		struct TEX_INFO*	pPrev;			// 一つ前のデータ
		struct TEX_INFO*	pNext;			// 一つ後のデータ
		LPDIRECT3DTEXTURE9	pTexture;		// テクスチャ
		LPCSTR				pszFName;		// テクスチャファイル名
		D3DXIMAGE_INFO		info;			// テクスチャ情報
		DWORD				dwReference;	// 同一テクスチャの使用数
	}*					m_pTexInfoTop;
	
	LPDIRECT3DDEVICE9	m_pDevice;		// デバイス
	LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャ
	TEX_INFO*			m_pTexInfo;		// テクスチャデータ

	VERTEX2D			m_vtx[4];		// 頂点情報
	D3DXVECTOR3			m_pos;			// 現在位置(テクスチャの左上頂点)
	D3DXVECTOR2			m_size;			// サイズ
	D3DXVECTOR2			m_offset;		// 描画オフセット

// ===== メンバ関数
public:
	CTexture();				// コンストラクタ
	virtual ~CTexture();	// デストラクタ

	void Init();		// デフォルト値で初期化
	void Init(const D3DXVECTOR2& size);	// 初期化
	void Init(const float width, const float height);	// 初期化
	void Init(const D3DXVECTOR2& size, const D3DXVECTOR3& pos);	// 初期化
	void Init(const float width, const float height,
				const float x, const float y, const float z);	// 初期化
	void Uninit();		// 後始末
	void Draw();		// 描画(アルファ無効)
	void DrawAlpha();	// 描画(アルファ有効)

	static CTexture* Create(const LPCTSTR pszFName);		// 生成
	void Release();											// 破棄
	
	// ----- セッター
	void SetColor(const D3DXVECTOR3& color);			// 頂点カラー設定
	void SetAlpha(float alpha);							// 透過度設定
	
	// ----- ゲッター
	D3DXVECTOR3& GetPosition() {return m_pos;}				// 現在位置取得(テクスチャの左上頂点)
	float GetPosX() const {return m_pos.x;}					// 現在位置取得(X座標)
	float GetPosY() const {return m_pos.y;}					// 現在位置取得(Y座標)
	float GetPosZ() const {return m_pos.z;}					// 現在位置取得(Z座標)
	D3DXVECTOR2& GetSize() {return m_size;}					// サイズ取得
	float GetWidth() const {return m_size.x;}				// テクスチャ幅取得
	float GetHeight() const {return m_size.y;}				// テクスチャ高さ取得
	D3DXVECTOR3 GetColor() {return D3DCOLOR_RGBA_TO_RGB(m_vtx[0].col);}	// 頂点カラー取得
	float GetAlpha() const {return (float)D3DCOLOR_RGBA_TO_ALPHA(m_vtx[0].col);}	// 透過度取得
	float GetLeftPos() const {return m_pos.x;}				// オブジェクトの左端座標を取得
	float GetRightPos() const {return m_pos.x + m_size.x;}	// オブジェクトの右端座標を取得
	float GetTopPos() const {return m_pos.y;}				// オブジェクトの上端座標を取得
	float GetBottomPos() const {return m_pos.y + m_size.y;}	// オブジェクトの下端座標を取得

	// ----- アフィン変換
	void SetPosition(const D3DXVECTOR3& pos);			// 現在位置更新(テクスチャの左上頂点)
	void SetPosition(const float x, const float y, const float z);		// 現在位置更新(テクスチャの左上頂点)
	void SetPosX(const float x);						// 現在位置更新(X座標)
	void SetPosY(const float y);						// 現在位置更新(Y座標)
	void SetPosZ(const float z);						// 現在位置更新(Z座標)
	void AddPosition(const D3DXVECTOR3& pos);			// 現在位置更新
	void AddPosition(const float x, const float y, const float z);		// 現在位置更新
	void AddPosX(const float x);						// 現在位置更新(X座標)
	void AddPosY(const float y);						// 現在位置更新(Y座標)
	void AddPosZ(const float z);						// 現在位置更新(Z座標)
	void HorizonFlip();									// 左右反転
	void VerticalFlip();								// 上下反転

	// ----- フェード関連
	void FadeColor(int nFade);			// フェード処理(絶対)
	bool FadeInColor(int nFade);		// フェードイン(相対)
	bool FadeOutColor(int nFade);		// フェードアウト(相対)
	void FadeAlpha(int nFade);			// 透過フェード処理(絶対)
	bool FadeInAlpha(int nFade);		// 透過フェードイン(相対)
	bool FadeOutAlpha(int nFade);		// 透過フェードアウト(相対)
	
	void Resize(const D3DXVECTOR2& size);				// テクスチャサイズ変更
	void Resize(const float width, const float height);	// テクスチャサイズ変更
	
	void UVDivision(const int num, const int width, const int height);	// UV値を分割

protected:
	bool Initialize(const LPCTSTR pszFName);	// 初期化
	void Finalize();							// 後始末

	HRESULT Load(const LPDIRECT3DDEVICE9 pDevice, const LPCTSTR pszFName);	// テクスチャ読み込み

	void Update();		// 更新
};

//――――――――――――――――――――――――――――――――――――――――――――
// ユーザ型定義
//――――――――――――――――――――――――――――――――――――――――――――
typedef std::vector<CTexture*>		LPTEXTURE_ARRAY;		// テクスチャクラスのリスト
typedef LPTEXTURE_ARRAY::iterator	LPTEXTURE_ARRAY_IT;		// テクスチャクラスリストのイテレータ


//========================================================================================
//	End of File
//========================================================================================