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
#include "../Object/FieldObject.h"

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――

// デバッグ用定数
const int MAX_COLLISION_BOX = 5;		// ブロックの数

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CStage
{
private:
	LPCTSTR			m_lpColTex;					// 当たり判定用ブロックのテクスチャ

	int				m_nMaxColBox;				// 当たり判定用ブロック最大数
	std::vector<CFieldObject*>	m_vecColBox;	// フィールドオブジェクトリスト

	int				m_nMaxClearBox;				// 当たり判定用ブロック最大数


public:	
	CStage();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	// ----- セッター
	void SetColBoxTexture(const LPCTSTR tex){m_lpColTex = tex;}


	// ----- ゲッター
	int GetColBoxMax(){return m_nMaxColBox;}
	CFieldObject* GetColBox(int no);
	
	// ----- デバッグ用
	void SetStage();
	
	static CStage* Create();	// 生成
	void Release();
};
//========================================================================================
//	End of File
//========================================================================================