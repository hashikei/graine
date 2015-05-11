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
	m_nPhase = P_STOP;
	m_nType = P_TYPE_PLAYER; // これはあとで変えないとだからな
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
//	Name        : 解放処理
//	Description : オブジェクトデータを解放する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayer::Release()
{
	CObject::Release();
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
		m_pos.x += PLAYER_MOVE_SPD;

		if (m_nState != P_MOVE){
//			m_fLastTime = m_pTimer->GetTime();		// タイマ値を更新
			m_nState = P_MOVE;	// 動いてる
		}
		// 向いてる方向を変える
		if (m_scale.x < 0)
			m_scale.x = -m_scale.x;
		m_bStop = false;
	}	
	if (GetPrsKey(DIK_LEFT)){		// 左

		m_pos.x -= PLAYER_MOVE_SPD;

		if (m_nState != P_MOVE){
//			m_fLastTime = m_pTimer->GetTime();		// タイマ値を更新
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
		FrameAnimation(10, 14, 10, 2, 0.5);
//		TimeAnimation(0, 0, 10, 2, 0.05f);
		break;
	case P_MOVE:
		FrameAnimation(1, 6, 10, 2, 0.5);
//		TimeAnimation(1, 6, 10, 2, 0.05f);
		break;
	}
	
	
}
//========================================================================================
//	End of File
//========================================================================================