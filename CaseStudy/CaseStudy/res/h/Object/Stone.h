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
#include "../Object/Flower.h"
#include "../Object/Stage.h"

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
const int STONE_ANIME_SIZE_X = 1;	// テクスチャ
const int STONE_ANIME_SIZE_Y = 1;

const float STONE_POS_DEFAULT_X = -200;
const float STONE_POS_DEFAULT_Y = 100;

const float STONE_SIZE_X = 512;
const float STONE_SIZE_Y = 512;

enum
{
	STONE_PHASE_INIT = 10,
	STONE_PHASE_START,
	STONE_PHASE_FLOWER,
	STONE_PHASE_WAIT,
	STONE_PHASE_UNINIT
};

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CStone : public CFlower
{
private:
	CCharacter *m_col;
	LPCTSTR		m_lpCol;		// Playerのテクスチャ

	static const LPCTSTR COL_TEX_FILENAME;

	bool Initialize(const LPCTSTR pszFName);	// 初期化
protected:
		
public:
	CStone();
	virtual void Init(D3DXVECTOR3 pos,D3DXVECTOR3 dir);			// 初期化
	virtual void Uninit();
	virtual void Update();
	virtual void Animation();// アニメ			これクラスにした方がいいのかな
	virtual void Draw();
	static CStone* Create(const LPCTSTR pszFName);			// 生成

	// ----- セッター
	void SetPhase(int no){m_nPhase = no;}

	// ----- ゲッター
	int GetPhase(){return m_nPhase;}
	CCharacter* GetCol(){return m_col;}

};
 
//========================================================================================
//	End of File
//========================================================================================