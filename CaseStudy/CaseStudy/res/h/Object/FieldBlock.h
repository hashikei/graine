//========================================================================================
//		File        : FieldBlock.h
//		Program     : フィールドブロックベース
//
//		Description : フィールドブロックベースの定義
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

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CFieldBlock : public CCharacter
{
// ===== メンバ定数
protected:
	static const int INITIALIZE_ELEMENT_NUM;		// 初期ブロック要素数

// ===== メンバ変数
protected:
	bool				m_bCol;			// 当たったかどうか
	int					m_nType;		// ブロックのタイプ
	LPCHARACTER_ARRAY	m_pElement;		// ブロックの要素リスト

	int		m_nFlower;

// ===== メンバ関数
public:
	virtual void Init();		// 初期化
	virtual void Init(const D3DXVECTOR2& size, const D3DXVECTOR3& pos);	// サイズを指定して初期化
	virtual void Uninit();		// 後始末
	virtual void Update();		// 更新
	virtual void Draw();		// 描画(透過なし)
	virtual void DrawAlpha();	// 描画(透過あり)

	// ----- ゲッター
	bool GetCol(){ return m_bCol; }
	int GetType(){ return m_nType; }					// 種類取得
	int GetFloawerNum(){ return m_nFlower; }
	int GetElementNum() const { return m_pElement.size(); }		// 用素数取得
	CCharacter* GetElement(int num) const { return num >= 0 && num < (int)m_pElement.size() ? m_pElement[num] : m_pElement[0]; }		// 要素取得

	// ----- セッター
	void SetType(int type){ m_nType = type; }		// プレイヤーの操作設定
	void SetElement(CCharacter* pElem) { m_pElement.push_back(pElem); }		// 要素設定

	void AddFlower(int no){ m_nFlower += no; }
	void SubFlower(int no){ m_nFlower -= no; }

	void EnableCol(){ m_bCol = true; }
	void DisableCol(){ m_bCol = false; }

	CFieldBlock();
	virtual ~CFieldBlock();
	static CFieldBlock* Create();	// 生成

private:
	bool Initialize();		// 初期化
	void Finalize();		// 後始末
};

//――――――――――――――――――――――――――――――――――――――――――――
// ユーザ型定義
//――――――――――――――――――――――――――――――――――――――――――――
typedef std::vector<CFieldBlock*>		LPFIELDBLOCK_ARRAY;		// フィールドブロックリスト
typedef LPFIELDBLOCK_ARRAY::iterator	LPFIELDBLOCK_ARRAY_IT;	// フィールドブロックリストのイテレータ


//========================================================================================
//	End of File
//========================================================================================