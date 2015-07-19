//========================================================================================
//		File        : Select.h
//		Program     : 楽曲選択クラス
//
//		Description : 楽曲選択クラスの定義
//
//		History     : 2014/07/30	作成開始
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
#include "../Object/Texture.h"
#include "../Object/Camera.h"
#include "../Object/SelectObject.h"
#include "../Scene/GameMain.h"

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CSelect : public CScene
{
	// ===== メンバ定数
private:

	static const LPCTSTR TEX_FILENAME[];			// テクスチャのファイル名
	static const D3DXVECTOR3 INIT_CAMERA_EYE;		// カメラの初期視点
	static const D3DXVECTOR3 INIT_CAMERA_LOOK;		// カメラの初期注視点
	static const D3DXVECTOR3 INIT_TEXTURE_POS[];	// テクスチャの初期位置

	static const float FADE_POSZ;		// フェード用テクスチャのZ座標
	static const int FADEIN_TIME;		// フェードイン間隔(アルファ値:0～255)
	static const int FADEOUT_TIME;		// フェードアウト間隔(アルファ値:0～255)

	// ----- テクスチャリスト
	static enum _eTexList
	{
		TL_BG_1 = 0,		// 背景テクスチャ
		TL_BG_2,		// 背景テクスチャ
		TL_RING_TRUE,
		TL_RING_FALSE,
		TL_TACTILE,
		TL_PLAYER,
		TL_ARROW,
		TL_ROGO_1,
		TL_ROGO_2,
		TL_ROGO_3,
		TL_ROGO_4,
		TL_ROGO_5,
		TL_STAGE_1,
		TL_STAGE_2,
		TL_STAGE_3,

		MAX_TEXLIST
	};

	// ----- オブジェクトリスト
	static enum _eObjectList
	{
		OL_BG_1 = 0,
		OL_BG_2,
		OL_RING_TRUE,
		OL_RING_FALSE,
		OL_STAGE_1,
		OL_STAGE_2,
		OL_STAGE_3,
		OL_ARROW_LEFT,
		OL_ARROW_RIGHT,
		OL_ROGO_1,
		OL_ROGO_2,
		OL_ROGO_3,
		OL_ROGO_4,
		OL_ROGO_5,
		OL_TACTILE,
		OL_PLAYER,

		MAX_OBJECTLIST
	};

	// ----- フェーズフラグ
	static enum _ePhase
	{
		PHASE_FADEIN = 0,		// フェードイン
		PHASE_FADEOUT,			// 次のシーンへフェードアウト
		PHASE_MAIN,				// 楽曲選択本編

		MAX_PHASE
	};

	// ===== メンバ変数
private:
	// ----- オブジェクト
	CCamera*		m_pCamera;		// カメラ
	CObject2D*		m_pBG;			// 背景

	DWORD			m_phase;			// フェーズフラグ

	CSelectObject*	m_pSelectPlayer[MAX_OBJECTLIST];
	int				m_nStatus;
	int				m_nStage;
	bool			m_bAnime;
	bool			m_bBG1Anime;
	bool			m_bBG2Anime;

	int*			m_pClearFlg;
	int				m_nClearFlg[5];
	int				m_nClear;
	bool			m_bEffect;

	float			m_fRotationZ;
	int				m_nCount;

	// ===== メンバ関数
public:
	CSelect();
	virtual ~CSelect();

	void	Init();			// 初期化
	void	Uninit();		// 後始末
	void	Update();		// 更新
	void	Draw();			// 描画
	static CSelect* Create();	// 生成

private:
	bool	Initialize();	// 初期化
	void	Finalize();		// 後始末

	void	Main();			// メイン
};


//========================================================================================
//	End of File
//========================================================================================