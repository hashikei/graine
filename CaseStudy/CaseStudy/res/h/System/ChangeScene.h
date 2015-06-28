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
#include "../Object/Object2D.h"

////――――――――――――――――――――――――――――――――――――――――――――
//// 定数定義
////――――――――――――――――――――――――――――――――――――――――――――
//// ----- フェード関連
//const LPCTSTR SELECT_FADE_FILENAME = _T("res/img/RainbowArrow.png");	// セレクト画面からの画面遷移用テクスチャファイル名
//
////――――――――――――――――――――――――――――――――――――――――――――
//// 構造体定義
////――――――――――――――――――――――――――――――――――――――――――――
//typedef struct _tSelectFade		// セレクト画面からの画面遷移用テクスチャリスト
//{
//	CTexture*	pRed;		// 赤
//	CTexture*	pOrange;	// 橙
//	CTexture*	pYellow;	// 黄
//	CTexture*	pYelGreen;	// 黄緑
//	CTexture*	pGreen;		// 緑
//	CTexture*	pSyan;		// 水色
//	CTexture*	pBlue;		// 青
//	CTexture*	pPurple;	// 紫
//} SELECTFADE;

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CChangeScene
{
// ===== メンバ定数
private:
	static const LPCTSTR TEX_FILENAME[];	// テクスチャのファイル名
	
	// ----- テクスチャリスト
	static enum _eTexList
	{
		TL_NORMALFADE = 0,		// 単色フェード用テクスチャ

		MAX_TEXLIST
	};

// ===== メンバ変数
private:
	static CObject2D*	m_pNormalFade;		// 単色フェード用テクスチャ

// ===== メンバ関数
public:
	// ----- ゲッター
	static CChangeScene& GetInstance();		// インスタンス取得

	// ----- セッター
	static void SetNormalFadePos(const D3DXVECTOR3& pos);		// 単色フェード用オブジェクトの位置設定
	static void SetNormalFadeColor(const D3DXVECTOR3& color);	// 単色フェード用オブジェクトの頂点カラー設定(0～255)
	static void SetNormalFadeAlpha(int alpha);					// 単色フェード用オブジェクトの透過度設定(0～255)

	// ----- 演出関連
	static bool NormalFadeIn(float z, int alpha);	// 単色で透過するフェードイン
	static bool NormalFadeOut(float z, int alpha);	// 単色で透過するフェードアウト

	// ----- 描画
	static void DrawNormalFade();		// 単色フェード用オブジェクトを描画

private:
	CChangeScene();
	CChangeScene(const CChangeScene&) {}
	CChangeScene &operator=(const CChangeScene&) {}
	virtual ~CChangeScene();

	static int CreateNormalFade();	// 単色フェード用テクスチャ生成(戻り値[0:失敗, 1:成功, 2:生成済み])
};

//――――――――――――――――――――――――――――――――――――――――――――
// 名前空間定義
//――――――――――――――――――――――――――――――――――――――――――――
namespace ChangeScene
{
	bool SideSliderIn(CObject2D* pTex, float x);		// 左右に滑るような遷移(画面に出現)
	bool SideSliderOut(CObject2D* pTex, float x);	// 左右に滑るような遷移(画面から退避)
}


//========================================================================================
//	End of File
//========================================================================================