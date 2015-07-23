//========================================================================================
//		File        : Player.h
//		Program     : プレイヤー
//
//		Description : プレイヤー個別の動き
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
#include "../System/Input.h"
#include "../System/System.h"
#include "../Object/Character.h"
#include "../Object/Stage.h"

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CUI : public CCharacter
{
private:
	CCharacter *m_MaxGrane;
	CCharacter *m_ClearBlock;

	int			m_nMaxGrane;
	int			m_nCurrentGrane;

	int			m_nMaxClearBlock;
	int			m_nCurrentClearBlock;

	static const LPCTSTR COL_TEX_FILENAME;

	bool Initialize(const LPCTSTR pszFName);	// 初期化
protected:
		
public:
	virtual void Init(D3DXVECTOR3 pos);			// 初期化
	virtual void Uninit();
	virtual void Update();
	virtual void Animation();// アニメ			これクラスにした方がいいのかな
	virtual void Draw();
	static CUI* Create(const LPCTSTR pszFName);			// 生成

	// ----- セッター
	
	// ----- ゲッター
	

};
 
//========================================================================================
//	End of File
//========================================================================================