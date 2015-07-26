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

#pragma once

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �C���N���[�h
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
#include "../System/PreInclude.h"
#
#include <d3d9.h>
#include <d3dx9.h>
#include "../System/Input.h"
#include "../System/System.h"
#include "../Object/Character.h"
#include "../Object/Stage.h"

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �萔��`
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\

#define PLAYER_ANIME_SIZE_X	(12)	// �e�N�X�`���̕�����
#define PLAYER_ANIME_SIZE_Y	(10)

const float PLAYER_POS_DEFAULT_X = -300;
const float PLAYER_POS_DEFAULT_Y = 100;

const float PLAYER_SIZE_X = 128;
const float PLAYER_SIZE_Y = 128;

#define PLAYER_ANIME_STOP	(1)
#define PLAYER_ANIME_MOVE	(6)
#define PLAYER_ANIME_JUMP	(2)

static const float PLAYER_MOVE_SPD = 10;	// �X�s�[�h

enum PLAYER_TYPE
{
	P_TYPE_PLAYER = 0,
	P_TYPE_OTHER,
	P_TYPE_THROW_READY_READY,
	P_TYPE_THROW_READY,
	P_TYPE_THROW,

	P_TYPE_FLOWER,

	P_TYPE_WAIT,

	P_TYPE_DELETE,

	MAX_PLAYER_TYPE
};

enum PLAYER_GRANE
{
	PLAYER_NORMAL = 0,
	PLAYER_ARROW,
	PLAYER_JACK,
	PLAYER_STONE,

	MAX_GRANE,
};

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �N���X��`
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
class CPlayer : public CCharacter
{
public:
	static const LPCTSTR TACTILE_TEX_FILENAME[];		// �G�o�e�N�X�`���̃t�@�C����

private:
	static const float JUMP_DEFAULT;		// �W�����v���x�̏����x
	static const float JUMP_GRAVITY;		// �W�����v���x�̌���

	static const double WAIT_LIMIT_TIME;		// �҂���ԂɂȂ鎞��

	static const double FALL_LIMIT_TIME;		// �҂���ԂɂȂ鎞��

	static const float WAIT_LENGTH;

	static const float PLAYER_LENGTH;		// ���삷���Ƃ��Ă���z�̋���

	static const float PLAYER_ARROW_SIZE;
	static const float PLAYER_JACK_SIZE;
	static const float PLAYER_STORN_SIZE;

	static const double WALK_SE_INTERVAL_TIME;		// ����SE�̍Đ��Ԋu(�b��)

protected:

	int		m_PrevStatus;

	int		m_nNo;		// ���ʔԍ�
	int		m_nGrane;	// ��̎��
	int		m_nType;	// �v���C���[�̎�ށi���삷�������̑����j

	int		m_nPrevRL;
	int		m_nRL;		// �����Ă����

	float	m_fSpeed;		// �ړ����x(�����_��)
	float	m_fJumpSpeed;	// �W�����v�̑��x

	bool	m_bDelete;
	bool	m_bCol;			//�������Ă邩�ǂ���
	bool	m_bChangeGrane; // 

	// �A�j���[�V�����֌W
	bool m_bAnimeFall;

	CCharacter*		m_pTactile;			// �G�o
	CCharacter*		m_pTactileTable[MAX_GRANE];	// �G�o���X�g
	LPCTSTR			m_lpShockTex;		// Player�̃e�N�X�`��

	CStage*	m_pStage;	// �����蔻����s���t�B�[���h

	CPlayer*	m_pPlayer;	// ����ݒ��Player(����ݒ�̏ꍇ��NULL)

	D3DXVECTOR3 m_vFlower;	// �Ԃ̍炭����

	int		m_clearFrame;	// �N���A���̃A�j���[�V�����t���[���ԍ�
	bool	m_bClearJump;	// �N���A���̃W�����v�t���O

	// ���Ԋ֌W
	double m_lastTime;
	double m_nowTime;
	double m_walkTimer;

	// �����p
	int		m_nThrowNo;

public:
	CPlayer();
	virtual void Init();						// ������
	virtual void Init(const D3DXVECTOR3& pos);	// �T�C�Y���w�肵�ď�����
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void moveControllerPlayer();					// �����i�v���C���[	����N���X�ɂ������������̂���
	virtual void moveControllerOther();						// �����i�W�c�j		����N���X�ɂ������������̂���
	virtual void moveControllerThrowReady();
	virtual void moveControllerThrowReadyReady();
	virtual void moveControllerThrow();
	virtual void SoundEffect();
	virtual void Animation();								// �A�j��			����N���X�ɂ������������̂���
	static CPlayer* Create(const LPCTSTR pszFName);	// ����

	// ----- �Z�b�^�[
	void SetNo(int no){ m_nNo = no; }					// �v���C���[�̎��ʔԍ�
	void SetGrane(int grane){ m_nGrane = grane; m_bChangeGrane = false; m_pTactile = m_pTactileTable[grane]; }
	void SetType(int type){ m_nType = type; }	// �v���C���[�̑���ݒ�
	void SetRL(int rl){m_nRL = rl;}
	void SetThrowNo(int no){ m_nThrowNo = no; }
	void SetPlayer(CPlayer* p){ m_pPlayer = p; }		// ���삷��Player���ݒ�
	void SetStage(CStage* f){ m_pStage = f; }			// �t�B�[���h���ݒ�
	void SetLastTime(){ m_lastTime = CTimer::GetTime(); }

	void SetPrevStatus(int no){ m_status = no; }

	void EnableDelete(){ m_bDelete = true; }
	void DisableDelete(){ m_bDelete = false; }

	void EnableCol(){ m_bCol = true; }
	void DisableCol(){ m_bCol = false; }

	void TactileTranslateZ(float z) {m_pTactile->TranslateZ(z);}
	void TactileTranslationZ(float z) {m_pTactile->TranslationZ(z);}

	void ClearDirection();

	// ----- �Q�b�^�[
	int GetStatus(){ return m_status; }
	int GetNo(){ return m_nNo; }						// �v���C���[�̎��ʔԍ�
	int GetGrane(){return m_nGrane;}
	int GetRL(){return m_nRL;}
	int GetType(){ return m_nType; }					// ��ގ擾
	bool GetDelete(){ return m_bDelete; }
	bool GetCol(){ return m_bCol; }

private:
	bool Initialize(const LPCTSTR pszFName);	// ������
	void Finalize();		// ��n��
};
//========================================================================================
//	End of File
//========================================================================================