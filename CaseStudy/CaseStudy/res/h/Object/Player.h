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

#define PLAYER_ANIME_SIZE_X	(4)	// テクスチャの分割数
#define PLAYER_ANIME_SIZE_Y	(7)

const float PLAYER_POS_DEFAULT_X = -200;
const float PLAYER_POS_DEFAULT_Y = 100;

const float PLAYER_SIZE_X = 128;
const float PLAYER_SIZE_Y = 128;

#define PLAYER_ANIME_STOP	(1)
#define PLAYER_ANIME_MOVE	(6)
#define PLAYER_ANIME_JUMP	(2)

static const float PLAYER_MOVE_SPD = 10;	// スピード最低値

const float PLAYER_LENGTH	= 80;	// 操作するやつとついてくる奴の距離		

enum PLAYER_TYPE
{
	P_TYPE_PLAYER = 0,
	P_TYPE_OTHER,
	P_TYPE_THROW_READY_READY,
	P_TYPE_THROW_READY,
	P_TYPE_THROW,

	P_TYPE_FLOWER,

	P_TYPE_DELETE,

	MAX_PLAYER_TYPE
};

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CPlayer : public CCharacter
{
private:
	const float JUMP_DEFAULT	= 10.f;	// ジャンプ速度の初速度
	const float JUMP_GRAVITY	= 0.1f;		// ジャンプ速度の減速
protected:
	
	int		m_nNo;		// 識別番号
	int		m_nType;	// プレイヤーの種類（操作するやつかその他か）

	int		m_nPrevRL;
	int		m_nRL;		// 向いてる方向

	float	m_fSpeed;		// 移動速度(ランダム)
	float	m_fJumpSpeed;	// ジャンプの速度

	bool	m_bDelete;
	bool	m_bCol;			//当たってるかどうか

	CStage*	m_pStage;	// 当たり判定を行うフィールド

	CPlayer*	m_pPlayer;	// 操作設定のPlayer(操作設定の場合はNULL)

	// 投げ用
	int		m_nThrowNo;

public:
	CPlayer();
	virtual void Init();									// 初期化
	virtual void Uninit();
	virtual void Update();
	virtual void moveControllerPlayer();					// 動き（プレイヤー	これクラスにした方がいいのかな
	virtual void moveControllerOther();						// 動き（集団）		これクラスにした方がいいのかな
	virtual void moveControllerThrowReady();
	virtual void moveControllerThrowReadyReady();
	virtual void moveControllerThrow();
	virtual void Animation();								// アニメ			これクラスにした方がいいのかな
	static CPlayer* Create(const LPCTSTR pszFName);	// 生成

	// ----- セッター
	void SetNo(int no){m_nNo = no;}					// プレイヤーの識別番号
	void SetPlayerType(int type){m_nType = type;}	// プレイヤーの操作設定
	void SetThrowNo(int no){m_nThrowNo = no;}
	void SetPlayer(CPlayer* p){m_pPlayer = p;}		// 操作するPlayer情報設定
	void SetStage(CStage* f){m_pStage = f;}			// フィールド情報設定

	void EnableDelete(){m_bDelete = true;}
	void DisableDelete(){m_bDelete = false;}

	void EnableCol(){m_bCol = true;}
	void DisableCol(){m_bCol = false;}

	// ----- ゲッター
	int GetStatus(){return m_status;}
	int GetNo(){return m_nNo;}						// プレイヤーの識別番号
	int GetType(){return m_nType;}					// 種類取得
	int GetRL(){return m_nRL;}
	bool GetDelete(){return m_bDelete;}
	bool GetCol(){return m_bCol;}
};
//========================================================================================
//	End of File
//========================================================================================