//========================================================================================
//		File        : Title.cpp
//		Program     : �^�C�g���N���X
//
//		Description : �^�C�g���N���X�̒�`
//
//		History     : 2013/12/18	�쐬�J�n
//
//																Author : Kei Hashimoto
//========================================================================================


//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �C���N���[�h
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
#include "../../h/System/PreInclude.h"
#
#include <tchar.h>
#include "../../h/System/System.h"
#include "../../h/System/Input.h"
#include "../../h/System/ChangeScene.h"
#include "../../h/Scene/GameMain.h"
#include "../../h/Scene/Title.h"

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// using�錾
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
using namespace Input;
using namespace System;

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �萔��`
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// ----- ���\�[�X���
const LPCTSTR CTitle::TEX_FILENAME[MAX_TEXLIST] =			// �e�N�X�`���t�@�C����
{
	_T("res/img/Title/saisyo_haikei.png"),					// �w�i�e�N�X�`���t�@�C����
	_T("res/img/Title/saisyo_gamen_nasi.png"),				// �w�i2�e�N�X�`���t�@�C����
	_T("res/img/Title/PressStart.png"),							// �X�^�[�g�A�C�R��
	_T("res/img/Title/rogo_dai.png"),						// �^�C�g�����S
};
const D3DXVECTOR3 CTitle::INIT_CAMERA_EYE(0, 0, -1000);		// �J�����̏������_
const D3DXVECTOR3 CTitle::INIT_CAMERA_LOOK(0, 0, 0);		// �J�����̏��������_
const D3DXVECTOR3 CTitle::INIT_TEXTURE_POS[MAX_TEXLIST] = {	// �e�N�X�`���̏����ʒu
	D3DXVECTOR3(0.0f, 0.0f, FAR_CLIP),						// �w�i
	D3DXVECTOR3(420.0f, 200.0f, FAR_CLIP),					// �w�i2
	D3DXVECTOR3(440.0f, 350.0f, 255),						// �X�^�[�g�A�C�R���摜
	D3DXVECTOR3(150.0f, 50.0f, 255),						// �^�C�g�����S
};

// ----- �t�F�[�h�֘A
const float CTitle::FADE_POSZ = -100.0f;	// �t�F�[�h�p�e�N�X�`����Z���W
const int CTitle::FADEIN_TIME = 10;		// �t�F�[�h�C���Ԋu(�A���t�@�l:0�`255)
const int CTitle::FADEOUT_TIME = 10;		// �t�F�[�h�A�E�g�Ԋu(�A���t�@�l:0�`255)


//========================================================================================
// public:
//========================================================================================

//����������������������������������������������������������������������������������������
//	Name        : �R���X�g���N�^
//	Arguments   : None.
//����������������������������������������������������������������������������������������
CTitle::CTitle()
{
	m_pCamera = NULL;
	m_pBG = NULL;
	m_pBG2 = NULL;
	m_pStart = NULL;
	m_pTitle = NULL;

	m_phase = MAX_PHASE;
}

//����������������������������������������������������������������������������������������
//	Name        : �f�X�g���N�^
//	Arguments   : None.
//����������������������������������������������������������������������������������������
CTitle::~CTitle()
{
}

