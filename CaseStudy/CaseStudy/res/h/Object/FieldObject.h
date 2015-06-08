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
	BLOCK_TYPE_LAYOUT,

	MAX_BLOCK_TYPE
};

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CFieldObject : public CCharacter
{
// ===== using宣言
public:
	using CObject2D::Init;

// ===== メンバ変数
protected:
	bool	m_bCol;
	int		m_nType;
	
// ===== メンバ関数
public:
	virtual void Init();		// 初期化
	virtual void Uninit();		// 後始末
	virtual void Update();		// 更新

	// ----- ゲッター
	bool GetCol(){return m_bCol;}
	int GetType(){return m_nType;}					// 種類取得

	// ----- セッター
	void SetType(int type){m_nType = type;}		// プレイヤーの操作設定

	void EnableCol(){m_bCol = true;}
	void DisableCol(){m_bCol = false;}

	CFieldObject();
	virtual ~CFieldObject();
	static CFieldObject* Create(const LPCTSTR pszFName);	// 生成
	
private:
	bool Initialize(const LPCTSTR pszFName);	// 初期化
	void Finalize();		// 後始末
};

//――――――――――――――――――――――――――――――――――――――――――――
// ユーザ型定義
//――――――――――――――――――――――――――――――――――――――――――――
typedef std::vector<CFieldObject*>		LPFIELDOBJECT_ARRAY;	// フィールドオブジェクトリスト
typedef LPFIELDOBJECT_ARRAY::iterator	LPFIELDOBJECT_ARRAY_IT;	// フィールドオブジェクトリストのイテレータ


//========================================================================================
//	End of File
//========================================================================================