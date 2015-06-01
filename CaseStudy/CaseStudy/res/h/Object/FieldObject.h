//========================================================================================
//		File        : FieldObject.h
//		Program     : フィールオブジェクトベース
//
//		Description : フィールドオブジェクトベースの定義
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
#include "../System/System.h"
#include "../Object/Character.h"


enum
{
	BLOCK_TYPE_0 = 0,
	BLOCK_TYPE_CLEAR,
	BLOCK_TYPE_OVER,

	MAX_BLOCK_TYPE
};

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CFieldObject : public CCharacter
{
private:
	bool	m_bCol;
	int		m_nType;
	int		m_nFlower;
public:
	// ----- ゲッター
	bool GetCol(){return m_bCol;}
	int GetType(){return m_nType;}					// 種類取得
	int GetFloawerNum(){return m_nFlower;}

	// ----- セッター
	void SetrType(int type){m_nType = type;}	// プレイヤーの操作設定

	void AddFlower(int no){m_nFlower += no;}
	void SubFlower(int no){m_nFlower -= no;}

	void EnableCol(){m_bCol = true;}
	void DisableCol(){m_bCol = false;}

	CFieldObject();
	static CFieldObject* Create(const LPCTSTR pszFName);	// 生成
	virtual void Release();								// 破棄
};
//========================================================================================
//	End of File
//========================================================================================