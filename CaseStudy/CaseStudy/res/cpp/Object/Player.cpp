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
#include "../../h/System/Timer.h"
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
	m_nNo = 0;				// 最初は全部0　すぐ変わる
	m_nType = P_TYPE_WAIT; // これはあとで変えないとだからな

	m_PrevStatus = m_status;

	// スピード決定
	m_fSpeed = 0;
	m_fJumpSpeed = 0;

	m_nThrowNo = 0;
	m_nRL		= 0;

	m_vFlower = D3DXVECTOR3(0,0,0);

	m_lastTime = CTimer::GetTime();
	m_nowTime = m_lastTime;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : 初期化
//	Arguments   : 
//	Returns     : 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayer::Init()
{
	// キャラクターの初期化
	CObject2D::Init();

	CCharacter::Init(D3DXVECTOR2(PLAYER_SIZE_X,PLAYER_SIZE_Y),
		D3DXVECTOR3(PLAYER_POS_DEFAULT_X,PLAYER_POS_DEFAULT_Y,0));

	// アニメーション初期化
	StartAnimation();

	UVDivision(0, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y);

	// 状態を待機に
	m_nType = P_TYPE_WAIT;
	m_status = ST_WAIT;
	AddStatus(ST_FLYING);

	m_pPlayer	= NULL;
	m_pStage	= NULL;

	m_fSpeed = PLAYER_MOVE_SPD + (0.1f *(rand() % 10)) + rand()%3;
	m_fJumpSpeed = JUMP_DEFAULT;

	m_colRadius = PLAYER_SIZE_X;

	m_nRL = 0;
	m_nPrevRL = 1;

	m_vFlower = D3DXVECTOR3(0,0,0);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : 初期化
//	Arguments   : 
//	Returns     : 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayer::Init(const D3DXVECTOR3& pos)
{
	// キャラクターの初期化
	CObject2D::Init();

	CCharacter::Init(D3DXVECTOR2(PLAYER_SIZE_X,PLAYER_SIZE_Y),pos);

	// アニメーション初期化
	StartAnimation();

	UVDivision(0, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y);

	// 状態を待機に
	m_nType = P_TYPE_WAIT;
		m_status = ST_WAIT;
	AddStatus(ST_FLYING);

	m_pPlayer	= NULL;
	m_pStage	= NULL;

	m_fSpeed = PLAYER_MOVE_SPD + (0.1f *(rand() % 10)) + rand()%3;
	m_fJumpSpeed = JUMP_DEFAULT;

	m_colRadius = PLAYER_SIZE_X;

	m_nRL = 0;
	m_nPrevRL = 1;

	m_vFlower = D3DXVECTOR3(0,0,0);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : 初期化
//	Arguments   : 
//	Returns     : 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayer::Uninit()
{
	CCharacter::Uninit();

	m_pPlayer = NULL;
	//delete this;
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
	m_PrevStatus = m_status;
	if(m_status & ST_NONE){
		m_status = ST_WAIT;
	}
	if(m_status & ST_MOVE){
		SubStatus(ST_MOVE);
	}
	D3DXVECTOR3 prevPos = m_pos; 

	switch (m_nType){
	case P_TYPE_PLAYER:
			moveControllerPlayer();
		break;
	case P_TYPE_OTHER:
			moveControllerOther();
		break;
	case P_TYPE_THROW_READY_READY:
			moveControllerThrowReadyReady();
		break;
	case P_TYPE_THROW_READY:
			moveControllerThrowReady();
		break;
	case P_TYPE_THROW:
			moveControllerThrow();
		break;
	case P_TYPE_FLOWER:
		break;
	}

	CCharacter::Update();
	
	// ----- 当たり判定
	AddStatus(ST_FLYING);

	// 補正
	float corre[4] = {40,40,5,40};		// 右、左、上、下

	m_colStartLine	= D3DXVECTOR2(prevPos.x,prevPos.y);
	for(int i = 0;i < m_pStage->GetColBoxMax();i++){

		if(m_pStage->GetColBox(i)->GetType() == BLOCK_TYPE_0)
			m_pStage->GetColBox(i)->DisableCol();
		DisableCol();

		if(m_status & ST_MOVE){

			// 右方向(当たったかどうかだけ)
			m_colEndLine	= D3DXVECTOR2(m_pos.x + m_colRadius / 2  - corre[0],m_pos.y);
			if(CollisionEnter(COL2D_LINESQUARE,m_pStage->GetColBox(i)) || CollisionStay(COL2D_LINESQUARE,m_pStage->GetColBox(i))){
				// ----- 当たってる

				// 移動を止める
				SubStatus(ST_MOVE);

				// 位置を当たったところに設定
				m_pos.x = m_lastColLinePos.x - m_colRadius / 2 + corre[0];
				EnableCol();
			}
			// 左方向(当たったかどうかだけ)
			m_colEndLine	= D3DXVECTOR2(m_pos.x - m_colRadius / 2 + corre[1],m_pos.y);
			if(CollisionEnter(COL2D_LINESQUARE,m_pStage->GetColBox(i)) || CollisionStay(COL2D_LINESQUARE,m_pStage->GetColBox(i))){
				// ----- 当たってる

				// 移動を止める
				SubStatus(ST_MOVE);

				// 位置を当たったところに設定
				m_pos.x = m_lastColLinePos.x + m_colRadius / 2 - corre[1];
				EnableCol();
			}
		}
		// 下方向(当たったかどうかだけ)
		m_colEndLine	= D3DXVECTOR2(m_pos.x,m_pos.y - m_colRadius / 2  + corre[2]);
		if(CollisionStay(COL2D_LINESQUARE,m_pStage->GetColBox(i))){
			// ----- 当たってる

			if(prevPos.y > m_lastColLinePos.y){
				// ジャンプ状態解除
				if(m_status & ST_JUMP){
					SubStatus(ST_JUMP);
					m_fJumpSpeed = JUMP_DEFAULT;
				}
				SubStatus(ST_FLYING);
				// 位置を当たったところに設定
				m_pos.y = m_lastColLinePos.y + m_colRadius / 2 - corre[2];
				EnableCol();
			}
		}
		// 上方向
		m_colEndLine	= D3DXVECTOR2(m_pos.x,m_pos.y + m_colRadius / 2  - corre[3]);
		if(CollisionEnter(COL2D_LINESQUARE,m_pStage->GetColBox(i)) || CollisionStay(COL2D_LINESQUARE,m_pStage->GetColBox(i))){
			// ----- 当たってる
			// ジャンプ状態解除
			SubStatus(ST_JUMP);
			m_fJumpSpeed = JUMP_DEFAULT;
			// 位置を当たったところに設定
			m_pos.y = m_lastColLinePos.y - m_colRadius / 2 + corre[3];
			EnableCol();
		}

		if(m_bCol){
			switch(m_pStage->GetColBox(i)->GetType())
			{
			case BLOCK_TYPE_0:
				// 当たってるブロックが分かりやすいように
				m_pStage->GetColBox(i)->SetColor(D3DXVECTOR3(128,255,128));
				m_pStage->GetColBox(i)->EnableCol();
				if(m_nType == P_TYPE_THROW){
					m_pStage->GetColBox(i)->AddFlower(1);
					m_nType = P_TYPE_FLOWER;
				}
			case BLOCK_TYPE_CLEAR:
				// 投げてるやつなら花にする
				if(m_nType == P_TYPE_THROW){
					m_pStage->GetColBox(i)->AddFlower(1);
					m_nType = P_TYPE_FLOWER;
				}
				break;
			case BLOCK_TYPE_OVER:
				// オーバブロックなら死ぬ
				m_bDelete = true;
				break;
			}
		}
	}
	Translate(m_pos);

	SoundEffect();

	Animation();

}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 操作
//	Description : 操作するプレイヤーの動き
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayer::moveControllerPlayer()
{
	if(GetPrsKey(DIK_RIGHT)){
		AddStatus(ST_MOVE);	
		m_nRL = 0;
		TranslationX(m_fSpeed);
	}
	if(GetPrsKey(DIK_LEFT)){
		AddStatus(ST_MOVE);
		m_nRL = 1;
		TranslationX(-m_fSpeed);
	}
	if(GetTrgKey(DIK_LSHIFT) && !(m_status & ST_JUMP) && !(m_status & ST_FLYING)){		// ジャンプ
		AddStatus(ST_JUMP);
	}
	if(GetTrgKey(DIK_X) && !(m_status & ST_CALL))
	{
		AddStatus(ST_CALL);
	}
	
	// ジャンプ中
	if(m_status & ST_JUMP){
		TranslationY(m_fJumpSpeed);
		m_fJumpSpeed -= JUMP_GRAVITY;
		// 上昇が終わったら
		if(m_fJumpSpeed < 0){
			m_fJumpSpeed = JUMP_DEFAULT;
			SubStatus(ST_JUMP);
		}
	}	
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
	if(D3DXVec3LengthSq(&(pos - m_pos)) < PLAYER_LENGTH * PLAYER_LENGTH){
		SubStatus(ST_MOVE);
		return;
	}else{
		AddStatus(ST_MOVE);
	}

	// 距離が遠いと止まる
	if(D3DXVec3LengthSq(&(pos - m_pos)) > WAIT_LENGTH * WAIT_LENGTH){
		m_status = ST_WAIT;
		AddStatus(ST_FLYING);
		m_nType = P_TYPE_WAIT;
	}
	D3DXVECTOR3 move;
	D3DXVec3Normalize(&move,&(pos - m_pos));
	if(m_status & ST_MOVE){
		if(move.x > 0)
			m_nRL = 0;
		else
			m_nRL = 1;
		m_pos.x += move.x * m_fSpeed * 0.9f;
	}

	if(m_pPlayer->GetStatus() & ST_JUMP){
		AddStatus(ST_JUMP);
		AddStatus(ST_FLYING);
	}
	// ジャンプ中
	if(m_status & ST_JUMP){
		TranslationY(m_fJumpSpeed);
		m_fJumpSpeed -= JUMP_GRAVITY;
		// 上昇が終わったら
		if(m_fJumpSpeed < 0){
			m_fJumpSpeed = JUMP_DEFAULT;
			SubStatus(ST_JUMP);
		}
	}	
	
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 操作
//	Description : ついてくるプレイヤーの動き
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayer::moveControllerThrowReadyReady()
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;

	float corre[2] = {5.0f,40};

	AddStatus(ST_MOVE);
	
	if(m_nRL)
		pos = D3DXVECTOR3(m_pPlayer->GetPosition().x - corre[0],m_pPlayer->GetPosition().y + m_colRadius - corre[1] ,m_pPlayer->GetPosition().z);
	else
		pos = D3DXVECTOR3(m_pPlayer->GetPosition().x + corre[0],m_pPlayer->GetPosition().y + m_colRadius - corre[1] ,m_pPlayer->GetPosition().z);
	
	D3DXVec3Normalize(&move,&(pos - m_pos));
		m_pos += move * (m_fSpeed);
	if(D3DXVec3LengthSq(&(pos - m_pos)) < 1000){
		SubStatus(ST_MOVE);
		m_nType = P_TYPE_THROW_READY;
	}
	
	// 時間系
	m_nowTime = CTimer::GetTime();
	if((m_nowTime - m_lastTime) > WAIT_LIMIT_TIME){
		m_nType = P_TYPE_WAIT;
		SubStatus(ST_MOVE);
	}
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 操作
//	Description : ついてくるプレイヤーの動き
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayer::moveControllerThrowReady()
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;

	m_status = ST_WAIT;

	float corre[2] = {5.0f,40};

	if(m_nRL)
		pos = D3DXVECTOR3(m_pPlayer->GetPosition().x - corre[0],m_pPlayer->GetPosition().y + m_colRadius - corre[1] ,m_pPlayer->GetPosition().z);
	else
		pos = D3DXVECTOR3(m_pPlayer->GetPosition().x + corre[0],m_pPlayer->GetPosition().y + m_colRadius - corre[1] ,m_pPlayer->GetPosition().z);
	m_nRL = m_pPlayer->GetRL();

	m_pos = pos;
	
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 操作
//	Description : ついてくるプレイヤーの動き
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayer::moveControllerThrow()
{
	if(m_nRL)
		TranslationX(-m_fSpeed * 2);
	else
		TranslationX(m_fSpeed * 2);
	
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : アニメ
//	Description : アニメーしょん
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayer::Animation()
{
	// 状態によってアニメーション変化
	switch (m_status)
	{
	case ST_WAIT:
		FrameAnimation(0,0, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.5f);
		break;
	case ST_WAIT + ST_MOVE:
		FrameAnimation(0,11, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.05f);
		break;
	}

	// 方向
	if(m_nRL != m_nPrevRL){
		m_scale.x = -m_scale.x;
		m_nPrevRL = m_nRL;
	}
	Scale(m_scale);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : SE
//	Description : Se
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayer::SoundEffect()
{
	// 状態によってアニメーション変化
	switch (m_status)
	{
	case ST_WAIT:				
		break;
	case ST_WAIT + ST_MOVE:		// 歩いてる
		break;
	}
}
//========================================================================================
//	End of File
//========================================================================================