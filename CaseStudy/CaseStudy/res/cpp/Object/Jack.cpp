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
#include "../../h/Object/Jack.h"

//========================================================================================
// 定数定義
//========================================================================================
const LPCTSTR CJack::COL_TEX_FILENAME = {
	_T("res/img/GameScene/Object/block.png"),
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CJack::CJack()
{
	m_nPhase = JACK_PHASE_INIT;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : 初期化
//	Arguments   : 
//	Returns     : 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CJack::Init(D3DXVECTOR3 pos,D3DXVECTOR3 dir)
{
	
	// キャラクターの初期化
	CCharacter::Init();

	Resize(D3DXVECTOR2(JACK_SIZE_X,JACK_SIZE_Y));
	pos += dir * (JACK_SIZE_Y / 3);
	Translate(pos);

	m_angle = AngleOf2Vector(dir,D3DXVECTOR3(0,1,0));

	m_col->Init(D3DXVECTOR2(JACK_SIZE_X / 3, JACK_SIZE_Y / 1.5),pos);
	m_col->UVDivision(0, 1, 1);
	m_col->SetType(1);

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

	UVDivision(0, JACK_ANIME_SIZE_X, JACK_ANIME_SIZE_Y);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : 初期化
//	Arguments   : 
//	Returns     : 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CJack::Uninit()
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
CJack* CJack::Create(const LPCTSTR pszFName)
{
	// ----- 変数宣言
	CJack* pObj;
	// ----- 初期化処理
	pObj = new CJack();
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
bool CJack::Initialize(const LPCTSTR pszFName)
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
void CJack::Update()
{	

	switch(m_nPhase)
	{
	case JACK_PHASE_INIT:
		m_nPhase++;
		break;
	case JACK_PHASE_START:
		m_nPhase++;
		break;
	case JACK_PHASE_FLOWER:
		break;
	case JACK_PHASE_WAIT:
		break;
	case JACK_PHASE_UNINIT:
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
void CJack::Animation()
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
void CJack::Draw()
{

	DrawAlpha();
	m_col->DrawAlpha();
}

//========================================================================================
//	End of File
//========================================================================================