//========================================================================================
//		File        : Stage
//		Program     : 
//
//		Description : ステージ上のオブジェクト管理（Player以外）
//						
//						
//
//		History     : 2015/05/18	作成開始
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
#include "../System/MapData.h"
#include "../Object/Character.h"
#include "../Object/FieldObject.h"


//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CStage
{
private:
	LPCTSTR			m_lpColTex;					// 当たり判定用ブロックのテクスチャ

	int					m_nMaxColBox;			// 当たり判定用ブロック最大数
	LPFIELDOBJECT_ARRAY	m_vecColBox;			// フィールドオブジェクトリスト

	int					m_nMaxLayoutBox;		// レイアウト用ブロック最大数
	LPCHARACTER_ARRAY	m_vecLayoutBox;			// レイアウトオブジェクトリスト

	int				m_nMaxClearBox;				// 当たり判定用ブロック最大数
	D3DXVECTOR2		m_vStart;

public:
	CStage();
	void Init(int stageID);
	void Uninit();
	void Update();
	void Draw();

	// ----- セッター
	void SetColBoxTexture(const LPCTSTR tex){ m_lpColTex = tex; }


	// ----- ゲッター
	int GetColBoxMax(){ return m_nMaxColBox; }
	int GetLayoutBoxMax(){ return m_nMaxLayoutBox; }
	CFieldObject* GetColBox(int no);
	CCharacter* GetLayoutBox(int no);
	D3DXVECTOR3 GetStart(){ return D3DXVECTOR3(m_vStart.x, m_vStart.y, 0.0f); }

	// ----- デバッグ用
	void SetStage(int stageID);

	static CStage* Create();	// 生成
	void Release();
};
//========================================================================================
//	End of File
//========================================================================================