//========================================================================================
//		File        : Player.h
//		Program     : プレイヤー
//
//		Description : プレイヤー個別の動き
//
//		History     : 2015/05/6	作成開始
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
#include "../System/Input.h"
#include "../System/System.h"
#include "../Object/Character.h"
#include "../Object/Stage.h"

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――

#define PLAYER_ANIME_SIZE_X	(12)	// テクスチャの分割数
#define PLAYER_ANIME_SIZE_Y	(10)

const float PLAYER_POS_DEFAULT_X = -300;
const float PLAYER_POS_DEFAULT_Y = 100;

const float PLAYER_SIZE_X = 128;
const float PLAYER_SIZE_Y = 128;

#define PLAYER_ANIME_STOP	(1)
#define PLAYER_ANIME_MOVE	(6)
#define PLAYER_ANIME_JUMP	(2)

static const float PLAYER_MOVE_SPD = 7;	// スピード

enum PLAYER_TYPE
{
	P_TYPE_PLAYER = 0,
	P_TYPE_OTHER,
	P_TYPE_THROW_READY_READY,
	P_TYPE_THROW_READY,
	P_TYPE_THROW,

	P_TYPE_FLOWER,

	P_TYPE_WAIT,

	P_TYPE_DELETE,

	MAX_PLAYER_TYPE
};

enum PLAYER_GRANE
{
	PLAYER_NORMAL = 0,
	PLAYER_ARROW,
	PLAYER_JACK,
	PLAYER_STONE,

	MAX_GRANE,
};

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CPlayer : public CCharacter
{
private:
	const float JUMP_DEFAULT = 20;		// ジャンプ速度の初速度
	const float JUMP_GRAVITY = 0.1f;		// ジャンプ速度の減速

	const double WAIT_LIMIT_TIME = 3;		// 待ち状態になる時間

	const double FALL_LIMIT_TIME = 1;		// 待ち状態になる時間

	const float WAIT_LENGTH = 1000;

	const float PLAYER_LENGTH = 80;		// 操作するやつとついてくる奴の距離

	const float PLAYER_ARROW_SIZE = 1.1f;
	const float PLAYER_JACK_SIZE = 1.3f;
	const float PLAYER_STORN_SIZE = 1.8f;

	static const LPCTSTR TACTILE_TEX_FILENAME[];		// 触覚テクスチャのファイル名

protected:

	int		m_PrevStatus;

	int		m_nNo;		// 識別番号
	int		m_nGrane;	// 種の種類
	int		m_nType;	// プレイヤーの種類（操作するやつかその他か）

	int		m_nPrevRL;
	int		m_nRL;		// 向いてる方向

	float	m_fSpeed;		// 移動速度(ランダム)
	float	m_fJumpSpeed;	// ジャンプの速度

	bool	m_bDelete;
	bool	m_bCol;			//当たってるかどうか
	bool	m_bChangeGrane; // 

	// アニメーション関係
	bool m_bAnimeFall;

	CCharacter*		m_pTactile;			// 触覚
	CCharacter*		m_pTactileTable[MAX_GRANE];	// 触覚リスト
	LPCTSTR			m_lpShockTex;		// Playerのテクスチャ

	CStage*	m_pStage;	// 当たり判定を行うフィールド

	CPlayer*	m_pPlayer;	// 操作設定のPlayer(操作設定の場合はNULL)

	D3DXVECTOR3 m_vFlower;	// 花の咲く向き

	// 時間関係
	double m_lastTime;
	double m_nowTime;

	// 投げ用
	int		m_nThrowNo;

public:
	CPlayer();
	virtual void Init();						// 初期化
	virtual void Init(const D3DXVECTOR3& pos);	// サイズを指定して初期化
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void moveControllerPlayer();					// 動き（プレイヤー	これクラスにした方がいいのかな
	virtual void moveControllerOther();						// 動き（集団）		これクラスにした方がいいのかな
	virtual void moveControllerThrowReady();
	virtual void moveControllerThrowReadyReady();
	virtual void moveControllerThrow();
	virtual void SoundEffect();
	virtual void Animation();								// アニメ			これクラスにした方がいいのかな
	static CPlayer* Create(const LPCTSTR pszFName);	// 生成

	// ----- セッター
	void SetNo(int no){ m_nNo = no; }					// プレイヤーの識別番号
	void SetGrane(int grane){ m_nGrane = grane; m_bChangeGrane = false; m_pTactile = m_pTactileTable[grane]; }
	void SetType(int type){ m_nType = type; }	// プレイヤーの操作設定
	void SetThrowNo(int no){ m_nThrowNo = no; }
	void SetPlayer(CPlayer* p){ m_pPlayer = p; }		// 操作するPlayer情報設定
	void SetStage(CStage* f){ m_pStage = f; }			// フィールド情報設定
	void SetLastTime(){ m_lastTime = CTimer::GetTime(); }

	void SetPrevStatus(int no){ m_status = no; }

	void EnableDelete(){ m_bDelete = true; }
	void DisableDelete(){ m_bDelete = false; }

	void EnableCol(){ m_bCol = true; }
	void DisableCol(){ m_bCol = false; }

	// ----- ゲッター
	int GetStatus(){ return m_status; }
	int GetNo(){ return m_nNo; }						// プレイヤーの識別番号
	int GetType(){ return m_nType; }					// 種類取得
	int GetRL(){ return m_nRL; }
	bool GetDelete(){ return m_bDelete; }
	bool GetCol(){ return m_bCol; }

private:
	bool Initialize(const LPCTSTR pszFName);	// 初期化
	void Finalize();		// 後始末
};
//========================================================================================
//	End of File
//========================================================================================