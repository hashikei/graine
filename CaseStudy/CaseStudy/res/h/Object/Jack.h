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
const int JACK_ANIME_SIZE_X = 1;	// テクスチャ
const int JACK_ANIME_SIZE_Y = 1;

const float JACK_POS_DEFAULT_X = -200;
const float JACK_POS_DEFAULT_Y = 100;

const float JACK_SIZE_X = 314;
const float JACK_SIZE_Y = 1024;

enum
{
	JACK_PHASE_INIT = 10,
	JACK_PHASE_START,
	JACK_PHASE_FLOWER,
	JACK_PHASE_WAIT,
	JACK_PHASE_UNINIT
};

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CJack : public CFlower
{
private:
	
protected:
		
public:
	CJack();
	virtual void Init(D3DXVECTOR3 pos,D3DXVECTOR3 dir);			// 初期化
	virtual void Uninit();
	virtual void Update();
	virtual void Animation();// アニメ			これクラスにした方がいいのかな
	virtual void Draw();
	static CJack* Create(const LPCTSTR pszFName);			// 生成

	// ----- セッター
	void SetPhase(int no){m_nPhase = no;}

	// ----- ゲッター
	int GetPhase(){return m_nPhase;}

};
 
//========================================================================================
//	End of File
//========================================================================================