//����������������������������������������������������������������������������������������
//	Name        : ������
//	Description : �^�C�g��������������
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CTitle::Init(void)
{
	// ----- �J����������
	m_pCamera->Init();
	D3DXVECTOR3 eye = INIT_CAMERA_EYE;
	D3DXVECTOR3 look = INIT_CAMERA_LOOK;
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	m_pCamera->SetParameter(eye, look, up);

	// ----- �e�N�X�`��������
	m_pBG->Init(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT), D3DXVECTOR3(568.0f, 320.0f, 1.0f));		// �w�i
	m_pBG2->Init(D3DXVECTOR2((float)1100, (float)1100), D3DXVECTOR3(568.0f, 320.0f, 0.5f));						// �w�i2
	m_pStart->Init(D3DXVECTOR2((float)250, (float)100), D3DXVECTOR3(568.0f, 550.0f, 0.0f));						// �X�^�[�g�A�C�R���摜
	m_pTitle->Init(D3DXVECTOR2((float)600, (float)400), D3DXVECTOR3(550.0f, -200.0f, 0.0f));					// �^�C�g���摜

	// ----- �t�F�[�h�ݒ�
	CChangeScene::SetNormalFadeAlpha(255);

	// ----- BGM�Đ�
	CGameMain::PlayBGM(BGM_TITLE, DSBPLAY_LOOPING);

	// ----- ���̃t�F�[�Y��
	m_phase = PHASE_FADEIN;		// �t�F�[�h�C���J�n
}

//����������������������������������������������������������������������������������������
//	Name        : ��n��
//	Description : �^�C�g���̏I������������
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CTitle::Uninit(void)
{
	// ----- �t�F�[�h�A�E�g�J�n
	if (m_phase != PHASE_FADEOUT)
		m_phase = PHASE_FADEOUT;

	// ----- �e�N�X�`����n��
	m_pBG->Uninit();		// �w�i
	m_pBG2->Uninit();		// �w�i2
	m_pStart->Uninit();		// �X�^�[�g
	m_pTitle->Uninit();		// �^�C�g��

	// ----- BGM��~
	CGameMain::StopBGM(BGM_TITLE);
}

//����������������������������������������������������������������������������������������
//	Name        : �X�V
//	Description : �^�C�g�����X�V����
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CTitle::Update(void)
{
	// ----- �I�u�W�F�N�g�X�V
	m_pCamera->Update();		// �J����

	switch (m_phase)
	{
		// �t�F�[�h�C��
	case PHASE_FADEIN:
		if (CChangeScene::NormalFadeOut(FADE_POSZ, FADEIN_TIME)) {
			m_phase = PHASE_MAIN;							// �^�C�g���{�ҊJ�n
		}
		break;

		// �t�F�[�h�A�E�g
	case PHASE_FADEOUT:
		if (CChangeScene::NormalFadeIn(FADE_POSZ, FADEOUT_TIME))
		{	// ���̃V�[����
			Uninit();							// ��n��
			CGameMain::SetScene(SID_SELECT);	// �y�ȑI����ʂ�
		}
		break;

		// �^�C�g���{��
	case PHASE_MAIN:
		Main();
		break;

	default:
		break;
	}
}

//����������������������������������������������������������������������������������������
//	Name        : �`��
//	Description : �^�C�g����ʂ�`�悷��
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CTitle::Draw(void)
{
	// ----- �J�����`��
	m_pCamera->Draw();

	// ----- �e�N�X�`���`��
	m_pBG->DrawScreenAlpha();			// �w�i

	m_pBG2->DrawScreenAlpha();		// �w�i2
	m_pBG2->RotationZ(-0.05f);

	m_pStart->DrawScreenAlpha();		// �X�^�[�g

	m_pTitle->DrawScreenAlpha();		// �^�C�g��

	switch (m_phase)
	{
		// �t�F�[�h�C���E�A�E�g
	case PHASE_FADEIN:
	case PHASE_FADEOUT:
		CChangeScene::DrawNormalFade();
		break;

		// �^�C�g���{��
	case PHASE_MAIN:
		break;

	default:
		break;
	}
}

//����������������������������������������������������������������������������������������
//	Name        : ��������
//	Description : �^�C�g���f�[�^�𐶐�����
//	Arguments   : None.
//	Returns     : �^�C�g���f�[�^
//����������������������������������������������������������������������������������������
CTitle* CTitle::Create()
{
	// ----- �ϐ��錾
	CTitle* pTitle;

	// ----- ����������
	pTitle = new CTitle();
	if (pTitle)
	{
		if (!pTitle->Initialize())
		{
			SAFE_DELETE(pTitle);
		}
	}

	return pTitle;
}

