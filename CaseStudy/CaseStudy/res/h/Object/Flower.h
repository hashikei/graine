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
#include "../Object/Character.h"
#include "../Object/Stage.h"

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
const int FLOWER_ANIME_SIZE_X = 1;	// テクスチャ
const int FLOWER_ANIME_SIZE_Y = 1;

const float FLOWER_POS_DEFAULT_X = -200;
const float FLOWER_POS_DEFAULT_Y = 100;

const float FLOWER_SIZE_X = 147;
const float FLOWER_SIZE_Y = 151;

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CFlower : public CCharacter
{
private:
	
protected:
	
public:
	CFlower();
	virtual void Init(D3DXVECTOR3 pos,float angle);			// 初期化
	virtual void Uninit();
	virtual void Update();
	virtual void Animation();								// アニメ			これクラスにした方がいいのかな
	static CFlower* Create(const LPCTSTR pszFName);			// 生成

	// ----- セッター

	// ----- ゲッター
	
};
//========================================================================================
//	End of File
//========================================================================================