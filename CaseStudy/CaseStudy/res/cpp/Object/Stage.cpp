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

	m_nMaxClearBox = 0;
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
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStage::Init()
{
	SetStage();
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
	for (unsigned int i = 0;i < m_vecColBox.size(); ++i)
		m_vecColBox[i]->Uninit();

	m_vecColBox.clear();		// オブジェクトリスト
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : いろんな更新
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStage::Update()
{
	for (unsigned int i = 0;i < m_vecColBox.size(); ++i)
		m_vecColBox[i]->Update();

}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : いろんな更新
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStage::Draw()
{
	for (unsigned int i = 0;i < m_vecColBox.size(); ++i){
		m_vecColBox[i]->DrawAlpha();
	}
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
//	Name        : Stage作成
//	Description : Stage作成
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStage::SetStage()
{
	m_nMaxColBox = MAX_COLLISION_BOX;

	// オブジェクト数
	//m_vecColBox.resize(m_nMaxColBox);

	// ----- ブロック設定
	CFieldObject* f_0;
	f_0 = CFieldObject::Create(m_lpColTex);
	f_0->Init(D3DXVECTOR2(800, 64), D3DXVECTOR3(480, -256, 0));
	f_0->SetAlpha(190);

	CFieldObject* f_1;
	f_1 = CFieldObject::Create(m_lpColTex);
	f_1->Init(D3DXVECTOR2(64, 256), D3DXVECTOR3(480, -96, 0));
	f_1->SetAlpha(190);

	CFieldObject* f_2;
	f_2 = CFieldObject::Create(m_lpColTex);
	f_2->Init(D3DXVECTOR2(256, 64), D3DXVECTOR3(-540, 0, 0));
	f_2->SetAlpha(190);

	CFieldObject* f_3;
	f_3 = CFieldObject::Create(m_lpColTex);
	f_3->Init(D3DXVECTOR2(800, 64), D3DXVECTOR3(-480, -256, 0));
//	f_3->RotateZ(10);
	f_3->SetAlpha(190);


	CFieldObject* f_C_0;
	f_C_0 = CFieldObject::Create(m_lpColTex);
	f_C_0->Init(D3DXVECTOR2(128, 128), D3DXVECTOR3(1024, 0, 0));
	f_C_0->SetColor(D3DXVECTOR3(255,255,128));
	f_C_0->RotateZ(45);
	f_C_0->SetAlpha(190);
	f_C_0->SetrType(BLOCK_TYPE_CLEAR);

	CFieldObject* f_Over_0;
	f_Over_0 = CFieldObject::Create(m_lpColTex);
	f_Over_0->Init(D3DXVECTOR2(1600, 64), D3DXVECTOR3(0, -1024, 0));
	f_Over_0->SetColor(D3DXVECTOR3(255,128,128));
	f_Over_0->SetrType(BLOCK_TYPE_OVER);


	// ::::: リストに追加 ::::: //
	m_vecColBox.push_back(f_0);
	m_vecColBox.push_back(f_1);
	m_vecColBox.push_back(f_2);
	m_vecColBox.push_back(f_3);
	m_vecColBox.push_back(f_C_0);
	m_vecColBox.push_back(f_Over_0);

}
//========================================================================================
//	End of File
//========================================================================================