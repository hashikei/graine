//========================================================================================
//		File        : Player.h
//		Program     : �v���C���[
//
//		Description : �v���C���[�ʂ̓���
//
//		History     : 2015/05/6	�쐬�J�n
//						   
//
//																Author : ���񂽂܃Y
//========================================================================================


//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �C���N���[�h
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
#include "../../h/System/Input.h"
#include "../../h/System/Timer.h"
#include "../../h/Scene/GameMain.h"
#include "../../h/Object/Player.h"

#include<math.h>
#include <stdlib.h>

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �萔��`
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
const LPCTSTR CPlayer::TACTILE_TEX_FILENAME[MAX_GRANE] = {
	_T("res/img/GameScene/Object/Tactile_1.png"),
	_T("res/img/GameScene/Object/Tactile_2.png"),
	_T("res/img/GameScene/Object/Tactile_3.png"),
	_T("res/img/GameScene/Object/Tactile_4.png"),
};

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �����o���̐錾
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
const float CPlayer::JUMP_DEFAULT = 25;		// �W�����v���x�̏����x
const float CPlayer::JUMP_GRAVITY = 1;		// �W�����v���x�̌���

const double CPlayer::WAIT_LIMIT_TIME = 3;		// �҂���ԂɂȂ鎞��

const double CPlayer::FALL_LIMIT_TIME = 1;		// �҂���ԂɂȂ鎞��

const float CPlayer::WAIT_LENGTH = 1000;

const float CPlayer::PLAYER_LENGTH = 80;		// ���삷���Ƃ��Ă���z�̋���

const float CPlayer::PLAYER_ARROW_SIZE = 1.1f;
const float CPlayer::PLAYER_JACK_SIZE = 1.3f;
const float CPlayer::PLAYER_STORN_SIZE = 1.8f;

const double CPlayer::WALK_SE_INTERVAL_TIME = 0.2;		// ����SE�̍Đ��Ԋu(�b��)


//========================================================================================
// public:
//========================================================================================
JOYINFOEX JoyInfoEx;
JOYINFOEX JoyInfoExPrev;
// �\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// using�錾
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
using namespace Input;
//����������������������������������������������������������������������������������������
//	Name        : �R���X�g���N�^
//	Arguments   : None.
//����������������������������������������������������������������������������������������
CPlayer::CPlayer()
{
	m_nNo = 0;				// �ŏ��͑S��0�@�����ς��
	m_nType = P_TYPE_WAIT; // ����͂��Ƃŕς��Ȃ��Ƃ������
	m_nGrane = PLAYER_NORMAL;

	m_PrevStatus = m_status;

	// �X�s�[�h����
	m_fSpeed = 0;
	m_fJumpSpeed = 0;

	m_nThrowNo = 0;
	m_nRL = 0;

	m_vFlower = D3DXVECTOR3(0, 0, 0);

	m_lastTime = CTimer::GetTime();
	m_nowTime = m_lastTime;
	m_walkTimer = 0.0;

	m_bDelete = false;
	m_bCol = false;
	m_bChangeGrane = false;
	m_bAnimeFall = false;

	m_pPlayer = NULL;

	m_pTactile = NULL;
	for (int i = 0; i < MAX_GRANE; ++i)
		m_pTactileTable[i] = NULL;
	
	m_clearFrame = 0;
}

//����������������������������������������������������������������������������������������
//	Name        : ������
//	Description : ������
//	Arguments   : 
//	Returns     : 
//����������������������������������������������������������������������������������������
void CPlayer::Init()
{
	// �L�����N�^�[�̏�����
	CObject2D::Init();

	CCharacter::Init(D3DXVECTOR2(PLAYER_SIZE_X, PLAYER_SIZE_Y),
		D3DXVECTOR3(PLAYER_POS_DEFAULT_X, PLAYER_POS_DEFAULT_Y, 0));

	for (int i = 0; i < MAX_GRANE; ++i) {
		m_pTactileTable[i]->Init(D3DXVECTOR2(PLAYER_SIZE_X, PLAYER_SIZE_Y),
			D3DXVECTOR3(PLAYER_POS_DEFAULT_X, PLAYER_POS_DEFAULT_Y, 0));
		m_pTactileTable[i]->StartAnimation();
		m_pTactileTable[i]->UVDivision(0, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y);
	}

	// �A�j���[�V����������
	StartAnimation();

	UVDivision(0, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y);

	// ��Ԃ�ҋ@��
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

	m_pTactile = m_pTactileTable[m_nGrane];

	m_walkTimer = 0.0;

	if(rand() % 2 == 0) {
		m_clearFrame = 84;
	} else {
		m_clearFrame = 96;
	}

	if(rand() % 2 == 0) {
		m_bClearJump = true;
	} else {
		m_bClearJump = false;
	}
}

//����������������������������������������������������������������������������������������
//	Name        : ������
//	Description : ������
//	Arguments   : 
//	Returns     : 
//����������������������������������������������������������������������������������������
void CPlayer::Init(const D3DXVECTOR3& pos)
{
	// �L�����N�^�[�̏�����
	CObject2D::Init();

	CCharacter::Init(D3DXVECTOR2(PLAYER_SIZE_X, PLAYER_SIZE_Y), pos);

	for (int i = 0; i < MAX_GRANE; ++i) {
		m_pTactileTable[i]->Init(D3DXVECTOR2(PLAYER_SIZE_X, PLAYER_SIZE_Y), pos);
		m_pTactileTable[i]->StartAnimation();
		m_pTactileTable[i]->UVDivision(0, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y);
	}

	// �A�j���[�V����������
	StartAnimation();

	UVDivision(0, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y);

	// ��Ԃ�ҋ@��
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

	m_pTactile = m_pTactileTable[m_nGrane];

	m_walkTimer = 0.0;
	
	if(rand() % 2 == 0) {
		m_clearFrame = 84;
	} else {
		m_clearFrame = 96;
	}

	if(rand() % 2 == 0) {
		m_bClearJump = true;
	} else {
		m_bClearJump = false;
	}
}

//����������������������������������������������������������������������������������������
//	Name        : ������
//	Description : ������
//	Arguments   : 
//	Returns     : 
//����������������������������������������������������������������������������������������
void CPlayer::Uninit()
{
	CCharacter::Uninit();
	RefreshSingleAnimation();
	
	for (int i = 0; i < MAX_GRANE; ++i) {
		m_pTactile->Uninit();
		m_pTactileTable[i]->RefreshSingleAnimation();
	}

	m_pPlayer = NULL;
	//delete this;
}
//����������������������������������������������������������������������������������������
//	Name        : ����
//	Description : �I�u�W�F�N�g�𐶐�����
//	Arguments   : pszFName / �ǂݍ��݃t�@�C����
//	Returns     : �I�u�W�F�N�g�f�[�^
//����������������������������������������������������������������������������������������
CPlayer* CPlayer::Create(const LPCTSTR pszFName)
{
	// ----- �ϐ��錾
	CPlayer* pObj;

	// ----- ����������
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

//����������������������������������������������������������������������������������������
//	Name        : �X�V
//	Description : �����ȍX�V
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
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

	if(m_status & ST_JACK){
		if(m_status & ST_JUMP){
			SubStatus(ST_JUMP);
		}
	}

	if(!(m_status & ST_JUMP)){
		SetGravity(DEFAULT_GRAVITY);
		m_fJumpSpeed = JUMP_DEFAULT;
	}

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

	if(m_status & ST_JACK){
		if(m_status & ST_FLYING)
			SubStatus(ST_FLYING);
	}

	CCharacter::Update();

	if(m_status & ST_JACK)
		SubStatus(ST_JACK);

	// ----- �����蔻��
	if (m_nType != P_TYPE_THROW_READY || m_nType != P_TYPE_THROW_READY){
		AddStatus(ST_FLYING);
	}

	// �␳
	float corre[4] = { 40, 40, 5, 40 };		// �E�A���A��A��

	m_colStartLine = D3DXVECTOR2(prevPos.x, prevPos.y);
	// �����蔻����T�C�Y�ɍ��킹��
	float prevColRa = m_colRadius;
	m_colRadius *= m_scale.y;

	if(m_status & ST_LAND)
		SubStatus(ST_LAND);
	
	for (int j = 0; j < m_pStage->GetMaxFieldBlock(); j++){
		CFieldBlock* pFieldBlock = m_pStage->GetFieldBlock(j);
		for (int i = 0; i < pFieldBlock->GetElementNum(); i++){
			CCharacter* pObj = pFieldBlock->GetElement(i);

			if (pFieldBlock->GetType() == CMapData::BT_NORMAL)
				pFieldBlock->DisableCol();
			DisableCol();

			if(CollisionStay(COL2D_CIRCLESQUARE,pObj) && pObj->GetType() == 1 &&!( m_status & ST_JUMP))
			{
				if(m_status & ST_FLYING){
					SubStatus(ST_FLYING);
				}
				SetGravity(DEFAULT_GRAVITY);
				m_fJumpSpeed = JUMP_DEFAULT;
				if(m_status & ST_JUMP)
					SubStatus(ST_JUMP);
				AddStatus(ST_JACK);
			}else{

				if (m_status & ST_MOVE){

					// �E����(�����������ǂ�������)
					m_colEndLine = D3DXVECTOR2(m_pos.x + m_colRadius / 2 - corre[0], m_pos.y);
					if (CollisionEnter(COL2D_LINESQUARE, pObj) || CollisionStay(COL2D_LINESQUARE, pObj)){
						// ----- �������Ă�
						if(pObj->GetType() != 1){
							// �ړ����~�߂�
							SubStatus(ST_MOVE);

							// �ʒu�𓖂������Ƃ���ɐݒ�
							m_pos.x = m_lastColLinePos.x - m_colRadius / 2 + corre[0];
							EnableCol();
						}
					}
					// ������(�����������ǂ�������)
					m_colEndLine = D3DXVECTOR2(m_pos.x - m_colRadius / 2 + corre[1], m_pos.y);
					if (CollisionEnter(COL2D_LINESQUARE, pObj) || CollisionStay(COL2D_LINESQUARE, pObj)){
						// ----- �������Ă�
						if(pObj->GetType() != 1){
							// �ړ����~�߂�
							SubStatus(ST_MOVE);

							// �ʒu�𓖂������Ƃ���ɐݒ�
							m_pos.x = m_lastColLinePos.x + m_colRadius / 2 - corre[1];
							EnableCol();
						}
					}
				}
				// ������(�����������ǂ�������)
				m_colEndLine = D3DXVECTOR2(m_pos.x, m_pos.y - m_colRadius / 2 + corre[2]);
				if (CollisionStay(COL2D_LINESQUARE, pObj)){
					// ----- �������Ă�
					if (prevPos.y > m_lastColLinePos.y){
						if(pObj->GetType() != 1){
						
							// �W�����v��ԉ���
							SubStatus(ST_FLYING);
							AddStatus(ST_LAND);
							// �ʒu�𓖂������Ƃ���ɐݒ�
							m_pos.y = m_lastColLinePos.y + m_colRadius / 2 - corre[2];
							EnableCol();
						}

					}
				
				}

				// �����
				m_colEndLine = D3DXVECTOR2(m_pos.x, m_pos.y + m_colRadius / 2 - corre[3]);
				if (CollisionEnter(COL2D_LINESQUARE, pObj) || CollisionStay(COL2D_LINESQUARE, pObj)){
					// ----- �������Ă�
					if(pObj->GetType() != 1){
						// �W�����v��ԉ���
						SetGravity(DEFAULT_GRAVITY);
						m_fJumpSpeed = JUMP_DEFAULT;
						if(m_status & ST_JUMP)
							SubStatus(ST_JUMP);
						// �ʒu�𓖂������Ƃ���ɐݒ�
						m_pos.y = m_lastColLinePos.y - m_colRadius / 2 + corre[3];
						EnableCol();
					}
				}

				if (m_bCol){
					// �u���b�N�̎�ނɂ���ē����������ɏ������ς��
					switch (pFieldBlock->GetType())
					{
						case CMapData::BT_NORMAL:
							pFieldBlock->EnableCol();
							if (m_nType == P_TYPE_THROW || m_nType == P_TYPE_THROW_READY){
								CGameMain::PlaySE(SE_UMARU);
								pFieldBlock->AddFlower(1);
								m_nType = P_TYPE_FLOWER;
							}
						case CMapData::BT_CLEAR:
							// �����Ă��Ȃ�Ԃɂ���
							if (m_nType == P_TYPE_THROW){
								CGameMain::PlaySE(SE_UMARU);
								pFieldBlock->AddFlower(1);
								m_nType = P_TYPE_FLOWER;
							}
							break;
						case CMapData::BT_OVER:
							// �I�[�o�u���b�N�Ȃ玀��
							m_bDelete = true;
							break;
					}
				}
			}
		}
	}

	// �����蔻������ɖ߂�
	m_colRadius = prevColRa;

	Translate(m_pos);

	// �G�o�Ɠ���
	m_pTactile->Translate(GetPosition());
	m_pTactile->TranslationZ(1.0f);
	m_pTactile->Rotate(GetRotation());
	m_pTactile->Scale(GetScale());

	SoundEffect();

	Animation();

}

//����������������������������������������������������������������������������������������
//	Name        : ����
//	Description : ���삷��v���C���[�̓���
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CPlayer::Draw()
{
	if (!m_bChangeGrane){
		switch (m_nGrane)
		{
		case PLAYER_NORMAL:
			SetColor(D3DXVECTOR3(128, 255, 128));
			m_pTactile->SetColor(D3DXVECTOR3(128, 255, 128));
			break;
		case PLAYER_ARROW:
			m_scale = D3DXVECTOR3(-PLAYER_ARROW_SIZE, PLAYER_ARROW_SIZE, 0);
			SetColor(D3DXVECTOR3(0, 198, 255));
			m_pTactile->SetColor(D3DXVECTOR3(0, 198, 255));
			m_bChangeGrane = true;
			break;
		case PLAYER_JACK:
			m_scale = D3DXVECTOR3(-PLAYER_JACK_SIZE, PLAYER_JACK_SIZE, 0);
			SetColor(D3DXVECTOR3(200, 255, 200));
			m_pTactile->SetColor(D3DXVECTOR3(200, 255, 200));
			m_bChangeGrane = true;
			break;
		case PLAYER_STONE:
			m_scale = D3DXVECTOR3(-PLAYER_STORN_SIZE, PLAYER_STORN_SIZE, 0);
			SetColor(D3DXVECTOR3(145, 74, 0));
			m_pTactile->SetColor(D3DXVECTOR3(145, 74, 0));
			m_bChangeGrane = true;
			break;
		}
	}

	// ����
	if(m_nRL == 1)
	{
		m_scale.x = abs(m_scale.x);
	}
	if(m_nRL == 0)
	{
		m_scale.x = -abs(m_scale.x);
	}
	Scale(m_scale);
	m_pTactile->Scale(m_scale);
	CCharacter::DrawAlpha();
	m_pTactile->DrawAlpha();
}
//����������������������������������������������������������������������������������������
//	Name        : ����
//	Description : ���삷��v���C���[�̓���
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CPlayer::moveControllerPlayer()
{
	JoyInfoEx.dwSize = sizeof(JOYINFOEX);
	JoyInfoEx.dwFlags = JOY_RETURNALL; // �S�Ă̏����擾

	if(joyGetPosEx(0, &JoyInfoEx) == JOYERR_NOERROR){ // ����
	
	if (GetPrsKey(DIK_RIGHT) || JoyInfoEx.dwXpos > 0x7FFF + 0x4000){
		if(CTimer::GetTime() - m_walkTimer > WALK_SE_INTERVAL_TIME &&
			!CheckStatus(ST_FLYING)) {
			m_walkTimer = CTimer::GetTime();
			CGameMain::PlaySE(SE_WALK);
		}
		AddStatus(ST_MOVE);
		m_nRL = 0;
		if (CMapData::GetRightWallX() > GetPosX())
			TranslationX(m_fSpeed);
	}
	if (GetPrsKey(DIK_LEFT) || JoyInfoEx.dwXpos < 0x7FFF - 0x4000){
		if(CTimer::GetTime() - m_walkTimer > WALK_SE_INTERVAL_TIME &&
			!CheckStatus(ST_FLYING)) {
			m_walkTimer = CTimer::GetTime();
			CGameMain::PlaySE(SE_WALK);
		}
		AddStatus(ST_MOVE);
		m_nRL = 1;
		if (CMapData::GetLeftWallX() < GetPosX())
			TranslationX(-m_fSpeed);
	}
	if ((GetTrgKey(DIK_LSHIFT)|| ((JoyInfoEx.dwButtons & JOY_BUTTON1) && !JoyInfoExPrev.dwButtons & JOY_BUTTON1)) && !(m_status & ST_JUMP)){		// �W�����v
		CGameMain::PlaySE(SE_JUMP,0,1);
		SubStatus(ST_LAND);
		AddStatus(ST_JUMP);
	}
	if ((GetTrgKey(DIK_X)  || ((JoyInfoEx.dwButtons & JOY_BUTTON2) && !(JoyInfoExPrev.dwButtons & JOY_BUTTON2)))&& !(m_status & ST_CALL))
	{
		AddStatus(ST_CALL);
	}

	if(m_status & ST_LAND){
		SetGravity(DEFAULT_GRAVITY);
	}
	
	// �W�����v��
	if (m_status & ST_JUMP){
		SetGravity(DEFAULT_GRAVITY / 10);
		TranslationY(m_fJumpSpeed);
		m_fJumpSpeed -= JUMP_GRAVITY;
		// �㏸���I�������
		if (m_fJumpSpeed < 0){
			SetGravity(DEFAULT_GRAVITY);
			m_fJumpSpeed = JUMP_DEFAULT;
			SubStatus(ST_JUMP);
		}
	}
	}

	JoyInfoExPrev = JoyInfoEx;
}
//����������������������������������������������������������������������������������������
//	Name        : ����
//	Description : ���Ă���v���C���[�̓���
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CPlayer::moveControllerOther()
{
	// �������߂�������t���Ă��Ȃ�
	D3DXVECTOR3 pos = m_pPlayer->GetPosition();

	if (D3DXVec3LengthSq(&(pos - m_pos)) < PLAYER_LENGTH * PLAYER_LENGTH){
		SubStatus(ST_MOVE);
		AddStatus(ST_WAIT);
		return;
	}
	else{
		AddStatus(ST_MOVE);
	}

	// �����������Ǝ~�܂�
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
// �W�����v��
	if (m_status & ST_JUMP){
		SetGravity(0.098f);
		TranslationY(m_fJumpSpeed);
		m_fJumpSpeed -= JUMP_GRAVITY;
		// �㏸���I�������
		if (m_fJumpSpeed < 0){
			SetGravity(0.98f);
			m_fJumpSpeed = JUMP_DEFAULT;
			SubStatus(ST_JUMP);
		}
	}

}
//����������������������������������������������������������������������������������������
//	Name        : ����
//	Description : ���Ă���v���C���[�̓���
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CPlayer::moveControllerThrowReadyReady()
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;

	if(m_pPlayer == NULL)
		return;

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

	// ���Ԍn
	m_nowTime = CTimer::GetTime();
	if ((m_nowTime - m_lastTime) > WAIT_LIMIT_TIME){
		m_nType = P_TYPE_WAIT;
		SubStatus(ST_MOVE);
	}
}
//����������������������������������������������������������������������������������������
//	Name        : ����
//	Description : ���Ă���v���C���[�̓���
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CPlayer::moveControllerThrowReady()
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;

	if(m_pPlayer == NULL)
		return;

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
//����������������������������������������������������������������������������������������
//	Name        : ����
//	Description : ���Ă���v���C���[�̓���
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CPlayer::moveControllerThrow()
{
	AddStatus(ST_MOVE);
	switch (m_nGrane)
	{
	case PLAYER_NORMAL:
	case PLAYER_JACK:
		TranslationY(m_fJumpSpeed / 1.5f);
		m_fJumpSpeed -= JUMP_GRAVITY;
		// �㏸���I�������
		if (m_fJumpSpeed < 0){
			m_fJumpSpeed = JUMP_DEFAULT;
		}
		if (m_nRL)
			TranslationX(-m_fSpeed * 1.5f);
		else
			TranslationX(m_fSpeed * 1.5f);
		break;
	case PLAYER_ARROW:
		SubStatus(ST_FLYING);
		if (m_nRL)
			TranslationX(-m_fSpeed * 5);
		else
			TranslationX(m_fSpeed * 5);
		break;
	case PLAYER_STONE:
		if (m_nRL)
			TranslationX(-m_fSpeed * 2);
		else
			TranslationX(m_fSpeed * 2);
		break;
	}

}
//����������������������������������������������������������������������������������������
//	Name        : �A�j��
//	Description : �A�j���[�����
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CPlayer::Animation()
{
	// ��Ԃɂ���ăA�j���[�V�����ω�

	switch (m_status)
	{
	case ST_WAIT  + ST_LAND:
		if (m_nType == P_TYPE_WAIT){
			m_bAnimeFall = true;

			m_pTactile->SingleAnimation(114, 108, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.75f);
			SingleAnimation(114, 108, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.75f);
			//FrameAnimation(106,100, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.5f);			
		}
		else{
			m_bAnimeFall = true;
			RefreshSingleAnimation();
			m_pTactile->RefreshSingleAnimation();
			FrameAnimation(60, 61, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.5f);
			m_pTactile->FrameAnimation(60, 61, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.5f);
		}
		break;
	case ST_WAIT + ST_MOVE + ST_LAND:
		m_bAnimeFall = true;
		RefreshSingleAnimation();
		m_pTactile->RefreshSingleAnimation();
		FrameAnimation(0, 11, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.05f);
		m_pTactile->FrameAnimation(0, 11, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.05f);
		break;
	case ST_WAIT + ST_JUMP + ST_FLYING:
	case ST_WAIT + ST_JUMP + ST_FLYING + ST_MOVE:
		RefreshSingleAnimation();
		m_pTactile->RefreshSingleAnimation();
		FrameAnimation(48, 48, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.05f);
		m_pTactile->FrameAnimation(48, 48, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.05f);
		break;
	case ST_WAIT + ST_FLYING:
	case ST_WAIT + ST_FLYING + ST_MOVE:
		// ���Ԍo�ƃA�j���[�V�����J�n
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
//����������������������������������������������������������������������������������������
//	Name        : SE
//	Description : Se
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CPlayer::SoundEffect()
{
	// ��Ԃɂ���ăA�j���[�V�����ω�
	switch (m_status)
	{
	case ST_WAIT:
		break;
	case ST_WAIT + ST_MOVE:		// �����Ă�
		break;
	}
}

//����������������������������������������������������������������������������������������
//	Name        : �N���A���o
//	Description : �N���A���̂��˂ۂ�̓��쉉�o����
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CPlayer::ClearDirection()
{
	if(m_bClearJump) {
		m_PrevStatus = m_status;
		if (m_status & ST_NONE){
			m_status = ST_WAIT;
		}
		if (m_status & ST_MOVE){
			SubStatus(ST_MOVE);
		}
		D3DXVECTOR3 prevPos = m_pos;

		if(m_status & ST_JACK){
			if(m_status & ST_JUMP){
				SubStatus(ST_JUMP);
			}
		}

		if(!(m_status & ST_JUMP)){
			SetGravity(DEFAULT_GRAVITY);
			m_fJumpSpeed = JUMP_DEFAULT;
		}

		if(m_status & ST_JACK){
			if(m_status & ST_FLYING)
				SubStatus(ST_FLYING);
		}

		CCharacter::Update();

		if(m_status & ST_JACK)
			SubStatus(ST_JACK);

		// ----- �����蔻��
		if (m_nType != P_TYPE_THROW_READY || m_nType != P_TYPE_THROW_READY){
			AddStatus(ST_FLYING);
		}

		// �␳
		float corre[4] = { 40, 40, 5, 40 };		// �E�A���A��A��

		m_colStartLine = D3DXVECTOR2(prevPos.x, prevPos.y);
		// �����蔻����T�C�Y�ɍ��킹��
		float prevColRa = m_colRadius;
		m_colRadius *= m_scale.y;

		if(m_status & ST_LAND)
			SubStatus(ST_LAND);
	
		for (int j = 0; j < m_pStage->GetMaxFieldBlock(); j++){
			CFieldBlock* pFieldBlock = m_pStage->GetFieldBlock(j);
			for (int i = 0; i < pFieldBlock->GetElementNum(); i++){
				CCharacter* pObj = pFieldBlock->GetElement(i);

				if (pFieldBlock->GetType() == CMapData::BT_NORMAL)
					pFieldBlock->DisableCol();
				DisableCol();

				if(CollisionStay(COL2D_CIRCLESQUARE,pObj) && pObj->GetType() == 1 &&!( m_status & ST_JUMP))
				{
					if(m_status & ST_FLYING){
						SubStatus(ST_FLYING);
					}
					SetGravity(DEFAULT_GRAVITY);
					m_fJumpSpeed = JUMP_DEFAULT;
					if(m_status & ST_JUMP)
						SubStatus(ST_JUMP);
					AddStatus(ST_JACK);
				}else{

					if (m_status & ST_MOVE){

						// �E����(�����������ǂ�������)
						m_colEndLine = D3DXVECTOR2(m_pos.x + m_colRadius / 2 - corre[0], m_pos.y);
						if (CollisionEnter(COL2D_LINESQUARE, pObj) || CollisionStay(COL2D_LINESQUARE, pObj)){
							// ----- �������Ă�
							if(pObj->GetType() != 1){
								// �ړ����~�߂�
								SubStatus(ST_MOVE);

								// �ʒu�𓖂������Ƃ���ɐݒ�
								m_pos.x = m_lastColLinePos.x - m_colRadius / 2 + corre[0];
								EnableCol();
							}
						}
						// ������(�����������ǂ�������)
						m_colEndLine = D3DXVECTOR2(m_pos.x - m_colRadius / 2 + corre[1], m_pos.y);
						if (CollisionEnter(COL2D_LINESQUARE, pObj) || CollisionStay(COL2D_LINESQUARE, pObj)){
							// ----- �������Ă�
							if(pObj->GetType() != 1){
								// �ړ����~�߂�
								SubStatus(ST_MOVE);

								// �ʒu�𓖂������Ƃ���ɐݒ�
								m_pos.x = m_lastColLinePos.x + m_colRadius / 2 - corre[1];
								EnableCol();
							}
						}
					}
					// ������(�����������ǂ�������)
					m_colEndLine = D3DXVECTOR2(m_pos.x, m_pos.y - m_colRadius / 2 + corre[2]);
					if (CollisionStay(COL2D_LINESQUARE, pObj)){
						// ----- �������Ă�
						if (prevPos.y > m_lastColLinePos.y){
							if(pObj->GetType() != 1){
						
								// �W�����v��ԉ���
								SubStatus(ST_FLYING);
								AddStatus(ST_LAND);
								// �ʒu�𓖂������Ƃ���ɐݒ�
								m_pos.y = m_lastColLinePos.y + m_colRadius / 2 - corre[2];
								EnableCol();
							}

						}
				
					}

					// �����
					m_colEndLine = D3DXVECTOR2(m_pos.x, m_pos.y + m_colRadius / 2 - corre[3]);
					if (CollisionEnter(COL2D_LINESQUARE, pObj) || CollisionStay(COL2D_LINESQUARE, pObj)){
						// ----- �������Ă�
						if(pObj->GetType() != 1){
							// �W�����v��ԉ���
							SetGravity(DEFAULT_GRAVITY);
							m_fJumpSpeed = JUMP_DEFAULT;
							if(m_status & ST_JUMP)
								SubStatus(ST_JUMP);
							// �ʒu�𓖂������Ƃ���ɐݒ�
							m_pos.y = m_lastColLinePos.y - m_colRadius / 2 + corre[3];
							EnableCol();
						}
					}

					if (m_bCol){
						// �u���b�N�̎�ނɂ���ē����������ɏ������ς��
						switch (pFieldBlock->GetType())
						{
							case CMapData::BT_NORMAL:
								pFieldBlock->EnableCol();
								if (m_nType == P_TYPE_THROW || m_nType == P_TYPE_THROW_READY){
									CGameMain::PlaySE(SE_UMARU);
									pFieldBlock->AddFlower(1);
									m_nType = P_TYPE_FLOWER;
								}
							case CMapData::BT_CLEAR:
								// �����Ă��Ȃ�Ԃɂ���
								if (m_nType == P_TYPE_THROW){
									CGameMain::PlaySE(SE_UMARU);
									pFieldBlock->AddFlower(1);
									m_nType = P_TYPE_FLOWER;
								}
								break;
							case CMapData::BT_OVER:
								// �I�[�o�u���b�N�Ȃ玀��
								m_bDelete = true;
								break;
						}
					}
				}
			}
		}

		// �����蔻������ɖ߂�
		m_colRadius = prevColRa;

		if(!(m_status & ST_JUMP)){
			SetGravity(DEFAULT_GRAVITY);
			m_fJumpSpeed = JUMP_DEFAULT;
		}

		if(m_status & ST_LAND){
			SetGravity(DEFAULT_GRAVITY);
			AddStatus(ST_JUMP);
		}
	
		// �W�����v��
		if (m_status & ST_JUMP){
			SetGravity(DEFAULT_GRAVITY / 10);
			TranslationY(m_fJumpSpeed);
			m_fJumpSpeed -= JUMP_GRAVITY;
			// �㏸���I�������
			if (m_fJumpSpeed < 0){
				SetGravity(DEFAULT_GRAVITY);
				m_fJumpSpeed = JUMP_DEFAULT;
				SubStatus(ST_JUMP);
			}
		}
	
		Translate(m_pos);

		// �G�o�Ɠ���
		m_pTactile->Translate(GetPosition());
		m_pTactile->TranslationZ(1.0f);
		m_pTactile->Rotate(GetRotation());
		m_pTactile->Scale(GetScale());
	}

	FrameAnimation(m_clearFrame, m_clearFrame + 7, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.1f);
	m_pTactile->FrameAnimation(m_clearFrame, m_clearFrame + 7, PLAYER_ANIME_SIZE_X, PLAYER_ANIME_SIZE_Y, 0.1f);
}

//����������������������������������������������������������������������������������������
//	Name        : ������
//	Description : �I�u�W�F�N�g������������
//	Arguments   : pszFName / �t�@�C����
//	Returns     : ����
//����������������������������������������������������������������������������������������
bool CPlayer::Initialize(const LPCTSTR pszFName)
{
	// ----- �e�N�X�`���ǂݍ���
	if (!CCharacter::Initialize(pszFName))
		return false;

	for (int i = 0; i < MAX_GRANE; ++i) {
		m_pTactileTable[i] = CCharacter::Create(TACTILE_TEX_FILENAME[i]);
		if (!m_pTactileTable[i])
			return false;
	}
	m_pTactile = m_pTactileTable[PLAYER_NORMAL];

	return true;
}

//����������������������������������������������������������������������������������������
//	Name        : ��n��
//	Description : �I�u�W�F�N�g�̏I������������
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CPlayer::Finalize()
{
	for (int i = MAX_GRANE - 1; i >= 0; --i) {
		SAFE_RELEASE(m_pTactileTable[i]);
	}
}

//========================================================================================
//	End of File
//========================================================================================