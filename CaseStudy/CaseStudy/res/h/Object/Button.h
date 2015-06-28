//========================================================================================
//		File        : Button.h
//		Program     : ボタンベース
//
//		Description : ボタンベースの定義
//
//		History     : 2015/05/29	作成開始
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
#include "../Object/Object2D.h"


enum
{
	B_PHASE_WAIT = 0,
	B_PHASE_CHOICE,
	B_PHASE_ENTER,
};

using namespace std;

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CButton : public CObject2D
{
private:
	
	int				m_nPhase;
	string			m_strName;
	
public:
	virtual void Init(D3DXVECTOR2 pos,D3DXVECTOR3 size);
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	// ----- ゲッター
	int GetPhase(){return m_nPhase;}
	string GetName(){return m_strName;}
	

	// ----- セッター
	void SetPhase(int no){m_nPhase = no;}
	void SetName(string str){m_strName = str;}

	CButton();
	static CButton* Create(const LPCTSTR pszFName);	// 生成
};
//========================================================================================
//	End of File
//========================================================================================