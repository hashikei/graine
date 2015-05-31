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



using namespace std;

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CGameOver
{
private:
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
	GO_RESET = 0,
	GO_SELECT
};
	enum{
		RESET_BUTTON = 0,
		GOSELECT_BUTTON,

		MAX_BUTTON
	};
	enum{
		TEX_WND_0	= 0,
		TEX_BUTTON_0,

		MAX_TEX
	};

	const D3DXVECTOR2 W_0_DEFAULET_SIZE		= D3DXVECTOR2(512,256);
	const D3DXVECTOR3 W_0_DEFAULET_POS		= D3DXVECTOR3(SCREEN_WIDTH / 2 - W_0_DEFAULET_SIZE.x / 2,
														SCREEN_HEIGHT / 2 - W_0_DEFAULET_SIZE.y / 2,0);

	const float B_0_POS_INTERVAL_X = 150;

	const D3DXVECTOR2 B_0_DEFAULET_SIZE		= D3DXVECTOR2(128,64);
	const D3DXVECTOR3 B_0_DEFAULET_POS		=  D3DXVECTOR3(SCREEN_WIDTH / 2 - B_0_DEFAULET_SIZE.x / 2 - B_0_POS_INTERVAL_X,
														SCREEN_HEIGHT / 2 - B_0_DEFAULET_SIZE.y / 2 + 60,0);

	const D3DXVECTOR2 B_1_DEFAULET_SIZE		= D3DXVECTOR2(128,64);
	const D3DXVECTOR3 B_1_DEFAULET_POS		=  D3DXVECTOR3(SCREEN_WIDTH / 2 - B_0_DEFAULET_SIZE.x / 2 + B_0_POS_INTERVAL_X,
														SCREEN_HEIGHT / 2 - B_0_DEFAULET_SIZE.y / 2 + 60,0);
	
	static const LPCTSTR TEX_FILENAME[];			// テクスチャのファイル名

	int				m_nPhase;
	int				m_nCurrent;

	int				m_nGo;			// 行先

	CTexture*		m_pWnd;

	CButton*		m_pButtonReset;
	CButton*		m_pButtonGoSelect;

	vector<CButton*> m_vecButton;

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

	CGameOver();
	static CGameOver* Create();	// 生成
	void Release();

private:
	void Wait();
	void Enter();
};
//========================================================================================
//	End of File
//========================================================================================