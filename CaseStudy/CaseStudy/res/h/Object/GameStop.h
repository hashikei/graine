//========================================================================================
//		File        : Button.h
//		Program     : ボタンベース
//
//		Description : ボタンベースの定義
//
//		History     : 2015/05/29	作成開始
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
#include "../Object/Button.h"

enum
{
	GAME_STOP_PHASE_INIT = 0,
	GAME_STOP_PHASE_WAIT,
	GAME_STOP_PHASE_ENTER,
	GAME_STOP_PHASE_UNINIT,
	GAME_STOP_PHASE_END,

	GAME_STOP_PHASE_MAX
};

enum
{
	GO_GAME = 0,
	GO_RESET,
	GO_SELECT
};

using namespace std;

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CGameStop
{
private:
	enum{
		GOGAME_BUTTON = 0,
		RESET_BUTTON,
		GOSELECT_BUTTON,

		MAX_BUTTON
	};
	enum{
		TEX_WND_0	= 0,
		TEX_CLOSE,
		TEX_RESET,
		TEX_SELECT,
		TEX_TEXT,

		MAX_TEX
	};

	static const D3DXVECTOR2 W_0_DEFAULET_SIZE;
	static const D3DXVECTOR3 W_0_DEFAULET_POS;
	static const D3DXVECTOR2 B_0_DEFAULET_SIZE;
	static const D3DXVECTOR3 B_0_DEFAULET_POS;
	
	static const D3DXVECTOR2 TEXT_SIZE;
	static const D3DXVECTOR3 TEXT_POS;
	
	static const float B_0_POS_INTERVAL_Y;

	static const LPCTSTR TEX_FILENAME[];			// テクスチャのファイル名

	static const double SELECT_ANIME_TIME;
	static const D3DXVECTOR3 SELECT_BUTTON_SCALE_L;
	static const D3DXVECTOR3 SELECT_BUTTON_SCALE_S;

	int				m_nPhase;
	int				m_nCurrent;

	int				m_nGo;			// 行先

	CObject2D*		m_pWnd;
	CObject2D*		m_pText;

	CButton*		m_pButtonGoGame;
	CButton*		m_pButtonReset;
	CButton*		m_pButtonGoSelect;

	vector<CButton*> m_vecButton;

	double			m_selectAnimeTimer;

public:
	virtual void Initialize();
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	// ----- ゲッター
	int GetPhase(){return m_nPhase;}
	int GetGo(){return m_nGo;}

	// ----- セッター
	void SetPhase(int no){m_nPhase = no;}

	CGameStop();
	static CGameStop* Create();	// 生成
	void Release();

private:
	void Wait();
	void Enter();
};
//========================================================================================
//	End of File
//========================================================================================