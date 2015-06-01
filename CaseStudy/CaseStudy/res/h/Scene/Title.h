//========================================================================================
//		File        : Title.h
//		Program     : タイトルクラス
//
//		Description : タイトルクラスの定義
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



#include "../Object/Character.h"
#include "../System/MapData.h"

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CTitle : public CScene
{
// ===== メンバ定数
private:
	static const LPCTSTR TEX_FILENAME[];			// テクスチャのファイル名
	static const D3DXVECTOR3 INIT_CAMERA_EYE;		// カメラの初期視点
	static const D3DXVECTOR3 INIT_CAMERA_LOOK;		// カメラの初期注視点
	static const D3DXVECTOR3 INIT_TEXTURE_POS[];	// テクスチャの初期位置

	static const int FADEIN_TIME;	// フェードイン間隔(アルファ値:0～255)
	static const int FADEOUT_TIME;	// フェードアウト間隔(アルファ値:0～255)

	// ----- テクスチャリスト
	static enum _eTexList
	{
		TL_BG = 0,		// 背景テクスチャ
		TL_FADE,		// フェード用テクスチャ

		MAX_TEXLIST
	};

	// ----- フェーズフラグ
	static enum _ePhase
	{
		PHASE_FADEIN = 0,		// フェードイン
		PHASE_FADEOUT,			// 次のシーンへフェードアウト
		PHASE_MAIN,				// タイトル本編
	
		MAX_PHASE
	};
	
// ===== メンバ変数
private:
	// ----- オブジェクト
	CCamera*		m_pCamera;		// カメラ
	CTexture*		m_pBG;			// 背景
	CTexture*		m_pFilter;		// フェード用フィルター

	DWORD			m_phase;		// フェーズフラグ



	CCharacter*		m_pPlayer;
	CCharacter*		m_pEnemy;

	LPFIELDOBJECT_ARRAY	m_pFieldObj;	// フィールドオブジェクトリスト

// ===== メンバ関数
public:
	CTitle();
	virtual ~CTitle();

	void	Init();			// 初期化
	void	Uninit();		// 後始末
	void	Update();		// 更新
	void	Draw();			// 描画
	static CTitle* Create();	// 生成

private:
	bool	Initialize();	// 初期化
	void	Finalize();		// 後始末

	void	Main();			// メイン
};


//========================================================================================
//	End of File
//========================================================================================