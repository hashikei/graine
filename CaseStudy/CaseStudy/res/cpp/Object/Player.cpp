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

#include<math.h>
#include <stdlib.h>

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
const LPCTSTR CPlayer::TACTILE_TEX_FILENAME = _T("res/img/GameScene/Object/Tactile.png");


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
	m_nGrane = PLAYER_NORMAL;

	m_PrevStatus = m_status;

	// スピード決定
	m_fSpeed = 0;
	m_fJumpSpeed = 0;

	m_nThrowNo = 0;
	m_nRL = 0;

	m_vFlower = D3DXVECTOR3(0, 0, 0);

	m_lastTime = CTimer::GetTime();
	m_nowTime = m_lastTime;

	m_bDelete = false;
	m_bCol = false;
	m_bChangeGrane = false;
	m_bAnimeFall = false;

	m_pShock = NULL;



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

	CCharacter::Init(D3DXVECTOR2(PLAYER_SIZE_X, PLAYER_SIZE_Y),
		D3DXVECTOR3(PLAYER_POS_DEFAULT_X, PLAYER_POS_DEFAULT_Y, 0));

	Initial();
	m_pShock->Init(D3DXVECTOR2(PLAYER_SIZE_X, PLAYER_SIZE_Y),
		D3DXVECTOR3(PLAYER_POS_DEFAULT_X, PLAYER_POS_DEFAULT_Y, 0));

	// アニメーション初期化
	StartAnimation();
	m_pShock->StartAnimation();

	UVDivision(0, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y);
	m_pShock->UVDivision(0, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y);

	// 状態を待機に
	m_nType = P_TYPE_WAIT;
	m_nGrane = PLAYER_NORMAL;
	m_status = ST_WAIT;
	AddStatus(ST_FLYING);

	m_pPlayer = NULL;
	m_pStage = NULL;

	m_fSpeed = PLAYER_MOVE_SPD;
	m_fJumpSpeed = JUMP_DEFAULT;

	m_colRadius = PLAYER_SIZE_X;

	m_nRL = 0;
	m_nPrevRL = 1;

	m_vFlower = D3DXVECTOR3(0, 0, 0);

	m_bDelete = false;
	m_bCol = false;
	m_bChangeGrane = false;
	m_bAnimeFall = false;
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

	CCharacter::Init(D3DXVECTOR2(PLAYER_SIZE_X, PLAYER_SIZE_Y), pos);

	Initial();
	m_pShock->Init(D3DXVECTOR2(PLAYER_SIZE_X, PLAYER_SIZE_Y), pos);

	// アニメーション初期化
	StartAnimation();
	m_pShock->StartAnimation();

	UVDivision(0, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y);
	m_pShock->UVDivision(0, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y);

	// 状態を待機に
	m_nType = P_TYPE_WAIT;
	m_nGrane = PLAYER_NORMAL;
	m_status = ST_WAIT;
	AddStatus(ST_FLYING);

	m_pPlayer = NULL;
	m_pStage = NULL;

	m_fSpeed = PLAYER_MOVE_SPD + (0.1f *(rand() % 10)) + rand() % 3;
	m_fJumpSpeed = JUMP_DEFAULT;

	m_colRadius = PLAYER_SIZE_X;

	m_nRL = 0;
	m_nPrevRL = 1;

	m_vFlower = D3DXVECTOR3(0, 0, 0);

	m_bDelete = false;
	m_bCol = false;
	m_bChangeGrane = false;
	m_bAnimeFall = false;
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
	m_pShock->Uninit();

	RefreshSingleAnimation();
	m_pShock->RefreshSingleAnimation();
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
	if (m_status & ST_NONE){
		m_status = ST_WAIT;
	}
	if (m_status & ST_MOVE){
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
	if (m_nType != P_TYPE_THROW_READY)
		AddStatus(ST_FLYING);

	// 補正
	float corre[4] = { 40, 40, 5, 40 };		// 右、左、上、下

	m_colStartLine = D3DXVECTOR2(prevPos.x, prevPos.y);
	// 当たり判定をサイズに合わせる
	float prevColRa = m_colRadius;
	m_colRadius *= m_scale.y;

	SubStatus(ST_LAND);

	for (int i = 0; i < m_pStage->GetColBoxMax(); i++){

		if (m_pStage->GetColBox(i)->GetType() == BLOCK_TYPE_0)
			m_pStage->GetColBox(i)->DisableCol();
		DisableCol();

		if (m_status & ST_MOVE){

			// 右方向(当たったかどうかだけ)
			m_colEndLine = D3DXVECTOR2(m_pos.x + m_colRadius / 2 - corre[0], m_pos.y);
			if (CollisionEnter(COL2D_LINESQUARE, m_pStage->GetColBox(i)) || CollisionStay(COL2D_LINESQUARE, m_pStage->GetColBox(i))){
				// ----- 当たってる

				// 移動を止める
				SubStatus(ST_MOVE);

				// 位置を当たったところに設定
				m_pos.x = m_lastColLinePos.x - m_colRadius / 2 + corre[0];
				EnableCol();
			}
			// 左方向(当たったかどうかだけ)
			m_colEndLine = D3DXVECTOR2(m_pos.x - m_colRadius / 2 + corre[1], m_pos.y);
			if (CollisionEnter(COL2D_LINESQUARE, m_pStage->GetColBox(i)) || CollisionStay(COL2D_LINESQUARE, m_pStage->GetColBox(i))){
				// ----- 当たってる

				// 移動を止める
				SubStatus(ST_MOVE);

				// 位置を当たったところに設定
				m_pos.x = m_lastColLinePos.x + m_colRadius / 2 - corre[1];
				EnableCol();
			}
		}
		// 下方向(当たったかどうかだけ)
		m_colEndLine = D3DXVECTOR2(m_pos.x, m_pos.y - m_colRadius / 2 + corre[2]);
		if (CollisionStay(COL2D_LINESQUARE, m_pStage->GetColBox(i))){
			// ----- 当たってる

			if (prevPos.y > m_lastColLinePos.y){
				// ジャンプ状態解除
				if (m_status & ST_JUMP){
					SubStatus(ST_JUMP);

					m_fJumpSpeed = JUMP_DEFAULT;
				}
				SubStatus(ST_FLYING);
				AddStatus(ST_LAND);
				// 位置を当たったところに設定
				m_pos.y = m_lastColLinePos.y + m_colRadius / 2 - corre[2];
				EnableCol();
			}
		}
		// 上方向
		m_colEndLine = D3DXVECTOR2(m_pos.x, m_pos.y + m_colRadius / 2 - corre[3]);
		if (CollisionEnter(COL2D_LINESQUARE, m_pStage->GetColBox(i)) || CollisionStay(COL2D_LINESQUARE, m_pStage->GetColBox(i))){
			// ----- 当たってる
			// ジャンプ状態解除
			SetGravity(0.98f);
			m_fJumpSpeed = JUMP_DEFAULT;
			SubStatus(ST_JUMP);
			// 位置を当たったところに設定
			m_pos.y = m_lastColLinePos.y - m_colRadius / 2 + corre[3];
			EnableCol();
		}

		if (m_bCol){
			// ブロックの種類によって当たった時に処理が変わる
			switch (m_pStage->GetColBox(i)->GetType())
			{
			case BLOCK_TYPE_0:
				// 当たってるブロックが分かりやすいように
				m_pStage->GetColBox(i)->SetColor(D3DXVECTOR3(128, 255, 128));
				m_pStage->GetColBox(i)->EnableCol();
				if (m_nType == P_TYPE_THROW){
					m_pStage->GetColBox(i)->AddFlower(1);
					m_nType = P_TYPE_FLOWER;
				}
			case BLOCK_TYPE_CLEAR:
				// 投げてるやつなら花にする
				if (m_nType == P_TYPE_THROW){
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
	// 当たり判定を元に戻す
	m_colRadius = prevColRa;

	Translate(m_pos);

	// 触覚と同期
	m_pShock->Translate(GetPosition());
	m_pShock->Rotate(GetRotation());
	m_pShock->Scale(GetScale());

	SoundEffect();

	Animation();

}//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 操作
//	Description : 操作するプレイヤーの動き
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayer::Draw()
{
	if (!m_bChangeGrane){
		switch (m_nGrane)
		{
		case PLAYER_NORMAL:
			SetColor(D3DXVECTOR3(128, 255, 128));
			m_pShock->SetColor(D3DXVECTOR3(128, 255, 128));
			break;
		case PLAYER_ARROW:
			m_scale = D3DXVECTOR3(-PLAYER_ARROW_SIZE, PLAYER_ARROW_SIZE, 0);
			SetColor(D3DXVECTOR3(0, 198, 255));
			m_pShock->SetColor(D3DXVECTOR3(0, 198, 255));
			m_bChangeGrane = true;
			break;
		case PLAYER_JACK:
			m_scale = D3DXVECTOR3(-PLAYER_JACK_SIZE, PLAYER_JACK_SIZE, 0);
			SetColor(D3DXVECTOR3(200, 255, 200));
			m_pShock->SetColor(D3DXVECTOR3(200, 255, 200));
			m_bChangeGrane = true;
			break;
		case PLAYER_STORN:
			m_scale = D3DXVECTOR3(-PLAYER_STORN_SIZE, PLAYER_STORN_SIZE, 0);
			SetColor(D3DXVECTOR3(145, 74, 0));
			m_pShock->SetColor(D3DXVECTOR3(145, 74, 0));
			m_bChangeGrane = true;
			break;
		}
	}

	// 方向
	if(m_nRL == 1)
	{
		m_scale.x = abs(m_scale.x);
	}
	if(m_nRL == 0)
	{
		m_scale.x = -abs(m_scale.x);
	}
	Scale(m_scale);
	m_pShock->Scale(m_scale);
	CCharacter::DrawAlpha();
	m_pShock->DrawAlpha();
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 操作
//	Description : 操作するプレイヤーの動き
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayer::moveControllerPlayer()
{
	if (GetPrsKey(DIK_RIGHT)){
		AddStatus(ST_MOVE);
		m_nRL = 0;
		TranslationX(m_fSpeed);
	}
	if (GetPrsKey(DIK_LEFT)){
		AddStatus(ST_MOVE);
		m_nRL = 1;
		TranslationX(-m_fSpeed);
	}
	if (GetTrgKey(DIK_LSHIFT) && !(m_status & ST_JUMP)){		// ジャンプ
		SubStatus(ST_LAND);
		AddStatus(ST_JUMP);
	}
	if (GetTrgKey(DIK_X) && !(m_status & ST_CALL))
	{
		AddStatus(ST_CALL);
	}

	if(m_status & ST_LAND){
		SetGravity(0.98f);
	}
	
	// ジャンプ中
	if (m_status & ST_JUMP){
		SetGravity(0.098f);
		TranslationY(m_fJumpSpeed);
		m_fJumpSpeed -= JUMP_GRAVITY;
		// 上昇が終わったら
		if (m_fJumpSpeed < 0){
			SetGravity(0.98f);
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

	if (D3DXVec3LengthSq(&(pos - m_pos)) < PLAYER_LENGTH * PLAYER_LENGTH){
		SubStatus(ST_MOVE);
		AddStatus(ST_WAIT);
		return;
	}
	else{
		AddStatus(ST_MOVE);
	}

	// 距離が遠いと止まる
	if (D3DXVec3LengthSq(&(pos - m_pos)) > WAIT_LENGTH * WAIT_LENGTH){
		SubStatus(ST_MOVE);
		AddStatus(ST_WAIT);
		m_nType = P_TYPE_WAIT;
		return;
	}

	D3DXVECTOR3 move;
	D3DXVec3Normalize(&move, &(pos - m_pos));
	if (m_status & ST_MOVE){
		if (move.x > 0)
			m_nRL = 0;
		else
			m_nRL = 1;
		m_pos.x += move.x * m_fSpeed;
	}

	if (m_pPlayer->GetStatus() & ST_JUMP){
		AddStatus(ST_JUMP);
	}
// ジャンプ中
	if (m_status & ST_JUMP){
		SetGravity(0.098f);
		TranslationY(m_fJumpSpeed);
		m_fJumpSpeed -= JUMP_GRAVITY;
		// 上昇が終わったら
		if (m_fJumpSpeed < 0){
			SetGravity(0.98f);
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

	float corre[2] = { 5.0f, 40 };

	SubStatus(ST_FLYING);
	AddStatus(ST_MOVE);

	float prev = m_colRadius;
	m_colRadius *= m_scale.y;

	if (m_nRL)
		pos = D3DXVECTOR3(m_pPlayer->GetPosition().x - corre[0], m_pPlayer->GetPosition().y + m_colRadius - corre[1], m_pPlayer->GetPosition().z);
	else
		pos = D3DXVECTOR3(m_pPlayer->GetPosition().x + corre[0], m_pPlayer->GetPosition().y + m_colRadius - corre[1], m_pPlayer->GetPosition().z);

	D3DXVec3Normalize(&move, &(pos - m_pos));
	m_pos += move * (m_fSpeed);
	if (D3DXVec3LengthSq(&(pos - m_pos)) < 1000){
		SubStatus(ST_MOVE);
		m_nType = P_TYPE_THROW_READY;
	}

	m_colRadius = prev;

	// 時間系
	m_nowTime = CTimer::GetTime();
	if ((m_nowTime - m_lastTime) > WAIT_LIMIT_TIME){
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

	float corre[2] = { 5.0f, 50 };

	float prev = m_colRadius;
	m_colRadius *= m_scale.y;
	corre[1] *= m_scale.y;

	if (m_nRL)
		pos = D3DXVECTOR3(m_pPlayer->GetPosition().x - corre[0], m_pPlayer->GetPosition().y + m_colRadius - corre[1], m_pPlayer->GetPosition().z);
	else
		pos = D3DXVECTOR3(m_pPlayer->GetPosition().x + corre[0], m_pPlayer->GetPosition().y + m_colRadius - corre[1], m_pPlayer->GetPosition().z);
	m_nRL = m_pPlayer->GetRL();

	m_colRadius = prev;

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
	AddStatus(ST_MOVE);
	switch (m_nGrane)
	{
	case PLAYER_NORMAL:
	case PLAYER_JACK:
		TranslationY(m_fJumpSpeed);
		m_fJumpSpeed -= JUMP_GRAVITY;
		// 上昇が終わったら
		if (m_fJumpSpeed < 0){
			m_fJumpSpeed = JUMP_DEFAULT;
		}
		if (m_nRL)
			TranslationX(-m_fSpeed * 3);
		else
			TranslationX(m_fSpeed * 3);
		break;
	case PLAYER_ARROW:
		SubStatus(ST_FLYING);
		if (m_nRL)
			TranslationX(-m_fSpeed * 10);
		else
			TranslationX(m_fSpeed * 10);
		break;
	case PLAYER_STORN:
		if (m_nRL)
			TranslationX(-m_fSpeed * 2);
		else
			TranslationX(m_fSpeed * 2);
		break;
	}

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
	case ST_WAIT  + ST_LAND:
		if (m_nType == P_TYPE_WAIT){
			m_bAnimeFall = true;
			m_pShock->SingleAnimation(114, 108, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.75f);
			SingleAnimation(114, 108, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.75f);
				//FrameAnimation(106,100, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.5f);
			
		}
		else{
			m_bAnimeFall = true;
			RefreshSingleAnimation();
			m_pShock->RefreshSingleAnimation();
			FrameAnimation(60, 61, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.5f);
			m_pShock->FrameAnimation(60, 61, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.5f);
		}
		break;
	case ST_WAIT + ST_MOVE + ST_LAND:
		m_bAnimeFall = true;
		RefreshSingleAnimation();
		m_pShock->RefreshSingleAnimation();
		FrameAnimation(0, 11, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.05f);
		m_pShock->FrameAnimation(0, 11, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.05f);
		break;
	case ST_WAIT + ST_JUMP + ST_FLYING:
	case ST_WAIT + ST_JUMP + ST_FLYING + ST_MOVE:
		RefreshSingleAnimation();
		m_pShock->RefreshSingleAnimation();
		FrameAnimation(48, 48, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.05f);
		m_pShock->FrameAnimation(48, 48, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.05f);
		break;
	case ST_WAIT + ST_FLYING:
	case ST_WAIT + ST_FLYING + ST_MOVE:
		// 時間経つとアニメーション開始
		/*if(m_bAnimeFall){
		m_nowTime = CTimer::GetTime();
		if((m_nowTime - m_lastTime) > FALL_LIMIT_TIME){
		if(SingleAnimation(48,53, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.25f)){
		m_bAnimeFall = false;
		m_lastTime = m_nowTime;
		}
		}
		}	*/
		break;
	}
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


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : プレイヤーを初期化する
//	Arguments   : None.
//	Returns     : 成否(true:成功)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CPlayer::Initial()
{
	m_pShock = CCharacter::Create(TACTILE_TEX_FILENAME);
	if (!m_pShock)
		return false;

	return true;
}

void CPlayer::Finalize()
{
	SAFE_RELEASE(m_pShock);
}

//========================================================================================
//	End of File
//========================================================================================