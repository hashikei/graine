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
	m_maxFieldBlock = 0;
	m_maxLayoutBlock = 0;

	m_maxClearBlock = 0;
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
	for (LPFIELDBLOCK_ARRAY_IT it = m_pFieldBlock.begin(); it != m_pFieldBlock.end(); ++it)
		(*it)->Uninit();
	for (LPCHARACTER_ARRAY_IT it = m_pLayoutBlock.begin(); it != m_pLayoutBlock.end(); ++it)
		(*it)->Uninit();

	m_pFieldBlock.clear();		// オブジェクトリスト
	m_pLayoutBlock.clear();		// オブジェクトリスト
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : いろんな更新
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStage::Update()
{
	for (LPFIELDBLOCK_ARRAY_IT it = m_pFieldBlock.begin(); it != m_pFieldBlock.end(); ++it)
		(*it)->Update();
	for (LPCHARACTER_ARRAY_IT it = m_pLayoutBlock.begin(); it != m_pLayoutBlock.end(); ++it)
		(*it)->Update();

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : いろんな更新
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStage::DrawFieldBlock()
{
#ifdef _DEBUG
	static bool drawFlg = false;
	if (GetAsyncKeyState('C') & 1)
		drawFlg = !drawFlg;
	if (drawFlg) {
		for (LPFIELDBLOCK_ARRAY_IT it = m_pFieldBlock.begin(); it != m_pFieldBlock.end(); ++it)
			(*it)->DrawAlpha();
	}
#endif
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : いろんな更新
//	Arguments   : num / ブロック番号
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStage::DrawLayoutBlock(int num)
{
	if (num < 0) {
		for (LPCHARACTER_ARRAY_IT it = m_pLayoutBlock.begin(); it != m_pLayoutBlock.end(); ++it)
			(*it)->DrawAlpha();
	} else {
		if (m_pLayoutBlock[num] != NULL) {
			m_pLayoutBlock[num]->DrawAlpha();
		}
	}
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
	CMapData::GetFieldBlockList(&m_pFieldBlock);
	CMapData::GetLayoutBlockList(&m_pLayoutBlock);

	m_vStart = CMapData::GetStartPoint();

	m_maxFieldBlock = m_pFieldBlock.size();
	m_maxLayoutBlock = m_pLayoutBlock.size();


#ifdef _DEBUG
	for (LPFIELDBLOCK_ARRAY_IT it = m_pFieldBlock.begin(); it != m_pFieldBlock.end(); ++it) {
		for (int i = 0; i < (*it)->GetElementNum(); ++i) {
			(*it)->GetElement(i)->TranslationZ(-10.0f);
		}
	}
#endif
}

//========================================================================================
//	End of File
//========================================================================================