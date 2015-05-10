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

const float PLAYER_MOVE_SPD = 5;	// スピード最低値

const float PLAYER_LENGTH	= 100;	// 操作するやつとついてくる奴の距離		

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
	int		m_nNo;		// 識別番号
	int		m_nState;	// プレイヤーのステート（動きについて）
	int		m_nType;	// プレイヤーの種類（操作するやつかその他か）

	float	m_fSpeed;	// 移動速度(ランダム)

	CPlayer*	m_pPlayer;	// 操作設定のPlayer(操作設定の場合はNULL)

public:
	CPlayer();
	void Update();
	void moveControllerPlayer();					// 動き（プレイヤー	これクラスにした方がいいのかな
	void moveControllerOther();						// 動き（集団）		これクラスにした方がいいのかな
	void Animation();								// アニメ			これクラスにした方がいいのかな
	static CPlayer* Create(const LPCTSTR pszFName);	// 生成

	// ----- セッター
	void SetNo(int no){m_nNo = no;}					// プレイヤーの識別番号
	void SetPlayerType(int type){m_nType = type;}	// プレイヤーの操作設定
	void SetPlayer(CPlayer* p){m_pPlayer = p;}		// 操作するPlayer情報設定

	// ----- ゲッター
	int GetNo(){return m_nNo;}						// プレイヤーの識別番号
	int GetType(){return m_nType;}					// 種類取得
};
//========================================================================================
//	End of File
//========================================================================================