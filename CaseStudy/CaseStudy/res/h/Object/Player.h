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
#include "../Object/Object2Dsub.h"

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
#define PLAYER_SIZE_X (200)
#define PLAYER_SIZE_Y (250)

#define PLAYER_ANIME_SIZE_X	(10)	// テクスチャの分割数
#define PLAYER_ANIME_SIZE_Y	(2)

#define PLAYER_ANIME_STOP	(1)
#define PLAYER_ANIME_MOVE	(6)
#define PLAYER_ANIME_JUMP	(2)

#define PLAYER_MOVE_SPD		(10)

enum PLAYER_PHASE
{


};
enum PLAYER_STATE
{
	P_STOP = 0,
	P_MOVE,
	P_JUMP,
};

enum PLAYER_TYPE
{
	P_TYPE_PLAYER = 0,
	P_TYPE_OTHER,
};

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CPlayer : public CObject2Dsub
{
private:
	bool	m_bStop;	// 動いてるかどうか
	int		m_nState;	// プレイヤーのステート（動きについて）
	int		m_nType;	// プレイヤーの種類（操作するやつかその他か）
public:
	CPlayer();
	void Update();
	void moveControllerPlayer();					// 動き（プレイヤー）これクラスにした方がいいのかな
	void Animation();								// アニメ            これクラスにした方がいいのかな
	static CPlayer* Create(const LPCTSTR pszFName);	// 生成
	virtual void Release();							// 破棄
};
//========================================================================================
//	End of File
//========================================================================================