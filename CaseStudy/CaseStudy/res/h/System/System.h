//========================================================================================
//		File        : System.h
//		Program     : ゲーム共通データ/システム
//
//		Description : ゲームの共通データ及びシステムの定義
//
//		History     : 2013/12/05	作成開始
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

//――――――――――――――――――――――――――――――――――――――――――――
// ユーザ型定義
//――――――――――――――――――――――――――――――――――――――――――――
typedef std::vector<ULONG>		ULONG_ARRAY;		// ULONG型配列
typedef ULONG_ARRAY::iterator	ULONG_ARRAY_IT;		// ULONG型配列イテレータ
typedef std::vector<LPTSTR>		LPTSTR_ARRAY;		// LPTSTR型配列
typedef LPTSTR_ARRAY::iterator	LPTSTR_ARRAY_IT;	// LPTSTR型配列イテレータ
typedef std::vector<LPCTSTR>	LPCTSTR_ARRAY;		// LPCTSTR型配列
typedef LPCTSTR_ARRAY::iterator	LPCTSTR_ARRAY_IT;	// LPCTSTR型配列イテレータ

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
// ----- ゲームウィンドウ関連
const int	SCREEN_WIDTH		= 1136;								// スクリーンの幅	// フルスクリーンにするなら：1024
const int	SCREEN_HEIGHT		= 640;								// スクリーンの高さ	//							 768
const int	SCREEN_LEFT			= (int)(-SCREEN_WIDTH * 0.5f);		// スクリーンの左端
const int	SCREEN_RIGHT		= (int)(SCREEN_WIDTH * 0.5f);		// スクリーンの右端
const int	SCREEN_TOP			= (int)(SCREEN_HEIGHT * 0.5f);		// スクリーンの上端
const int	SCREEN_BOTTOM		= (int)(-SCREEN_HEIGHT * 0.5f);		// スクリーンの下端
const float	FRAME_RATE			= 1000.0f / 60.0f;					// フレームレート
const float	FPS					= 60.0f;							// FPS

// ----- 方向フラグ
static enum _eDirect
{
	DIR_LEFT = 0,	// 左方向
	DIR_RIGHT,		// 右方向
	DIR_UP,			// 上方向
	DIR_DOWN,		// 下方向
	DIR_FRONT,		// 前方向
	DIR_BACK,		// 後方向
	DIR_NONE,		// なし

	MAX_DIRECT
};

//――――――――――――――――――――――――――――――――――――――――――――
// マクロ定義
//――――――――――――――――――――――――――――――――――――――――――――
// ----- テクスチャアニメーション用
#define TEX_ANIME_LEFT(num, width)				((1.0f / width) * ((num) % width))
#define TEX_ANIME_RIGHT(num, width)				((1.0f / width) * ((num) % width) + (1.0f / width))				
#define TEX_ANIME_TOP(num, width, height)		((1.0f / height) * ((num) / width))				
#define TEX_ANIME_BOTTOM(num, width, height)	((1.0f / height) * ((num) / width) + (1.0f / height))

// ----- 解放用
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)			if(x){ x->Release(); x=NULL; }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)			if(x){ delete x; x=NULL; }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if(x){ delete[] x; x=NULL; }
#endif

//――――――――――――――――――――――――――――――――――――――――――――
// 名前空間定義
//――――――――――――――――――――――――――――――――――――――――――――
namespace System
{
	D3DXVECTOR3 CalcScreenLeft3D(const D3DXVECTOR3* pCamera, const D3DXVECTOR3* pTarget);

	void SearchFileName(LPTSTR_ARRAY *pflist, LPCTSTR path);		// ファイル検索
}


//========================================================================================
//	End of File
//========================================================================================