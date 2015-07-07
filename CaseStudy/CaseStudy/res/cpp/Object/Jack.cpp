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
// public:
//========================================================================================

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
	pos += dir * (JACK_SIZE_Y / 2);
	Translate(pos);

	//m_angle = AngleOf2Vector(pos,D3DXVECTOR3(0,1,0));

	//RotateZ(m_angle);


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
	FrameAnimation(0, 0, 1, 1, 0.5f);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : アニメ
//	Description : アニメーしょん
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CJack::Draw()
{
	CCharacter::Draw();

}

//========================================================================================
//	End of File
//========================================================================================