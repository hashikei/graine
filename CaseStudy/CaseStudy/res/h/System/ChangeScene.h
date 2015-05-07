//========================================================================================
//		File        : ChangeScene.h
//		Program     : シーン遷移システム
//
//		Description : シーン遷移システムの定義
//
//		History     : 2015/02/20	作成開始
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
#include <vector>
#include "../Object/Texture.h"
/*
//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
// ----- フェード関連
const LPCTSTR SELECT_FADE_FILENAME = _T("res/img/RainbowArrow.png");	// セレクト画面からの画面遷移用テクスチャファイル名

//――――――――――――――――――――――――――――――――――――――――――――
// 構造体定義
//――――――――――――――――――――――――――――――――――――――――――――
typedef struct _tSelectFade		// セレクト画面からの画面遷移用テクスチャリスト
{
	CTexture*	pRed;		// 赤
	CTexture*	pOrange;	// 橙
	CTexture*	pYellow;	// 黄
	CTexture*	pYelGreen;	// 黄緑
	CTexture*	pGreen;		// 緑
	CTexture*	pSyan;		// 水色
	CTexture*	pBlue;		// 青
	CTexture*	pPurple;	// 紫
} SELECTFADE;

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CChangeScene
{
// ===== メンバ定数
private:
	
// ===== メンバ変数
private:

// ===== メンバ関数
public:
	static CChangeScene& GetInstance();		// インスタンス取得

private:
	CChangeScene();
	CChangeScene(const CChangeScene&) {}
	CChangeScene &operator=(const CChangeScene&) {}
	virtual ~CChangeScene();
};
*/
//――――――――――――――――――――――――――――――――――――――――――――
// 名前空間定義
//――――――――――――――――――――――――――――――――――――――――――――
namespace ChangeScene
{
	bool SideSliderIn(CTexture* pTex, float x);		// 左右に滑るような遷移(画面に出現)
	bool SideSliderOut(CTexture* pTex, float x);	// 左右に滑るような遷移(画面から退避)
}


//========================================================================================
//	End of File
//========================================================================================