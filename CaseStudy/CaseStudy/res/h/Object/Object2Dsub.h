//========================================================================================
//		File        : CObject2Dsub.h
//		Program     : オブジェクトベースの拡張
//
//		Description : オブジェクトベースの拡張、なんとなくベースに触りたくなかった。
//
//		History     : 2015/5/6	作成開始
//						   
//
//																Author : きんたまズ
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
#include "../System/Timer.h"

#include "../Object/Object2D.h"

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――

#define		GRAVITY_DEFAULT	(9.8f)	// 死ぬほど早いしゲームっぽくない
#define		GRAVITY_CASE_1	(0.1f)
#define		GRAVITY_CASE_2	(0.1f)
#define		GRAVITY_ZERO	(0.00f)

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CObject2Dsub : public CObject2D
{
protected:
	int			m_nPhase;			// 状態

	D3DXVECTOR3 m_accel;			// 動く方向
	
	// ----- アニメーション用
	CTimer*		m_pTimer;
	float		m_fLastTime;
	int			m_nCurrentFrame;
	bool		m_bAnimeInit;		// アニメの初期化フラグ

	float		m_fGravity;			// 重力
	float		m_fGAccel;			// 落下速度
public:
	CObject2Dsub();			// コンストラクタ

	virtual void Update();
	void TimeAnimation(int minNo, int maxNo, int width, int height,float time);	// 秒数ごとのパラパラアニメ

	// ----- セッター
	void SetGravity(float g);

	// ----- ゲッター
	float GetGravity(){ return m_fGravity; }
};
//========================================================================================
//	End of File
//========================================================================================