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
#include "../Object/Camera.h"

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CGameCamera : public CCamera
{
private:
	static const float DEFAULT_CAMERA_POS_Z;
	static const D3DXVECTOR3 CAMERA_DEFAULT_POS;
	static const float C_LENGTH_0;
	static const float C_LENGTH_1;
	static const float C_LENGTH_2;
	static const float C_MOVE_SPD;

private:
// ===== メンバ変数

	int				m_nPhase;	// フェイズ
	bool			m_bMove;
	D3DXVECTOR3		m_vNextEye;	// 動く先
// ===== メンバ関数
public:
	CGameCamera();
	virtual ~CGameCamera();

	virtual void Init();		// 初期化
	virtual void Uninit();		// 後始末
	virtual void Update();		// 更新
	virtual void Draw();		// 描画
	virtual void Release();		// 解放

	void cameraControllMain();	// カメラ操作（メイン中）

	static CGameCamera* Create();	// カメラ生成

	// ----- セッター
	void SetPhase(int no){m_nPhase = no;}
	void SetNextEye(D3DXVECTOR3 eye){m_vNextEye = eye; m_vNextEye.z = DEFAULT_CAMERA_POS_Z;}

	// -----ゲッター
	int GetPhase(){return m_nPhase;}
	D3DXVECTOR3 GetNextEye(){return m_vNextEye;}

private:
	void			SyncEyeLook();		// カメラの位置と注視点の同期
	virtual bool	Initialize();		// 初期化
	virtual void	Finalize();			// 後始末
};


//========================================================================================
//	End of File
//========================================================================================