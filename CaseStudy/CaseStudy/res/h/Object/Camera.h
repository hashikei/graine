//========================================================================================
//		File        : Camera.h
//		Program     : カメラクラス
//
//		Description : カメラクラスの定義
//
//		History     : 2013/11/26	作成開始
//					  2014/05/23	ファイル名変更
//						   07/02	Objectクラスから継承
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
#include "../Object/Object.h"

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CCamera : public CObject
{
// ===== メンバ定数
protected:
	// カメラ種別
	enum _eType {
		TYPE_FIXED = 0,		// 定点
		TYPE_FIRSTPERSON,	// 一人称
		TYPE_THIRDPERSON,	// 三人称
	
		MAX_TYPE			// カメラ種別数
	};

// ===== メンバ変数
protected:
	D3DXVECTOR3	m_eye;		// 視点座標
	D3DXVECTOR3	m_look;		// 注視点座標
	D3DXVECTOR3	m_up;		// アップベクトル
	D3DXVECTOR3	m_newEye;	// 次視点座標
	D3DXVECTOR3	m_newLook;	// 次注視座標
	D3DXVECTOR3	m_newUp;	// 次アップベクトル
	int			m_type;		// カメラ種別

// ===== メンバ関数
public:
	CCamera();
	virtual ~CCamera();

	virtual void Init();		// 初期化
	virtual void Uninit();		// 後始末
	virtual void Update();		// 更新
	virtual void Draw();		// 描画
	virtual void Release();		// 解放

	static CCamera* Create();	// カメラ生成

	// ----- セッター
	void SetParameter(D3DXVECTOR3& eye, D3DXVECTOR3& look, D3DXVECTOR3& up);	// カメラパラメータセット
	void SetParameter(const D3DXVECTOR3& eye, const D3DXVECTOR3& look, const D3DXVECTOR3& up);	// カメラパラメータセット
	void SetEye(D3DXVECTOR3& eye);							// 視点座標セット
	void SetEye(const D3DXVECTOR3& eye);					// 視点座標セット
	void SetEyeX(float x) {m_eye.x = x;}					// 視点X座標セット
	void SetEyeY(float y) {m_eye.y = y;}					// 視点Y座標セット
	void SetEyeZ(float z) {m_eye.z = z;}					// 視点Z座標セット
	void SetLook(D3DXVECTOR3& look);						// 注視点座標セット
	void SetLook(const D3DXVECTOR3& look);					// 注視点座標セット
	void SetLookX(float x) {m_look.x = x;}					// 注視点X座標セット
	void SetLookY(float y) {m_look.y = y;}					// 注視点Y座標セット
	void SetLookZ(float z) {m_look.z = z;}					// 注視点Z座標セット
	void SetUp(D3DXVECTOR3& up);							// 上方ベクトルセット
	void SetUp(const D3DXVECTOR3& up);						// 上方ベクトルセット
	void SetUpX(float x) {m_up.x = x;}						// 上方ベクトル(X軸方向)セット
	void SetUpY(float y) {m_up.y = y;}						// 上方ベクトル(Y軸方向)セット
	void SetUpZ(float z) {m_up.z = z;}						// 上方ベクトル(Z軸方向)セット
	void SetType(int type) {m_type = type;}					// カメラ種別セット
	
	// ----- ゲッター
	D3DXVECTOR3 GetEye() const	{return m_eye;}		// 視点座標取得
	D3DXVECTOR3 GetLook() const	{return m_look;}	// 注視点座標取得
	D3DXVECTOR3 GetUp() const	{return m_up;}		// アップベクトル取得

private:
	virtual bool	Initialize();		// 初期化
	virtual void	Finalize();			// 後始末
};


//========================================================================================
//	End of File
//========================================================================================