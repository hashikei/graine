//========================================================================================
//		File        : Object2D.h
//		Program     : 2Dオブジェクトベース
//
//		Description : 2Dオブジェクトベースの定義
//					  原点：ポリゴンの中央
//
//		History     : 2014/06/11	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================

#pragma once

#ifndef __USE_INPUT_OBJECT2D
#define __USE_INPUT_OBJECT2D
#endif

//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../System/PreInclude.h"
#
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include <vector>
#include "../System/System.h"
#include "../System/Graphics.h"
#include "../System/Timer.h"
#include "../Object/Object.h"
#include "../Object/Texture.h"

#ifdef __USE_INPUT_OBJECT2D
#include "../System/Input.h"
#endif

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
const D3DMATERIAL9 DEFAULT_MATERIAL = {	// 汎用マテリアル設定
	{1.0f, 1.0f, 1.0f, 1.0f},		// Diffuse
	{0.1f, 0.1f, 0.1f, 0.1f},		// Ambient
	{0.1f, 0.1f, 0.1f, 0.1f},		// Specular
	{0.1f, 0.1f, 0.1f, 0.1f},		// Emissive
	1.0f							// Power
};

//――――――――――――――――――――――――――――――――――――――――――――
// 構造体定義
//――――――――――――――――――――――――――――――――――――――――――――
typedef struct _tVertex3D
{
	D3DXVECTOR3	vtx;		// 頂点座標
	D3DCOLOR	col;		// 反射光
	D3DXVECTOR2	uv;			// テクスチャ座標
} VERTEX3D;

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CObject2D : public CObject
{
// ===== メンバ変数
protected:
	// 2Dオブジェクトデータリスト
	static struct OBJ2D_INFO
	{
		struct OBJ2D_INFO*	pPrev;			// 一つ前のデータ
		struct OBJ2D_INFO*	pNext;			// 一つ後のデータ
		LPDIRECT3DTEXTURE9	pTexture;		// テクスチャ
		LPCSTR				pszFName;		// テクスチャファイル名
		D3DXIMAGE_INFO		info;			// テクスチャ情報
		DWORD				dwReference;	// 同一テクスチャの使用数
	}*					m_pObj2DInfoTop;
	
	LPDIRECT3DDEVICE9	m_pDevice;		// デバイス
	LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャ
	OBJ2D_INFO*			m_pObj2DInfo;	// 2Dオブジェクトデータ

	VERTEX3D			m_vtx[4];		// 頂点情報
	D3DMATERIAL9		m_material;		// マテリアル
	D3DXVECTOR2			m_size;			// オブジェクトサイズ
	D3DXVECTOR2			m_halfSize;		// オブジェクトサイズ(半分)
	D3DXVECTOR2			m_defSize;		// オブジェクトのデフォルトサイズ
	D3DXVECTOR2			m_offset;		// 描画オフセット
	
// ===== メンバ関数
public:
	CObject2D();			// コンストラクタ
	virtual ~CObject2D();	// デストラクタ

	virtual void Init();		// デフォルト値で初期化
	virtual void Init(const D3DXVECTOR2& size, const D3DXVECTOR3& pos);	// サイズを指定して初期化
	virtual void Uninit();		// 後始末
	virtual void Update();		// 更新
	virtual void Draw();		// 描画(アルファ無効)
	virtual void DrawAlpha();	// 描画(アルファ有効)
	virtual void DrawBillBoard(const D3DXVECTOR3& target);		// ビルボード描画(アルファ無効)
	virtual void DrawBillBoardAlpha(const D3DXVECTOR3& target);	// ビルボード描画(アルファ有効)

	static CObject2D* Create(const LPCTSTR pszFName);	// 生成
	virtual void Release();								// 破棄
	
	// ----- セッター
	virtual void SetColor(const D3DXVECTOR3& color);	// 頂点カラー設定(0～255)
	virtual void SetAlpha(float alpha);					// 透過度設定(0～255)

	// ----- ゲッター
	virtual D3DXVECTOR2 GetSize() const {return m_size;}			// サイズ取得
	virtual float GetWidth() const {return m_size.x;}				// オブジェクト幅取得
	virtual float GetHeight() const {return m_size.y;}				// オブジェクト高さ取得
	virtual D3DXVECTOR2 GetHalfSize() const {return m_halfSize;}	// ハーフサイズ取得
	virtual float GetHalfWidth() const {return m_halfSize.x;}		// ハーフサイズ取得(幅)
	virtual float GetHalfHeight() const {return m_halfSize.y;}		// ハーフサイズ取得(高さ)
	virtual D3DXVECTOR2 GetDefaultSize() const {return m_defSize;}	// デフォルトサイズ取得
	virtual float GetDefaultWidth() const {return m_defSize.x;}		// デフォルトサイズ取得(幅)
	virtual float GetDefaultHeight() const {return m_defSize.y;}	// デフォルトサイズ取得(高さ)
	virtual D3DXVECTOR3 GetColor() const {return D3DCOLOR_RGBA_TO_RGB(m_vtx[0].col);}	// 頂点カラー取得
	virtual float GetAlpha() const {return (float)D3DCOLOR_RGBA_TO_ALPHA(m_vtx[0].col);}	// 透過度取得
	virtual float GetLeftPos() const {return m_pos.x - m_halfSize.x;}		// オブジェクトの左端座標を取得
	virtual float GetRightPos() const {return m_pos.x + m_halfSize.x;}		// オブジェクトの右端座標を取得
	virtual float GetTopPos() const {return m_pos.y + m_halfSize.y;}		// オブジェクトの上端座標を取得
	virtual float GetBottomPos() const {return m_pos.y - m_halfSize.y;}		// オブジェクトの下端座標を取得
	virtual VERTEX3D GetVertex(int num) const {return m_vtx[num];}			// 頂点情報取得
	
	// ----- 行列変換
	virtual void Translation(const D3DXVECTOR3& vec);	// 相対移動
	virtual void TranslationX(float x);					// 相対移動(X座標)
	virtual void TranslationY(float y);					// 相対移動(Y座標)
	virtual void TranslationZ(float z);					// 相対移動(Z座標)
	virtual void Translate(const D3DXVECTOR3& pos);		// 絶対移動
	virtual void TranslateX(float x);					// 絶対移動(X座標)
	virtual void TranslateY(float y);					// 絶対移動(Y座標)
	virtual void TranslateZ(float z);					// 絶対移動(Z座標)
	virtual void Rotation(const D3DXVECTOR3& angle);	// 相対回転
	virtual void RotationX(float angle);				// 相対回転(X軸)
	virtual void RotationY(float angle);				// 相対回転(Y軸)
	virtual void RotationZ(float angle);				// 相対回転(Z軸)
//	virtual void RotationQuaternion(const D3DXVECTOR3* pV, float angle);	// 相対クォータニオン回転
	virtual void Rotate(const D3DXVECTOR3& angle);		// 絶対回転
	virtual void RotateX(float angle);					// 絶対回転(X軸)
	virtual void RotateY(float angle);					// 絶対回転(Y軸)
	virtual void RotateZ(float angle);					// 絶対回転(Z軸)
//	virtual void RotateQuaternion(const D3DXVECTOR3* pV, float angle);		// 絶対クォータニオン回転
	virtual void Scaling(const D3DXVECTOR3& scale);		// 相対拡縮
	virtual void ScalingX(float x);						// 相対拡縮(X方向)
	virtual void ScalingY(float y);						// 相対拡縮(Y方向)
	virtual void Scale(const D3DXVECTOR3& scale);		// 絶対拡縮
	virtual void ScaleX(float x);						// 絶対拡縮(X方向)
	virtual void ScaleY(float y);						// 絶対拡縮(Y方向)

	// ----- フレームアニメーション関連
	virtual void UVDivision(int num, int width, int height);		// UV値を分割

	// ----- フェード関連
	virtual void FadeColor(int nFade);			// フェード処理(絶対)
	virtual bool FadeInColor(int nFade);		// フェードイン(相対)
	virtual bool FadeOutColor(int nFade);		// フェードアウト(相対)
	virtual void FadeAlpha(int nFade);			// 透過フェード処理(絶対)
	virtual bool FadeInAlpha(int nFade);		// 透過フェードイン(相対)
	virtual bool FadeOutAlpha(int nFade);		// 透過フェードアウト(相対)

	virtual void Resize(const D3DXVECTOR2& size);	// オブジェクトサイズ変更
	virtual void Resize(const float width, const float height);	// オブジェクトサイズ変更

protected:
	virtual bool Initialize(const LPCTSTR pszFName);	// 初期化
	virtual void Finalize();							// 後始末
	
	virtual HRESULT Load(const LPDIRECT3DDEVICE9 pDevice, const LPCTSTR pszFName);	// テクスチャ読み込み
	
#ifdef __USE_INPUT_OBJECT2D
	// ----- マウス操作関連
	virtual bool CheckMouseOverBox();					// マウスとの当たり判定(四角形)
	virtual bool CheckMouseOverCircle(float radius);	// マウスとの当たり判定(円)
#endif
};

//――――――――――――――――――――――――――――――――――――――――――――
// ユーザ型定義
//――――――――――――――――――――――――――――――――――――――――――――
typedef std::vector<CObject2D*>		LPOBJECT2D_ARRAY;		// オブジェクト2Dクラスのリスト
typedef LPOBJECT2D_ARRAY::iterator	LPOBJECT2D_ARRAY_IT;	// オブジェクト2Dクラスリストのイテレータ


//========================================================================================
//	End of File
//========================================================================================