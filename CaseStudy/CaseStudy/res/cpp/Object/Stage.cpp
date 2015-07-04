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
#include "../../h/Object/Stage.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CStage::CStage()
{
	m_nMaxColBox = 0;
	m_nMaxLayoutBox = 0;

	m_nMaxClearBox = 0;
	m_vStart = D3DXVECTOR2(0, 0);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成
//	Description : オブジェクトを生成する
//	Arguments   : pszFName / 読み込みファイル名
//	Returns     : オブジェクトデータ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CStage* CStage::Create()
{
	// ----- 変数宣言
	CStage* pObj;

	// ----- 初期化処理
	pObj = new CStage();


	return pObj;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 解放
//	Description : オブジェクトを解放する
//	Arguments   : 
//	Returns     : 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStage::Release()
{
	delete this;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : いろんな初期化
//	Arguments   : stageID / ステージのID
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStage::Init(int stageID)
{
	SetStage(stageID);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : いろんな後始末
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStage::Uninit()
{
	// リスト内全部後始末
	for (unsigned int i = 0; i < m_vecColBox.size(); ++i)
		m_vecColBox[i]->Uninit();
	for (LPCHARACTER_ARRAY_IT it = m_vecLayoutBox.begin(); it != m_vecLayoutBox.end(); ++it)
		(*it)->Uninit();

	m_vecColBox.clear();		// オブジェクトリスト
	m_vecLayoutBox.clear();		// オブジェクトリスト
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : いろんな更新
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStage::Update()
{
	for (unsigned int i = 0; i < m_vecColBox.size(); ++i)
		m_vecColBox[i]->Update();
	for (LPCHARACTER_ARRAY_IT it = m_vecLayoutBox.begin(); it != m_vecLayoutBox.end(); ++it)
		(*it)->Update();

}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : いろんな更新
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStage::Draw()
{
	for (unsigned int i = 0; i < m_vecColBox.size(); ++i){
		m_vecColBox[i]->DrawAlpha();
	}
	for (LPCHARACTER_ARRAY_IT it = m_vecLayoutBox.begin(); it != m_vecLayoutBox.end(); ++it)
		(*it)->DrawAlpha();
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : ColBox情報取得
//	Description : ColBox情報取得
//	Arguments   : ColBoxの番号
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CFieldObject* CStage::GetColBox(int no)
{
	return m_vecColBox[no];

}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : LayoutBox情報取得
//	Description : LayoutBox情報取得
//	Arguments   : LayoutBoxの番号
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CCharacter* CStage::GetLayoutBox(int no)
{
	return m_vecLayoutBox[no];
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : Stage作成
//	Description : Stage作成
//	Arguments   : stageID / ステージのID
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStage::SetStage(int stageID)
{
	CMapData::LoadData(stageID);	// マップデータ読み込み
	CMapData::GetFieldObjList(&m_vecColBox);
	CMapData::GetLayoutObjList(&m_vecLayoutBox);

	m_vStart = CMapData::GetStartPoint();

	m_nMaxColBox = m_vecColBox.size();
	m_nMaxLayoutBox = m_vecLayoutBox.size();


	// ----- ステージサイズ調整(テスト用)
	m_vecLayoutBox[0]->TranslateZ(-10.0f);
}

//========================================================================================
//	End of File
//========================================================================================