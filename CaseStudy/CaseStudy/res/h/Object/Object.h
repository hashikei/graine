//========================================================================================
//		File        : CObject.h
//		Program     : オブジェクトベース
//
//		Description : オブジェクトベースの定義
//
//		History     : 2014/06/11	作成開始
//						   06/18	メンバ変数追加
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
#include "../System/System.h"

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CObject
{
// ===== メンバ変数
protected:
	D3DXMATRIX	m_world;		// ワールド変換行列
	D3DXVECTOR3	m_pos;			// 現在位置
	D3DXVECTOR3	m_rot;			// 回転角(0～359)
	D3DXVECTOR3	m_scale;		// 拡縮率
	bool		m_bExist;		// 存在フラグ
	bool		m_bDraw;		// 描画フラグ

// ===== メンバ関数
public:
	CObject();				// コンストラクタ
	virtual ~CObject();		// デストラクタ

	virtual void Init() = 0;		// 初期化
	virtual void Uninit() = 0;		// 後始末
	virtual void Update() = 0;		// 更新
	virtual void Draw() = 0;		// 描画
	virtual void Release();			// 解放

	// ----- ゲッター
	virtual D3DXMATRIX GetMatrix() const {return m_world;}	// ワールド変換行列取得
	virtual D3DXVECTOR3 GetPosition() const {return m_pos;}	// 現在位置取得
	virtual float GetPosX() const {return m_pos.x;}			// 現在位置取得(X座標)
	virtual float GetPosY() const {return m_pos.y;}			// 現在位置取得(Y座標)
	virtual float GetPosZ() const {return m_pos.z;}			// 現在位置取得(Z座標)
	virtual D3DXVECTOR3 GetRotation() const {return m_rot;}	// 回転角取得
	virtual float GetRotX() const {return m_rot.x;}			// 回転角取得(X方向)
	virtual float GetRotY() const {return m_rot.y;}			// 回転角取得(Y方向)
	virtual float GetRotZ() const {return m_rot.z;}			// 回転角取得(Z方向)
	virtual D3DXVECTOR3 GetScale() const {return m_scale;}	// 拡縮率取得
	virtual float GetScaleX() const {return m_scale.x;}		// 拡縮率取得(X方向)
	virtual float GetScaleY() const {return m_scale.y;}		// 拡縮率取得(Y方向)
	virtual float GetScaleZ() const {return m_scale.z;}		// 拡縮率取得(Z方向)
	virtual bool GetExistFlg() const {return m_bExist;}		// 存在フラグ取得
	virtual bool GetDrawFlg() const {return m_bDraw;}		// 描画フラグ取得
	
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
	virtual void ScalingZ(float z);						// 相対拡縮(Z方向)
	virtual void Scale(const D3DXVECTOR3& scale);		// 絶対拡縮
	virtual void ScaleX(float x);						// 絶対拡縮(X方向)
	virtual void ScaleY(float y);						// 絶対拡縮(Y方向)
	virtual void ScaleZ(float z);						// 絶対拡縮(Z方向)

	virtual void Enable() {m_bExist = true;}			// 存在を有効化
	virtual void Disable() {m_bExist = false;}			// 存在を無効化
	virtual void RenderEnable() {m_bDraw = true;}		// 描画機能を有効化
	virtual void RenderDisable() {m_bDraw = false;}		// 描画機能を無効化

protected:
	virtual bool Initialize();		// 初期化
	virtual void Finalize() = 0;	// 後始末
	virtual void RecalcMatrix();	// ワールド変換行列再計算
};


//========================================================================================
//	End of File
//========================================================================================