//========================================================================================
// private:
//========================================================================================

//����������������������������������������������������������������������������������������
//	Name        : ������
//	Description : �^�C�g��������������
//	Arguments   : None.
//	Returns     : ����(true:����)
//����������������������������������������������������������������������������������������
bool CTitle::Initialize()
{
	// �J��������
	m_pCamera = CCamera::Create();
	if (m_pCamera == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CTitle::Camera�̐����Ɏ��s���܂����B"), _T("error"), MB_OK);
#endif
		return false;
	}

	// ----- �e�N�X�`������
	// �w�i
	m_pBG = CObject2D::Create(TEX_FILENAME[TL_BG]);
	if (m_pBG == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CTitle::BG�̐����Ɏ��s���܂����B"), _T("error"), MB_OK);
#endif
		return false;
	}

	//�w�i2
	m_pBG2 = CCharacter::Create(TEX_FILENAME[TL_BG2]);
	if (m_pBG2 == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CTitle::BG2�̐����Ɏ��s���܂����B"), _T("error"), MB_OK);
#endif
		return false;
	}

	// �X�^�[�g
	m_pStart = CCharacter::Create(TEX_FILENAME[TL_START]);
	if (m_pStart == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CTitle::Start�̐����Ɏ��s���܂����B"), _T("error"), MB_OK);
#endif
		return false;
	}
	// �^�C�g��
	m_pTitle = CCharacter::Create(TEX_FILENAME[TL_TITLE]);
	if (m_pTitle == NULL) {
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("CTitle::Title�̐����Ɏ��s���܂����B"), _T("error"), MB_OK);
#endif
		return false;
	}

	return true;
}

//����������������������������������������������������������������������������������������
//	Name        : ��n��
//	Description : �^�C�g���̏I������������
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CTitle::Finalize(void)
{
	// ----- �e�N�X�`�����
	SAFE_RELEASE(m_pBG);		// �w�i
	SAFE_RELEASE(m_pBG2);		// �w�i2
	SAFE_RELEASE(m_pStart);		// �X�^�[�g
	SAFE_RELEASE(m_pTitle);		// �^�C�g��


	// ----- �J�������
	SAFE_RELEASE(m_pCamera);
}

//����������������������������������������������������������������������������������������
//	Name        : ���C��
//	Description : �^�C�g���̃��C������
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CTitle::Main()
{
	// ----- ���̃V�[����
	if (GetTrgKey(DIK_RETURN)) {
		CGameMain::PlaySE(SE_ENTER);
		m_phase = PHASE_FADEOUT;		// �t�F�[�h�A�E�g�J�n
	}

	static bool DownUp = false;
	D3DXVECTOR2 Move(m_pTitle->GetPosX(), m_pTitle->GetPosY());
	//for (DownUp = 0; DownUp < 1; DownUp++)
	static float stop = 0.0f;
	static float rise = 5.0f;
	static float bottom = -4.0f;
	if (!DownUp)
		{
			if (stop == 1){
				m_pTitle->TranslationY(4.0f);
				
			}
			else{
				m_pTitle->TranslationY(bottom);
				bottom ++;
			}
				if (Move.y > 270.0f)
				{
					DownUp = true;
					stop++;
				}
			
		}
		else{
			if (stop != 2)
			{
				m_pTitle->TranslationY(rise);
				rise --;
				if (Move.y < 190.0f)
				{
					DownUp = false;

				}
			}
		}

		static bool UpDown = false;
		D3DXVECTOR2 Pos(m_pStart->GetPosX(), m_pStart->GetPosY());
		if (!UpDown)
		{

			m_pStart->TranslationY(1.5f);
			if (Pos.y > 510.0f)
			{
				UpDown = true;

			}
		}
		else{
			m_pStart->TranslationY(-1.0f);
			if (Pos.y < 490.0f)
			{
				UpDown = false;

			}
		}
}


//========================================================================================
//	End of File
//========================================================================================