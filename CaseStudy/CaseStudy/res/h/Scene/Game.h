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
#include "../Object/Texture.h"
#include "../Object/Camera.h"

#include "../Object/FieldObject.h"
#include "../Object/Player.h"


//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
const int MAX_OBJECT = 50;		//オブジェクト数(仮)

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CGame : public CScene
{
// ===== メンバ定数
private:
	static const LPCTSTR TEX_FILENAME[];			// テクスチャのファイル名
	static const D3DXVECTOR3 INIT_CAMERA_EYE;		// カメラの初期視点
	static const D3DXVECTOR3 INIT_CAMERA_LOOK;		// カメラの初期注視点
	static const D3DXVECTOR3 INIT_TEXTURE_POS[];	// テクスチャの初期位置
	
	static const int FADEIN_TIME;			// フェードイン間隔(アルファ値:0～255)
	static const int FADEOUT_TIME;			// フェードアウト間隔(アルファ値:0～255)

	// ----- テクスチャリスト
	static enum _eTexList
	{
		TL_BG = 0,		// 背景テクスチャ
		TL_PLAYER_0,	// プレイヤーテクスチャ（本体）
		TL_BLOCK_0,		// ブロックテクスチャ
		TL_FADE,		// フェード用テクスチャ

		MAX_TEXLIST
	};

	// ----- フェーズフラグ
	static enum _ePhase
	{
		PHASE_FADEIN = 0,		// フェードイン
		PHASE_FADEOUT,			// 次のシーンへフェードアウト
		PHASE_MAIN,				// ゲーム本編
	
		MAX_PHASE
	};

// ===== メンバ変数
private:
	// ----- オブジェクト
	CCamera*	m_pCamera;		// カメラ
	CTexture*	m_pBG;			// 背景
	CTexture*	m_pFilter;		// フェード用フィルター

	/*　いくみくんが追加したよ　*/
	LPOBJECT2D_ARRAY	vecFieldObj;	// フィールドオブジェクトリスト
	// 以下↑にぶち込む奴共(適当)
	CFieldObject* m_pBlock;		// ブロック
	
	// ----- プレイヤー　----- //
	CPlayer*	m_pPlayer;

	// ----- ゲームシステム
	DWORD		m_phase;		// フェーズフラグ

// ===== メンバ関数
public:
	CGame();
	virtual ~CGame();

	void	Init(void);			// 初期化
	void	Uninit(void);		// 後始末
	void	Update(void);		// 更新
	void	Draw(void);			// 描画
	static CGame* Create();		// 生成

private:
	bool	Initialize();		// 初期化
	void	Finalize(void);		// 後始末

	void	Main();		// メイン
};


//========================================================================================
//	End of File
//========================================================================================