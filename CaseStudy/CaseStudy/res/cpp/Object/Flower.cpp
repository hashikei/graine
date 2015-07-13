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
#include "../../h/Object/Flower.h"

//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CFlower::CFlower()
{
	m_nPhase = FLOWER_PHASE_INIT;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : 初期化
//	Arguments   : 
//	Returns     : 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CFlower::Init(D3DXVECTOR3 pos,D3DXVECTOR3 dir,const LPCTSTR pszFName)
{
	m_kuki = CObject2D::Create(pszFName);
	//m_kuki->Init(D3DXVECTOR3(1,1,0));

	// キャラクターの初期化
	CCharacter::Init();
	Resize(D3DXVECTOR2(FLOWER_SIZE_X,FLOWER_SIZE_Y));
	pos += dir * (FLOWER_SIZE_X / 2);
	Translate(pos);

	m_angle = AngleOf2Vector(pos,D3DXVECTOR3(0,1,0));

	if(m_angle > 0)
		RotateZ((float)m_angle);

	m_lastTime		= CTimer::GetTime();

	if(rand() % 2 == 0)
		m_rotSpd = 3;
	else if(rand() % 2 == 1)
		m_rotSpd = -3;

	// アニメーション初期化
	StartAnimation();

	UVDivision(0, FLOWER_ANIME_SIZE_X, FLOWER_ANIME_SIZE_Y);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : 初期化
//	Arguments   : 
//	Returns     : 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CFlower::Uninit()
{
	CCharacter::Uninit();

}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成
//	Description : オブジェクトを生成する
//	Arguments   : pszFName / 読み込みファイル名
//	Returns     : オブジェクトデータ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CFlower* CFlower::Create(const LPCTSTR pszFName)
{
	// ----- 変数宣言
	CFlower* pObj;
	// ----- 初期化処理
	pObj = new CFlower();
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
void CFlower::Update()
{	

	switch(m_nPhase)
	{
	case FLOWER_PHASE_INIT:
		m_lastTime = CTimer::GetTime();
		m_nPhase++;
		break;
	case FLOWER_PHASE_START:
		RotationZ((float)(m_nowTime - m_lastTime) * m_rotSpd);
		m_nowTime = CTimer::GetTime();
		if(m_nowTime - m_lastTime > abs(m_rotSpd)){
			m_nPhase++;
		}
		break;
	case FLOWER_PHASE_FLOWER:
		m_nPhase++;
		break;
	case FLOWER_PHASE_WAIT:
		RotationZ(m_rotSpd);
		break;
	case FLOWER_PHASE_UNINIT:
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
void CFlower::Animation()
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
void CFlower::Draw()
{
	CCharacter::DrawAlpha();

	//m_kuki->DrawAlpha();
}
//２つのベクトルABのなす角度θを求める
double CFlower::AngleOf2Vector(D3DXVECTOR3 A, D3DXVECTOR3 B )
{
	//ベクトルAとBの長さを計算する
	double length_A = D3DXVec3Length(&A);
	double length_B = D3DXVec3Length(&B);

	//内積とベクトル長さを使ってcosθを求める
	double cos_sita = D3DXVec3Dot(&A,&B) / ( length_A * length_B );

	//cosθからθを求める
	double sita = acos( cos_sita );	

	//ラジアンでなく0～180の角度でほしい場合はコメント外す
	sita = sita * 180.0 / (3.1415);

	return sita;
}
//========================================================================================
//	End of File
//========================================================================================