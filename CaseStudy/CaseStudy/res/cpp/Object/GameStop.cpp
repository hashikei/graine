//========================================================================================
//		File        : Button.h
//		Program     : �{�^���x�[�X
//
//		Description : �{�^���x�[�X�̒�`
//
//		History     : 2015/05/29	�쐬�J�n
//						   
//
//																Author : ���񂽂܃Y
//========================================================================================
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �C���N���[�h
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
#include "../../h/System/PreInclude.h"
#
#include <math.h>
#include "../../h/Scene/GameMain.h"
#include "../../h/Object/GameStop.h"
#include "../../h/System/Input.h"
#include "../../h/System/Timer.h"
#include <tchar.h>

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// using�錾
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
using namespace System;
using namespace Input;


//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �����o���̐錾
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// ----- �����o�萔
// private:
const LPCTSTR CGameStop::TEX_FILENAME[MAX_TEX] = {
	_T("res/img/GameScene/Popup/WindowL.png"),		// �E�B���h�E�e�N�X�`���t�@�C����
	_T("res/img/GameScene/Popup/Close.png"),		// �{�^���e�N�X�`���t�@�C����
	_T("res/img/GameScene/Popup/Reset.png"),		// �{�^���e�N�X�`���t�@�C����
	_T("res/img/GameScene/Popup/Select.png"),		// �{�^���e�N�X�`���t�@�C����
	_T("res/img/GameScene/Popup/Pause.png"),		// �e�L�X�g�e�N�X�`���t�@�C����
};

const D3DXVECTOR2 CGameStop::W_0_DEFAULET_SIZE		= D3DXVECTOR2(330,439);
const D3DXVECTOR3 CGameStop::W_0_DEFAULET_POS		= D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0);

const D3DXVECTOR2 CGameStop::B_0_DEFAULET_SIZE		= D3DXVECTOR2(200,73);
const D3DXVECTOR3 CGameStop::B_0_DEFAULET_POS		=  D3DXVECTOR3(SCREEN_WIDTH / 2 - 5.0f,
														SCREEN_HEIGHT / 2 + 45.0f,0);

const D3DXVECTOR2 CGameStop::TEXT_SIZE(399.0f * 0.7f, 109.0f * 0.7f);
const D3DXVECTOR3 CGameStop::TEXT_POS((float)SCREEN_WIDTH * 0.5f + 5.0f, 195.0f, 0.0f);

const float CGameStop::B_0_POS_INTERVAL_Y = 80;

const double CGameStop::SELECT_ANIME_TIME = 0.5;
const D3DXVECTOR3 CGameStop::SELECT_BUTTON_SCALE_L = D3DXVECTOR3(1.1f, 1.1f, 1.0f);
const D3DXVECTOR3 CGameStop::SELECT_BUTTON_SCALE_S = D3DXVECTOR3(0.95f, 0.95f, 1.0f);


//========================================================================================
// public:
//========================================================================================
//����������������������������������������������������������������������������������������
//	Name        : �R���X�g���N�^
//	Description : �R���X�g���N�^
//	Arguments   : 
//	Returns     : 
//����������������������������������������������������������������������������������������
CGameStop::CGameStop()
{
	m_nPhase	= 0;
	m_nCurrent	= 0;
	m_nGo		= 0;

	m_pWnd				= NULL;
	m_pText				= NULL;

	m_pButtonGoGame		= NULL;
	m_pButtonReset		= NULL;
	m_pButtonGoSelect	= NULL;

	m_selectAnimeTimer	= 0.0;
}

