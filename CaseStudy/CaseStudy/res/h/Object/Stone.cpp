//========================================================================================
//		File        : Flower.h
//		Program     : 花
//
//		Description : 
//
//		History     : 2015/05/26	作成開始
//						   
//
//																Author : きんたまズ
//========================================================================================


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/Input.h"
#include "../../h/Object/Stone.h"

//========================================================================================
// 定数定義
//========================================================================================
const LPCTSTR CStone::COL_TEX_FILENAME = {
	_T("res/img/GameScene/Object/block.png"),
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CStone::CStone()
{
	m_nPhase = STONE_PHASE_INIT;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : 初期化
//	Arguments   : 
//	Returns     : 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStone::Init(D3DXVECTOR3 pos,D3DXVECTOR3 dir)
{
	
	// キャラクターの初期化
	CCharacter::Init();

	Resize(D3DXVECTOR2(STONE_SIZE_X,STONE_SIZE_Y));
	pos += dir * (STONE_SIZE_Y / 3);
	Translate(pos);

	m_angle = AngleOf2Vector(dir,D3DXVECTOR3(0,1,0));

	m_col->Init(D3DXVECTOR2(STONE_SIZE_X / 2, STONE_SIZE_Y / 2),
			pos);
	m_col->UVDivision(0, 1, 1);

	if(dir.x > 0){
		RotateZ(-(float)m_angle);
		m_col->RotateZ(-(float)m_angle);
	}
	if(dir.x < 0){
		RotateZ((float)m_angle);
		m_col->RotateZ((float)m_angle);
	}

	// アニメーション初期化
	StartAnimation();

	UVDivision(0, STONE_ANIME_SIZE_X, STONE_ANIME_SIZE_Y);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : 初期化
//	Arguments   : 
//	Returns     : 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStone::Uninit()
{
	CCharacter::Uninit();

	m_col->Uninit();
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成
//	Description : オブジェクトを生成する
//	Arguments   : pszFName / 読み込みファイル名
//	Returns     : オブジェクトデータ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CStone* CStone::Create(const LPCTSTR pszFName)
{
	// ----- 変数宣言
	CStone* pObj;
	// ----- 初期化処理
	pObj = new CStone();
	if (pObj)
	{
		if (!pObj->Initialize(pszFName))
		{
			SAFE_DELETE(pObj);
		}
	}

	return pObj;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : オブジェクトを初期化する
//	Arguments   : pszFName / ファイル名
//	Returns     : 成否
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CStone::Initialize(const LPCTSTR pszFName)
{
	// ----- テクスチャ読み込み
	if (!CCharacter::Initialize(pszFName))
		return false;

	m_col = CCharacter::Create(COL_TEX_FILENAME);
	if (!m_col)
		return false;
	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : いろんな更新
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStone::Update()
{	

	switch(m_nPhase)
	{
	case STONE_PHASE_INIT:
		m_nPhase++;
		break;
	case STONE_PHASE_START:
		m_nPhase++;
		break;
	case STONE_PHASE_FLOWER:
		break;
	case STONE_PHASE_WAIT:
		break;
	case STONE_PHASE_UNINIT:
		break;
	}

	Translate(m_pos);

	Animation();

}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : アニメ
//	Description : アニメーしょん
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStone::Animation()
{
	// 状態によってアニメーション変化
	FrameAnimation(1, 1, 1, 1, 0.5f);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : アニメ
//	Description : アニメーしょん
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CStone::Draw()
{
	DrawAlpha();
	//m_col->DrawAlpha();
}

//========================================================================================
//	End of File
//========================================================================================