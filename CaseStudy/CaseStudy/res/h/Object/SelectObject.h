//========================================================================================
//		File        : SelectObject.h
//		Program     : シーン遷移システム
//
//		Description : シーン遷移システムの定義
//
//		History     : 2015/02/20	作成開始
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
#include <vector>
#include "../Object/Character.h"

enum SelectObject_Status
{
	S_STATUS_WAIT = 0,
	S_STATUS_LEFT,
	S_STATUS_RIGHT,
};

enum SelectClear
{
	SC_CLEAR_CLEAR = 0,
	SC_CLEAR_FALSE,
	SC_FALSE_CLEAR,
	SC_FALSE_FALSE,
};

#define SELECT_ANIME_SIZE_X		(12)	// テクスチャの分割数
#define SELECT_ANIME_SIZE_Y		(10)
#define PLAYER_INIT_POS_X		(SCREEN_WIDTH / 2)
#define PLAYER_INIT_POS_Y		((SCREEN_HEIGHT / 2) + 50)
#define LEFT_ARROW_INIT_POS_X	(SCREEN_RIGHT - 384)
#define LEFT_ARROW_INIT_POS_Y	100
#define RIGHT_ARROW_INIT_POS_X	(SCREEN_RIGHT + 384)
#define RIGHT_ARROW_INIT_POS_Y	100
#define ROGO_INIT_POS_X			(SCREEN_WIDTH / 2)
#define ROGO_INIT_POS_Y			100
#define ROGO_SCREEN_OUT_POS_X	(SCREEN_LEFT - 192)
#define ROGO_SCREEN_OUT_POS_Y	(SCREEN_TOP + 192)
#define STAGE_INIT_POS_X		(SCREEN_WIDTH / 2)
#define STAGE_INIT_POS_Y		(SCREEN_HEIGHT + SCREEN_TOP)
#define MOVE_COUNT				10

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CSelectObject : public CCharacter
{
	// ===== メンバ定数
private:
	// ===== メンバ変数
private:	
	int		m_nClearLeft[5];
	int		m_nClearRight[5];
	bool	m_bAnime;
	// ===== メンバ関数
public:
	CSelectObject();
	virtual ~CSelectObject();

	static CSelectObject* Create(const LPCTSTR pszFName);	// 生成
	bool BGUpdate(int nNo, int nStatus, bool bClear, bool bAnime);
	void PlayerUpdate(bool bAnime, int nStatus);		// シーン遷移 仮2 (画面に出現)
	void TactileUpdate(bool bAnime, int nStatus);
	int ArrowUpdate(int nNo, int nStatus);
	void RogoUpdate(int nNo, int nStatus, int nStage);
	bool StageUpdate(int nNo,int nStatus, int nStage);
	void ClearSave(bool bStage1,bool bStage2,bool bStage3,bool bStage4,bool bStage5);
	void StageChange(int nNo,int nClear);
};

//========================================================================================
//	End of File
//========================================================================================