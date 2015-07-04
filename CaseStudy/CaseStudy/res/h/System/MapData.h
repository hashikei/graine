//========================================================================================
//		File        : MapData.h
//		Program     : マップデータクラス
//
//		Description : マップデータクラスの定義
//
//		History     : 2015/05/18	作成開始
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
#include "../Object/Character.h"
#include "../Object/FieldBlock.h"

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CMapData
{
	// ===== メンバ定数
private:
	static LPCTSTR	MAPDATA_LIST[];		// マップデータのファイル名リスト

public:
	// マップのステージIDリスト(ファイル名リスト)
	static enum _eStageID
	{
		ID_STAGE1 = 0,
		ID_STAGE2,
		ID_STAGE3,
		ID_STAGE4,
		ID_STAGE5,
		ID_STAGE6,
		ID_STAGE7,
		ID_STAGE8,
		ID_STAGE9,
		ID_STAGE10,

		MAX_STAGEID
	};

	// マップデータのパラメータ
	static enum _eDataParam
	{
		DP_BID = 0,			// ブロックのID
		DP_EID,				// ブロックの要素のID
		DP_TEX,				// テクスチャ名
		DP_POSX,			// X座標
		DP_POSY,			// Y座標
		DP_POSZ,			// Z座標
		DP_WIDTH,			// 幅
		DP_HEIGHT,			// 高さ
		DP_ANGLE,			// 回転角度
		DP_COLR,			// R値
		DP_COLG,			// G値
		DP_COLB,			// B値
		DP_COLA,			// A値
		DP_COLFLG,			// 当たり判定の有無
		DP_TYPE,			// 種別

		MAX_DATAPARAM	// パラメータ数
	};

	// ブロック種別
	static enum _eBlockType
	{
		BT_NORMAL = 0,	// 0:普通のフィールドブロック
		BT_CLEAR,		// 1:クリア条件フィールドブロック
		BT_OVER,		// 2:障害フィールドブロック
		BT_LAYOUT,		// 3:レイアウトブロック
		BT_LAYOUTOBJ,	// 4:レイアウトオブジェクト

		MAX_BLOCKTYPE
	};

	static const int	INIT_OBJECT_NUM;	// 初期オブジェクト数


	// ===== メンバ変数
private:
	static LPFIELDBLOCK_ARRAY	m_pFieldBlock;	// フィールドブロックリスト
	static LPCHARACTER_ARRAY	m_pLayoutBlock;	// レイアウトブロックリスト
	static D3DXVECTOR2			m_startPoint;	// 開始位置

	static float	m_leftLimit;		// 最左端
	static float	m_rightLimit;		// 最右端
	static float	m_topLimit;			// 最上端
	static float	m_bottomLimit;		// 最下端

	// ===== メンバ関数
public:
	static CMapData& GetInstance();	// インスタンス取得

	static bool LoadData(int id);	// マップデータロード
	static void DeleteData();		// マップデータ破棄

	// ----- ゲッター
	static void GetFieldBlockList(LPFIELDBLOCK_ARRAY* pObjList);		// フィールドブロックリスト取得
	static void GetLayoutBlockList(LPCHARACTER_ARRAY* pObjList);		// レイアウトブロックリスト取得
	static D3DXVECTOR2& GetStartPoint() { return m_startPoint; }		// 開始位置取得
	static int GetClearBlockNum();										// クリア条件ブロック数取得
	static float GetLeftLimit() { return m_leftLimit; }					// 最左端取得
	static float GetRightLimit() { return m_rightLimit; }				// 最右端取得
	static float GetTopLimit() { return m_topLimit; }					// 最上端取得
	static float GetBottomLimit() { return m_bottomLimit; }				// 最下端取得

private:
	CMapData();
	CMapData(const CMapData&) {}
	CMapData &operator=(const CMapData&) {}
	virtual ~CMapData();
};


//========================================================================================
//	End of File
//========================================================================================