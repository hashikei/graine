//========================================================================================
//		File        : Game.h
//		Program     : ゲーム本編クラス
//
//		Description : ゲーム本編クラスの定義
//
//		History     : 2013/12/18	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================

#pragma once

//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../System/PreInclude.h"
#
#include "../System/Graphics.h"
#include "../Scene/Scene.h"
#include "../Object/Object2D.h"
#include "../Object/GameCamera.h"

#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Object/Flower.h"
#include "../Object/Jack.h"
#include "../Object/PlayersGroup.h"
#include "../Object/GameStop.h"
#include "../Object/GameOver.h"
#include "../Object/GameClear.h"


//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
const int MAX_OBJECT = 50;		//オブジェクト数(仮)

// ----- フェーズフラグ
static enum _ePhase
{
	PHASE_FADEIN = 0,		// フェードイン
	PHASE_FADEOUT,			// 次のシーンへフェードアウト
	PHASE_MAIN,				// ゲーム本編
	PHASE_STOP,				// 一時停止
	PHASE_OVER,				// オーバ
	PHASE_CLEAR,			// クリア

	MAX_PHASE
};
//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CGame : public CScene
{
// ===== メンバ構造体
private:
	// クリッピング情報
	struct TCLIPINFO {
		D3DXVECTOR3	pos;	// 位置
		D3DXVECTOR2 size;	// サイズ
	};

// ===== ユーザ型定義
private:
	typedef std::vector<TCLIPINFO>		CLIPINFO_ARRAY;
	typedef CLIPINFO_ARRAY::iterator	CLIPINFO_ARRAY_IT;

// ===== メンバ定数
private:
	static const LPCTSTR TEX_FILENAME[];			// テクスチャのファイル名
	static const D3DXVECTOR3 INIT_TEXTURE_POS[];	// テクスチャの初期位置

	static const float FADE_POSZ;			// フェード用テクスチャのZ座標
	static const int FADEIN_TIME;			// フェードイン間隔(アルファ値:0～255)
	static const int FADEOUT_TIME;			// フェードアウト間隔(アルファ値:0～255)

	static D3DXVECTOR2	CLIP_SIZE;			// クリッピングサイズ
	static D3DXVECTOR3	CLIP_INITPOS;		// クリッピング初期位置
	static float		CLIP_SCALING_SPD;	// クリッピング拡大速度

	// ----- テクスチャリスト
	static enum _eTexList
	{
		TL_BG_DARK = 0,		// 背景テクスチャ
		TL_BG_LIGHT,		// 背景テクスチャ
		TL_PLAYER_0,		// プレイヤーテクスチャ（本体）
		TL_BLOCK_0,			// ブロックテクスチャ
		TL_FLOWER_0,
		TL_FLOWER_1,
		TL_JACK_0,
		TL_CLIP,

		MAX_TEXLIST
	};

// ===== メンバ変数
private:
	// ----- オブジェクト
	CGameCamera*	m_pCamera;		// カメラ
	CObject2D*		m_pDarkBG;		// 背景
	CObject2D*		m_pLightBG;		// 背景

	// ----- プレイヤー　----- //
	CPlayersGroup*		m_pPlayersGroup;

	std::vector<CFlower*> m_listFlower;

	CStage*				m_pStage;
	static int			m_stageID;		// 選択したステージのID

	CGameStop*			m_pGameStop;
	CGameOver*			m_pGameOver;
	CGameClear*			m_pGameClear;

	CCharacter*			m_pClipCircle;
	CLIPINFO_ARRAY		m_clipInfoList;
	std::vector<float>	m_clipEasingList;

	// ----- ゲームシステム
	DWORD		m_phase;		// フェーズフラグ
	DWORD		m_pNextScene;

// ===== メンバ関数
public:
	CGame();
	virtual ~CGame();

	void	Init(void);			// 初期化
	void	Uninit(void);		// 後始末
	void	Update(void);		// 更新
	void	Draw(void);			// 描画
	static CGame* Create();		// 生成

	void CreateFlower(D3DXVECTOR3 pos, D3DXVECTOR3 dir);
	void CreateJack(D3DXVECTOR3 pos, D3DXVECTOR3 dir);

	static void SetStageID(int id) {	// ステージID設定
		id >= 0 && id < CMapData::MAX_STAGEID ?
			m_stageID = id : m_stageID = 0;
	}

private:
	bool	Initialize();		// 初期化
	void	Finalize(void);		// 後始末

	void	Main();		// メイン
	void	DrawMain();

	void	Stop();		// 一時停止
	void	DrawStop();

	void	Over();		// オーバ
	void	DrawOver();

	void	Clear();		// クリア
	void	DrawClear();
};


//========================================================================================
//	End of File
//========================================================================================