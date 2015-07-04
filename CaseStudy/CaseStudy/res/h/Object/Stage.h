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
#include "../Object/FieldBlock.h"


//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CStage
{
private:
	LPCTSTR			m_lpColTex;					// 当たり判定用ブロックのテクスチャ

	int					m_maxFieldBlock;		// フィールドブロック最大数
	LPFIELDBLOCK_ARRAY	m_pFieldBlock;			// フィールドブロックリスト

	int					m_maxLayoutBlock;		// レイアウトブロック最大数
	LPCHARACTER_ARRAY	m_pLayoutBlock;			// レイアウトブロックリスト

	int				m_maxClearBlock;			// クリア判定ブロック最大数
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
	int GetMaxFieldBlock(){ return m_maxFieldBlock; }
	int GetMaxLayoutBlock(){ return m_maxFieldBlock; }
	CFieldBlock* GetFieldBlock(int num) const { return num >= 0 && num < (int)m_pFieldBlock.size() ? m_pFieldBlock[num] : m_pFieldBlock[0]; };
	CCharacter* GetLayoutBlock(int num) const { return num >= 0 && num < (int)m_pLayoutBlock.size() ? m_pLayoutBlock[num] : m_pLayoutBlock[0]; };
	D3DXVECTOR3 GetStart(){ return D3DXVECTOR3(m_vStart.x, m_vStart.y, 0.0f); }

	// ----- デバッグ用
	void SetStage(int stageID);

	static CStage* Create();	// 生成
	void Release();
};
//========================================================================================
//	End of File
//========================================================================================