//========================================================================================
//		File        : ChangeScene.cpp
//		Program     : シーン遷移システム
//
//		Description : シーン遷移システムの実装
//
//		History     : 2015/02/20	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include <tchar.h>
#include "../../h/System/System.h"
#include "../../h/System/ChangeScene.h"
/*
//――――――――――――――――――――――――――――――――――――――――――――
// グローバル変数宣言
//――――――――――――――――――――――――――――――――――――――――――――
static CChangeScene& g_changeScene = CChangeScene::GetInstance();		// シーン遷移システムの実体生成


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : インスタンス取得
//	Description : シーン遷移システムのインスタンスを取得する
//	Arguments   : None.
//	Returns     : シーン遷移システム
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CChangeScene& CChangeScene::GetInstance()
{
	// ----- インスタンス生成
	static CChangeScene changeScene;

	return changeScene;
}
*/

//========================================================================================
// namespace:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 左右の遷移(画面に出現)
//	Description : 画面に出現するよう、左右に滑るような遷移
//	Arguments   : pTex / テクスチャ
//				  x    / X方向移動量
//	Returns     : 完了フラグ(true:遷移完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool ChangeScene::SideSliderIn(CTexture* pTex, float x)
{
	// ----- 移動準備
	bool bLeft = true;
	if(x < 0)
		bLeft = false;

	// ----- 移動処理
	pTex->AddPosX(x);
	// 境界判定
	if(bLeft) {
		if(pTex->GetPosX() > SCREEN_WIDTH) {
			pTex->SetPosX((float)SCREEN_WIDTH);
			return true;
		}
	} else {
		if(pTex->GetPosX() + pTex->GetWidth() < 0.0f) {
			pTex->SetPosX(-pTex->GetWidth());
			return true;
		}
	}

	return false;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 左右の遷移(画面から退避)
//	Description : 画面から退避するよう、左右に滑るような遷移
//	Arguments   : pTex / テクスチャ
//				  x    / X方向移動量
//	Returns     : 完了フラグ(true:遷移完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool ChangeScene::SideSliderOut(CTexture* pTex, float x)
{
	// ----- 移動準備
	bool bLeft = true;
	if(x < 0)
		bLeft = false;

	// ----- 移動処理
	pTex->AddPosX(x);
	// 境界判定
	if(bLeft) {
		if(pTex->GetPosX() + pTex->GetWidth() > SCREEN_WIDTH) {
			pTex->SetPosX((float)SCREEN_WIDTH - pTex->GetWidth());
			return true;
		}
	} else {
		if(pTex->GetPosX() < 0.0f) {
			pTex->SetPosX(0.0f);
			return true;
		}
	}

	return false;
}


//========================================================================================
//	End of File
//========================================================================================