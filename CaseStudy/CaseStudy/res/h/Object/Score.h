//========================================================================================
//		File        : Score.h
//		Program     : スコアクラス
//
//		Description : スコアクラスの定義
//
//		History     : 2014/11/27	作成開始
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
#include "../System/System.h"
#include "../Object/Number.h"

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CScore
{
// ===== メンバ定数
public:
	static const LONG			MAX_SCORE;			// スコアの最大値
	static const int			SCORE_DIGIT;		// 最大スコアの桁数

private:
	static const LPCTSTR		TEX_FILENAME;		// テクスチャのファイル名
	static const D3DXVECTOR2	OBJ_SIZE;			// オブジェクトサイズ
	static const int			WIDTH_FRAME;		// アニメーションフレームの横分割数
	static const int			HEIGHT_FRAME;		// アニメーションフレームの縦分割数

// ===== メンバ変数
private:
	LPNUMBER_ARRAY	m_pNumberList;		// 数字オブジェクト
	LONG			m_score;			// 描画スコア

// ===== メンバ関数
public:
	CScore();				// コンストラクタ
	virtual ~CScore();		// デストラクタ
	
	void Init(const D3DXVECTOR3& pos);	// 初期化
	void Uninit();		// 後始末
	void Update();		// 更新
	void Draw();		// 描画(アルファ無効)
	void DrawAlpha();	// 描画(アルファ有効)

	static CScore* Create();	// 生成
	void Release();				// 破棄

	void Translate(const D3DXVECTOR3& pos);	// 移動処理
	void SetScore(LONG score);				// 描画スコアを更新

private:
	bool Initialize();	// 初期化
	void Finalize();	// 後始末
};


//========================================================================================
//	End of File
//========================================================================================