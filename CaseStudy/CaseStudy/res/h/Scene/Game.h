//========================================================================================
//		File        : Game.h
//		Program     : �Q�[���{�҃N���X
//
//		Description : �Q�[���{�҃N���X�̒�`
//
//		History     : 2013/12/18	�쐬�J�n
//
//																Author : Kei Hashimoto
//========================================================================================

#pragma once

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �C���N���[�h
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
#include "../System/PreInclude.h"
#
#include "../System/Graphics.h"
#include "../Scene/Scene.h"
#include "../Object/Object2D.h"
#include "../Object/GameCamera.h"

#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Object/Flower.h"
#include "../Object/Jack.h"
#include "../Object/Stone.h"
#include "../Object/PlayersGroup.h"
#include "../Object/GameStop.h"
#include "../Object/GameOver.h"
#include "../Object/GameClear.h"


//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �萔��`
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
const int MAX_OBJECT = 50;		//�I�u�W�F�N�g��(��)

// ----- �t�F�[�Y�t���O
static enum _ePhase
{
	PHASE_FADEIN = 0,		// �t�F�[�h�C��
	PHASE_FADEOUT,			// ���̃V�[���փt�F�[�h�A�E�g
	PHASE_START,
	PHASE_MAIN,				// �Q�[���{��
	PHASE_STOP,				// �ꎞ��~
	PHASE_OVER,				// �I�[�o
	PHASE_CLEAR,			// �N���A
	PHASE_STOPFADEIN,
	PHASE_STOPFADEOUT,
	PHASE_OVERFADEIN,
	PHASE_OVERFADEOUT,
	PHASE_CLEARFADEIN,
	PHASE_CLEARFADEOUT,
	PHASE_NOWLOADING,
	PHASE_LOADFADEIN,
	PHASE_LOADFADEOUT,

	MAX_PHASE
};

enum 
{
	S_PHASE_INIT = 0,
	S_PHASE_START,
	S_PHASE_FLOWER,
	S_PHASE_STAGE,
	S_PHASE_GOMAIN
};

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �N���X��`
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
class CGame : public CScene
{
// ===== �����o�\����
private:
	// �N���b�s���O���
	struct TCLIPINFO {
		int			type;	// �Ԃ��ӂ�
		D3DXVECTOR3	pos;	// �ʒu
		D3DXVECTOR2 size;	// �T�C�Y
	};

// ===== ���[�U�^��`
private:
	typedef std::vector<TCLIPINFO>		CLIPINFO_ARRAY;
	typedef CLIPINFO_ARRAY::iterator	CLIPINFO_ARRAY_IT;

// ===== �����o�萔
public:
	// ----- �I�u�W�F�N�g���X�g
	static enum _eObjectList
	{
		OL_LB_DARK = 0,
		OL_PLAYER_DARK,
		OL_TACTILE_DARK,
		OL_FLOWER_DARK,
		OL_STONE_DARK,
		OL_JACK_DARK,
		OL_SCROLL_DARK,
		OL_BG_DARK,
		OL_LAYOUT_OBJECT,
		OL_LB_LIGHT,
		OL_PLAYER_LIGHT,
		OL_TACTILE_LIGHT,
		OL_FLOWER_LIGHT,
		OL_STONE_LIGHT,
		OL_JACK_LIGHT,
		OL_SCROLL_LIGHT,
		OL_BG_LIGHT,

		MAX_OBJECTLIST
	};

	static const float	OBJ_PRIORITIES[];	// �I�u�W�F�N�g�̃v���C�I���e�B���X�g

private:
	static const LPCTSTR TEX_FILENAME[];			// �e�N�X�`���̃t�@�C����
	static const D3DXVECTOR3 INIT_TEXTURE_POS[];	// �e�N�X�`���̏����ʒu

	static const D3DXVECTOR2 BG_SIZE;		// �w�i�T�C�Y

	static const D3DXVECTOR2 FILTER_SIZE;
	static const D3DXVECTOR3 FILTER_POS;

	static const float FADE_POSZ;				// �t�F�[�h�p�e�N�X�`����Z���W
	static const int FADEIN_TIME;				// �t�F�[�h�C���Ԋu(�A���t�@�l:0�`255)
	static const int FADEOUT_TIME;				// �t�F�[�h�A�E�g�Ԋu(�A���t�@�l:0�`255)
	static const int STOP_FADEIN_TIME;			// �t�F�[�h�C���Ԋu(�A���t�@�l:0�`255)
	static const int STOP_FADEOUT_TIME;			// �t�F�[�h�A�E�g�Ԋu(�A���t�@�l:0�`255)
	static const int NOWLOADING_FADEIN_TIME;	// �t�F�[�h�C���Ԋu(�A���t�@�l:0�`255)
	static const int NOWLOADING_FADEOUT_TIME;	// �t�F�[�h�A�E�g�Ԋu(�A���t�@�l:0�`255)

	static const D3DXVECTOR3	CLIP_INITPOS;			// �N���b�s���O�����ʒu
	static const D3DXVECTOR2	CLIP_SIZE;				// �N���b�s���O�T�C�Y
	static const float			CLIP_SCALING_SPD;		// �N���b�s���O�g�呬�x
	static const float			CLIP_LATEST_SPD;		// �N���b�s���O�Œx���x
	static const D3DXVECTOR2	CLIP_SIZE_JACK;			// �ӎ��̃N���b�s���O�T�C�Y
	static const float			CLIP_SCALING_SPD_JACK;	// �ӎ��̃N���b�s���O�g�呬�x
	static const float			CLIP_LATEST_SPD_JACK;	// �ӎ��̃N���b�s���O�Œx���x
	
	static const float	SCROLL_EFFECT_SPD;		// �X�N���[���G�t�F�N�g�ړ����x

	static const int	WND_FILTER_ALPHA;

	static const D3DXVECTOR3 DIRECTION_PLAYER_POS;
	static const D3DXVECTOR2 NOWLOADING_TEXT_SIZE;
	static const D3DXVECTOR3 NOWLOADING_TEXT_POS;
	static const float NOWLOADING_TEXT_INTERVAL;
	static const float NOWLOADING_TEXT_AMOUNT;
	static const float NOWLOADING_TEXT_SPD;

	// ----- �e�N�X�`�����X�g
	static enum _eTexList
	{
		TL_BG_DARK = 0,		// �w�i�e�N�X�`��
		TL_BG_LIGHT,		// �w�i�e�N�X�`��
		TL_PLAYER_0,		// �v���C���[�e�N�X�`���i�{�́j
		TL_BLOCK_0,			// �u���b�N�e�N�X�`��
		TL_FLOWER_0,
		TL_FLOWER_1,
		TL_JACK_0,
		TL_STONE_0,
		TL_CLIP,
		TL_SCROLL_EFFECT,
		TL_FILTER,
		TL_TEXT_N_LARGE,
		TL_TEXT_O,
		TL_TEXT_W,
		TL_TEXT_L,
		TL_TEXT_A,
		TL_TEXT_D,
		TL_TEXT_I,
		TL_TEXT_N_SMALL,
		TL_TEXT_G,

		MAX_TEXLIST
	};

	// ----- �N���b�s���O�^�C�v
	static enum _eClipType
	{
		CT_FLOWER = 0,
		CT_JACK,

		MAX_CLIPTYPE
	};

// ===== �����o�ϐ�
private:
	// ----- �I�u�W�F�N�g
	static CGameCamera*	m_pCamera;		// �J����
	static CObject2D*	m_pDarkBG;		// �w�i
	static CObject2D*	m_pLightBG;		// �w�i
	static CObject2D*	m_pFilter;		// �t�B���^�[

	// ----- �v���C���[�@----- //
	static CPlayersGroup*	m_pPlayersGroup;

	static std::vector<CFlower*> m_listFlower;

	static CStage*		m_pStage;
	static int			m_stageID;		// �I�������X�e�[�W��ID

	static CCharacter*	m_pScrollEffectDark;	// �w�i�ŃX�N���[������G�t�F�N�g
	static CCharacter*	m_pScrollEffectLight;	// �w�i�ŃX�N���[������G�t�F�N�g

	static CGameStop*	m_pGameStop;
	static CGameOver*	m_pGameOver;
	static CGameClear*	m_pGameClear;

	static CCharacter*				m_pClipCircle;
	static CLIPINFO_ARRAY			m_clipInfoList;
	static std::vector<float>		m_clipEasingList;
	static std::vector<D3DXVECTOR2>	m_clearClipSizeList;

	// ----- �Q�[���V�X�e��
	static DWORD	m_phase;		// �t�F�[�Y�t���O
	static DWORD	m_pNextScene;

	static HANDLE			m_hNowLoading;		// Now Loading�p�n���h��
	static CRITICAL_SECTION	m_cs;				// �N���e�B�J���Z�N�V����
	static bool				m_bLoaded;			// ���\�[�X�̃��[�h�����t���O
	static CCharacter*		m_pDirPlayer;		// ���o�p���˂ۂ�
	static CCharacter*		m_pDirTactile;		// ���o�p���˂ۂ�̐G�o
	static LPCHARACTER_ARRAY	m_pLoadingTextes;	// Now Loading�̃e�L�X�g

	int	m_nStart;

// ===== �����o�֐�
public:
	CGame();
	virtual ~CGame();

	void	Init(void);			// ������
	void	Uninit(void);		// ��n��
	void	Update(void);		// �X�V
	void	Draw(void);			// �`��
	static CGame* Create();		// ����

	void CreateFlower(D3DXVECTOR3 pos,D3DXVECTOR3 dir);
	CCharacter* CreateJack(D3DXVECTOR3 pos,D3DXVECTOR3 dir);
	CCharacter* CreateStone(D3DXVECTOR3 pos,D3DXVECTOR3 dir);

	static void SetStageID(int id) {	// �X�e�[�WID�ݒ�
		id >= 0 && id < CMapData::MAX_STAGEID ?
			m_stageID = id : m_stageID = 0;
	}

	static unsigned int WINAPI NowLoading(void* arg);	// Now Loading

private:
	bool	Initialize();		// ������
	void	Finalize(void);		// ��n��

	void	Main();		// ���C��
	void	DrawMain();

	void	Start();	// �J�n
	void	DrawStart();

	void	Stop();		// �ꎞ��~
	void	DrawStop();

	void	Over();		// �I�[�o
	void	DrawOver();

	void	Clear();		// �N���A
	void	DrawClear();
};


//========================================================================================
//	End of File
//========================================================================================