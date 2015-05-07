//========================================================================================
//		File        : Number.h
//		Program     : 数字クラス
//
//		Description : 数字クラスの定義
//
//		History     : 2014/11/27	作成開始
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
#include <tchar.h>
#include "../System/System.h"
#include "../Object/Object2D.h"

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CNumber : public CObject2D
{
// ===== メンバ定数
private:

// ===== メンバ変数
private:

// ===== メンバ関数
public:
	CNumber();				// コンストラクタ
	virtual ~CNumber();		// デストラクタ
	
	void Init();		// デフォルト値で初期化
	void Init(const D3DXVECTOR2& size);	// 初期化
	void Init(const float width, const float height);	// 初期化
	void Uninit();		// 後始末
	void Update();		// 更新

	static CNumber* Create(const LPCTSTR pszFName);		// 生成
	void Release();										// 破棄

private:
	bool Initialize(const LPCTSTR pszFName);	// 初期化
	void Finalize();							// 後始末
};

//――――――――――――――――――――――――――――――――――――――――――――
// ユーザ型定義
//――――――――――――――――――――――――――――――――――――――――――――
typedef std::vector<CNumber*>		LPNUMBER_ARRAY;		// 数字クラスのリスト
typedef LPNUMBER_ARRAY::iterator	LPNUMBER_ARRAY_IT;	// 数字クラスリストのイテレータ


//========================================================================================
//	End of File
//========================================================================================