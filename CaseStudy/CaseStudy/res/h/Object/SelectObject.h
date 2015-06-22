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

enum Select_Stage
{
	S_STAGE_1 = 0,
	S_STAGE_2,

	S_STAGE_MAX,
};

#define PLAYER_INIT_POS_X		0
#define PLAYER_INIT_POS_Y		-10
#define LEFT_ARROW_INIT_POS_X	(SCREEN_LEFT + 192)
#define LEFT_ARROW_INIT_POS_Y	192
#define RIGHT_ARROW_INIT_POS_X	(SCREEN_RIGHT - 192)
#define RIGHT_ARROW_INIT_POS_Y	192
#define ROGO_INIT_POS_X			0
#define ROGO_INIT_POS_Y			192
#define ROGO_SCREEN_OUT_POS_X	(SCREEN_LEFT - 192)
#define ROGO_SCREEN_OUT_POS_Y	(SCREEN_TOP + 192)
#define STAGE_INIT_POS_X		0
#define STAGE_INIT_POS_Y		(-SCREEN_RIGHT - 100)
#define MOVE_COUNT				20

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CSelectObject : public CCharacter
{
	// ===== メンバ定数
private:
	// ===== メンバ変数
private:
	int		m_nStatus;
	// ===== メンバ関数
public:
	CSelectObject();
	virtual ~CSelectObject();

	static CSelectObject* Create(const LPCTSTR pszFName);	// 生成
	int PlayerUpdate(int nStatus);		// シーン遷移 仮2 (画面に出現)
	int ArrowUpdate(int nNo,int nStatus);
	void RogoUpdate(int nNo,int nStatus,int nStage);
	int StageUpdate(int nStatus,int nStage);
};

//========================================================================================
//	End of File
//========================================================================================