//����������������������������������������������������������������������������������������
//	Name        : ����
//	Description : �I�u�W�F�N�g�𐶐�����
//	Arguments   : pszFName / �ǂݍ��݃t�@�C����
//	Returns     : �I�u�W�F�N�g�f�[�^
//����������������������������������������������������������������������������������������
CGameStop* CGameStop::Create()
{
	// ----- �ϐ��錾
	CGameStop* pObj;

	// ----- ����������
	pObj = new CGameStop();
	
	return pObj;
}
//����������������������������������������������������������������������������������������
//	Name        : ��ԍŏ�������
//	Description : 
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CGameStop::Initialize()
{
		// �E�B���h�E�쐬
	m_pWnd				= CObject2D::Create(TEX_FILENAME[TEX_WND_0]);
	m_pWnd->Init(W_0_DEFAULET_SIZE,D3DXVECTOR3(W_0_DEFAULET_POS.x,W_0_DEFAULET_POS.y,0));
	
	m_pText				= CObject2D::Create(TEX_FILENAME[TEX_TEXT]);
	m_pText->Init(TEXT_SIZE,TEXT_POS);
	
	// �{�^���쐬
	m_pButtonGoGame		= CButton::Create(TEX_FILENAME[TEX_CLOSE]);
	m_pButtonReset		= CButton::Create(TEX_FILENAME[TEX_RESET]);
	m_pButtonGoSelect	= CButton::Create(TEX_FILENAME[TEX_SELECT]);

	m_pButtonGoGame->Init(B_0_DEFAULET_SIZE,D3DXVECTOR3(B_0_DEFAULET_POS.x,B_0_DEFAULET_POS.y - B_0_POS_INTERVAL_Y,0));
	m_pButtonReset->Init(B_0_DEFAULET_SIZE,D3DXVECTOR3(B_0_DEFAULET_POS.x,B_0_DEFAULET_POS.y,0));
	m_pButtonGoSelect->Init(B_0_DEFAULET_SIZE,D3DXVECTOR3(B_0_DEFAULET_POS.x,B_0_DEFAULET_POS.y + B_0_POS_INTERVAL_Y,0));
	
	m_pButtonGoGame->SetName("GoGame");
	m_pButtonReset->SetName("Reset");
	m_pButtonGoSelect->SetName("Select");
		
	m_vecButton.push_back(m_pButtonGoGame);
	m_vecButton.push_back(m_pButtonReset);
	m_vecButton.push_back(m_pButtonGoSelect);
}
//����������������������������������������������������������������������������������������
//	Name        : �������
//	Description : 
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CGameStop::Release()
{
	if(m_pWnd){
		m_pWnd->Uninit();
		SAFE_RELEASE(m_pWnd)
	}

	// ���X�g���S����n��
	for (unsigned int i = 0;i < m_vecButton.size(); ++i){
		m_vecButton[i]->Uninit();
		SAFE_RELEASE(m_vecButton[i])
	}

	m_vecButton.clear();		// �I�u�W�F�N�g���X�g
}
//����������������������������������������������������������������������������������������
//	Name        : ������
//	Description : �����ȏ�����
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CGameStop::Init()
{
	m_nPhase = GAME_STOP_PHASE_INIT;
	m_nCurrent = GOGAME_BUTTON;
	m_nGo		= GO_GAME;

	// �ŏ��̑I���́u�Q�[���ɖ߂�v
	m_vecButton[GOGAME_BUTTON]->SetPhase(B_PHASE_CHOICE);

	m_selectAnimeTimer = 0.0;
}
//����������������������������������������������������������������������������������������
//	Name        : ��n��
//	Description : 
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CGameStop::Uninit()
{
}
//����������������������������������������������������������������������������������������
//	Name        : �X�V
//	Description : �����ȍX�V
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CGameStop::Update()
{
	switch(m_nPhase)
	{
	case GAME_STOP_PHASE_INIT:
		// ----- BGM�Đ�
		CGameMain::PlayBGM(BGM_PAUSE, DSBPLAY_LOOPING);
		m_nPhase = GAME_STOP_PHASE_WAIT;
		break;
	case GAME_STOP_PHASE_WAIT:
		Wait();
		break;
	case GAME_STOP_PHASE_ENTER:
		Enter();
		break;
	case GAME_STOP_PHASE_UNINIT:
		m_nPhase++;
	case GAME_STOP_PHASE_END:
		break;
	}
}
//����������������������������������������������������������������������������������������
//	Name        : �`��
//	Description : �����ȕ`��
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CGameStop::Draw()
{
	m_pWnd->DrawScreenAlpha();
	m_pText->DrawScreenAlpha();

	for(unsigned int i = 0;i < m_vecButton.size();i++){
		m_vecButton[i]->DrawScreenAlpha();
	}
}
//����������������������������������������������������������������������������������������
//	Name        : ����҂�
//	Description : 
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CGameStop::Wait()
{
	// �I���@���E�L�[
	if(GetTrgKey(DIK_DOWN)){
		CGameMain::PlaySE(SE_CHOICE);
		if(m_nCurrent < MAX_BUTTON - 1) {
			m_nCurrent++;
			m_selectAnimeTimer = 0.0;
		}
	}
	if(GetTrgKey(DIK_UP)){
		CGameMain::PlaySE(SE_CHOICE);
		if(m_nCurrent > 0) {
			m_nCurrent--;
			m_selectAnimeTimer = 0.0;
		}
	}

	// ���ݑI������Ă�{�^��
	for(unsigned int i = 0;i < m_vecButton.size();i++){

		if(i == m_nCurrent)
			m_vecButton[i]->SetPhase(B_PHASE_CHOICE);
		else
			m_vecButton[i]->SetPhase(B_PHASE_WAIT);

		if(m_vecButton[i]->GetPhase() == B_PHASE_CHOICE){
//			m_vecButton[i]->SetColor(D3DXVECTOR3(255,255,0));
			if(CTimer::GetTime() - m_selectAnimeTimer > SELECT_ANIME_TIME) {
				m_selectAnimeTimer = CTimer::GetTime();

				if(m_vecButton[i]->GetScale().x >= SELECT_BUTTON_SCALE_L.x) {
					m_vecButton[i]->Scale(SELECT_BUTTON_SCALE_S);
				} else {
					m_vecButton[i]->Scale(SELECT_BUTTON_SCALE_L);
				}
			}
		}else{
//			m_vecButton[i]->SetColor(D3DXVECTOR3(255,255,255));
			m_vecButton[i]->Scale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		}
	}

	// ����
	if(GetTrgKey(DIK_RETURN)){
		CGameMain::PlaySE(SE_ENTER);
		m_vecButton[m_nCurrent]->SetPhase(B_PHASE_ENTER);
		m_nPhase = GAME_STOP_PHASE_ENTER;
	}

	if(GetTrgKey(DIK_RSHIFT)){
		CGameMain::PlaySE(SE_ENTER);
		m_vecButton[0]->SetPhase(B_PHASE_ENTER);
		m_nPhase = GAME_STOP_PHASE_ENTER;
	}
}
//����������������������������������������������������������������������������������������
//	Name        : ����
//	Description : 
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CGameStop::Enter()
{
	for(unsigned int i = 0;i < m_vecButton.size();i++){
		if(m_vecButton[i]->GetPhase() == B_PHASE_ENTER){
			switch(i)
			{
			case GOGAME_BUTTON:
				m_nGo = GO_GAME;
				CGameMain::PlayBGM(BGM_GAME, DSBPLAY_LOOPING);
				break;
			case RESET_BUTTON:
				m_nGo = GO_RESET;
				break;
			case GOSELECT_BUTTON:
				m_nGo = GO_SELECT;
				break;
			}
		}
	}
	
	CGameMain::StopBGM(BGM_PAUSE);
	
	m_nPhase++;
}
//========================================================================================
//	End of File
//========================================================================================