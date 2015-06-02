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
#include "../Object/FieldObject.h"

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

		MAX_STAGEID
	};

	// マップデータのパラメータ
	static enum _eDataParam
	{
		DP_ID = 0,			// ID
		DP_TEX,				// テクスチャ名
		DP_POSX,			// X座標
		DP_POSY,			// Y座標
		DP_POSZ,			// Z座標
		DP_WIDTH,			// 幅
		DP_HEIGHT,			// 高さ
		DP_ANGLE,			// 回転角度
		DP_COLFLG,			// 当たり判定の有無
		DP_TYPE,			// 種別

		MAX_DATAPARAM	// パラメータ数
	};

	static const int	INIT_OBJECT_NUM;	// 初期オブジェクト数

// ===== メンバ変数
private:
	static LPFIELDOBJECT_ARRAY	m_pFieldObj;	// フィールドオブジェクトリスト

// ===== メンバ関数
public:
	static CMapData& GetInstance();	// インスタンス取得

	static bool LoadData(int id);	// マップデータロード

	static void GetFieldObjList(LPFIELDOBJECT_ARRAY* pObjList);		// フィールドオブジェクトリスト取得
	
private:
	CMapData();
	CMapData(const CMapData&) {}
	CMapData &operator=(const CMapData&) {}
	virtual ~CMapData();
};


//========================================================================================
//	End of File
//========================================================================================