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


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/Input.h"
#include "../../h/Object/Player.h"

//========================================================================================
// public:
//========================================================================================
// ――――――――――――――――――――――――――――――――――――――――――――
// using宣言
//――――――――――――――――――――――――――――――――――――――――――――
using namespace Input;
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CPlayer::CPlayer()
{
	m_bStop = true;			// 止まってる
	m_nNo = 0;				// 最初は全部0　すぐ変わる
	m_nPhase = P_STOP;
	m_nType = P_TYPE_OTHER; // これはあとで変えないとだからな

	// スピード決定
	
	m_fSpeed = PLAYER_MOVE_SPD + (0.1f *(rand() % 10)) + rand()%3;

	m_fGravity = GRAVITY_CASE_2;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成
//	Description : オブジェクトを生成する
//	Arguments   : pszFName / 読み込みファイル名
//	Returns     : オブジェクトデータ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CPlayer* CPlayer::Create(const LPCTSTR pszFName)
{
	// ----- 変数宣言
	CPlayer* pObj;

	// ----- 初期化処理
	pObj = new CPlayer();
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
void CPlayer::Update()
{
	switch (m_nType){
	case P_TYPE_PLAYER:
			moveControllerPlayer();
		break;
	case P_TYPE_OTHER:
			moveControllerOther();
		break;
	}

	Animation();

	// これは絶対に最後に来るようにね☆
	CObject2Dsub::Update();
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 操作
//	Description : 操作するプレイヤーの動き
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayer::moveControllerPlayer()
{
	m_bStop = true;
	// キー入力
	if (GetPrsKey(DIK_RIGHT)){		// 右
		m_pos.x += m_fSpeed;

		if (m_nState != P_MOVE){
			m_nState = P_MOVE;	// 動いてる
		}
		// 向いてる方向を変える
		if (m_scale.x < 0)
			m_scale.x = -m_scale.x;
		m_bStop = false;
	}	
	if (GetPrsKey(DIK_LEFT)){		// 左

		m_pos.x -= m_fSpeed;

		if (m_nState != P_MOVE){
			m_nState = P_MOVE;	// 動いてる
		}
		// 向いてる方向を変える
		if (m_scale.x > 0)
			m_scale.x = -m_scale.x;
		m_bStop = false;
	}
	if (m_bStop == true)
		m_nState = P_STOP;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 操作
//	Description : ついてくるプレイヤーの動き
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayer::moveControllerOther()
{
	// 距離が近かったら付いてこない
	D3DXVECTOR3 pos = m_pPlayer->GetPosition();
	if(D3DXVec3LengthSq(&(pos - m_pos)) < PLAYER_LENGTH * PLAYER_LENGTH)
		return;

	D3DXVECTOR3 move;
	D3DXVec3Normalize(&move,&(pos - m_pos));
	m_pos += move * m_fSpeed;
	
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : アニメ
//	Description : アニメーしょん
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayer::Animation()
{
	// m_nStateによってアニメーション変化
	switch (m_nState)
	{
	case P_STOP:
		FrameAnimation(1, 7, 10, 2, 0.5f);
//		TimeAnimation(0, 0, 10, 2, 0.05f);
		break;
	case P_MOVE:
		FrameAnimation(1, 7, 10, 2, 0.1f);
//		TimeAnimation(1, 6, 10, 2, 0.05f);
		break;
	}
	
	
}
//========================================================================================
//	End of File
//========================================================================================