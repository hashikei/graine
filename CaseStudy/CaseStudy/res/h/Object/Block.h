//========================================================================================
//		File        : Block.h
//		Program     : ブロックベース
//
//		Description : ブロックベースの定義
//
//		History     : 2015/06/30	作成開始
//						   
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
class CBlock : public CCharacter
{
// ===== メンバ定数
protected:
	static const int INITIALIZE_ELEMENT_NUM;		// 初期ブロック要素数

// ===== メンバ変数
protected:
	int					m_nType;		// ブロックのタイプ
	LPCHARACTER_ARRAY	m_pElement;		// ブロックの要素リスト

	int		m_nFlower;
	
// ===== メンバ関数
public:
	virtual void Init();		// 初期化
	virtual void Init(const D3DXVECTOR2& size, const D3DXVECTOR3& pos);	// サイズを指定して初期化
	virtual void Uninit();		// 後始末
	virtual void Update();		// 更新

	// ----- ゲッター
	int GetType(){return m_nType;}					// 種類取得
	int GetFloawerNum(){return m_nFlower;}

	// ----- セッター
	void SetType(int type){m_nType = type;}		// プレイヤーの操作設定
	void SetElement(CCharacter* pElem) { m_pElement.push_back(pElem); }		// 要素設定

	void AddFlower(int no){m_nFlower += no;}
	void SubFlower(int no){m_nFlower -= no;}

	CBlock();
	virtual ~CBlock();
	static CBlock* Create(const LPCTSTR pszFName);	// 生成
	
protected:
	bool Initialize(const LPCTSTR pszFName);	// 初期化
	void Finalize();		// 後始末
};

//――――――――――――――――――――――――――――――――――――――――――――
// ユーザ型定義
//――――――――――――――――――――――――――――――――――――――――――――
typedef std::vector<CBlock*>	LPBLOCK_ARRAY;		// ブロックリスト
typedef LPBLOCK_ARRAY::iterator	LPBLOCK_ARRAY_IT;	// ブロックリストのイテレータ


//========================================================================================
//	End of File
//========================================================================================