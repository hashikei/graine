//========================================================================================
//		File        : Input.cpp
//		Program     : ���̓f�o�C�X�N���X
//
//		Description : ���̓f�o�C�X�N���X�̎���
//
//		History     : 2013/07/02	�쐬�J�n
//						   10/24	�W���C�p�b�h�֘A���R�����g�A�E�g���Ė�����
//					  2014/08/07	�V���O���g���N���X��
//
//																Author : Kei Hashimoto
//========================================================================================


//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �C���N���[�h
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
#include "../../h/System/PreInclude.h"
#
#include <windows.h>
#include "../../h/System/Input.h"

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �}�N����`
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)			if(x){ x->Release(); x=NULL; }
#endif

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �O���[�o���ϐ��錾
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
static CInput& g_input = CInput::GetInstance();		// ���̓f�o�C�X�N���X�̎��̐���

static BYTE	g_prsKey[256];		// �v���X�L�[���z��
static BYTE	g_trgKey[256];		// �g���K�L�[���z��
static BYTE	g_rlsKey[256];		// �����[�X�L�[���z��
static BYTE	g_prsClick[8];		// �v���X�N���b�N���z��
static BYTE	g_trgClick[8];		// �g���K�N���b�N���z��
static BYTE	g_rlsClick[8];		// �����[�X�N���b�N���z��

static POINT g_curPos;			// �J�[�\�����W


//========================================================================================
// public:
//========================================================================================

//����������������������������������������������������������������������������������������
//	Name        : �C���X�^���X�擾
//	Description : ���̓f�o�C�X�N���X�̃C���X�^���X���擾����
//	Arguments   : None.
//	Returns     : ���̓f�o�C�X�N���X
//����������������������������������������������������������������������������������������
CInput& CInput::GetInstance()
{
	// ----- �C���X�^���X����
	static CInput input;

	return input;
}

//����������������������������������������������������������������������������������������
//	Name        : ���͏����̏�����
//	Description : ���͏����S�ʂ�����������
//	Arguments   : hInst / �C���X�^���X
//				  hWnd  / �E�B���h�E�n���h��
//	Returns     : ����
//����������������������������������������������������������������������������������������
HRESULT CInput::InitInput(HINSTANCE hInst, HWND hWnd)
{
	// ----- DirectInput�I�u�W�F�N�g�̍쐬
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION,
				IID_IDirectInput8, (void **)&m_pDInput, NULL)))
		return E_FAIL;

	// ----- �L�[�{�[�h�̏�����
	if(FAILED(InitKeyboard(hWnd)))
		return E_FAIL;
	
	// ----- �}�E�X�̏�����
	if(FAILED(InitMouse(hWnd)))
		return E_FAIL;
	
	// ----- �Q�[���p�b�h�̏�����
	//if(FAILED(InitJoypad(hWnd)))
	//	return E_FAIL;

	return S_OK;
}

//����������������������������������������������������������������������������������������
//	Name        : ���͏����̏I������
//	Description : ���͏����S�ʂ̌�n�������A�������I������
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CInput::Release(void)
{
	// ----- �L�[�{�[�h�̏I������
	UninitKeyboard();
	
	// ----- �}�E�X�̏I������
	UninitMouse();
	
	// ----- �W���C�p�b�h�̏I������
	//UninitJoypad();

	// ----- �I�u�W�F�N�g�̌�n��
	SAFE_RELEASE(m_pDInput);
}

//����������������������������������������������������������������������������������������
//	Name        : ���͏����̍X�V����
//	Description : ���͏����S�ʂ��X�V����
//	Arguments   : hWnd / �E�B���h�E�n���h��
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CInput::UpdateInput(HWND hWnd)
{
	// ----- �L�[�{�[�h�̍X�V
	UpdateKeyboard();
	
	// ----- �}�E�X�̍X�V
	UpdateMouse();
	
	// ----- �W���C�p�b�h�̍X�V
	//UpdateJoypad();

	// ----- �L�[���͂̍X�V
	UpdateInputKey();

	// ----- �}�E�X���͂̍X�V
	UpdateClick();

	// ----- �J�[�\���̍X�V
	UpdateCursor(hWnd);
}


//========================================================================================
// private:
//========================================================================================

//����������������������������������������������������������������������������������������
//	Name        : �R���X�g���N�^
//	Arguments   : None.
//����������������������������������������������������������������������������������������
CInput::CInput()
{
	m_pDInput			= NULL;
	m_pDIDevKeyboard	= NULL;
	m_pDIDevMouse		= NULL;
	m_numCnt			= 0;
}

//����������������������������������������������������������������������������������������
//	Name        : �f�X�g���N�^
//	Arguments   : None.
//����������������������������������������������������������������������������������������
CInput::~CInput()
{
}

//����������������������������������������������������������������������������������������
//	Name        : �L�[�{�[�h�̏�����
//	Description : �L�[�{�[�h�Ɋւ����������������
//	Arguments   : hWnd / �E�B���h�E�n���h��
//	Returns     : ����
//����������������������������������������������������������������������������������������
HRESULT CInput::InitKeyboard(HWND hWnd)
{
	// ----- �f�o�C�X�I�u�W�F�N�g���쐬
	if(FAILED(m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevKeyboard, NULL)) || m_pDIDevKeyboard == NULL)
		return E_FAIL;

	// ----- �f�[�^�t�H�[�}�b�g��ݒ�
	if(FAILED(m_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;

	// ----- �������[�h��ݒ�(�t�H�A�O���E���h����r�����[�h)
	if(FAILED(m_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		return E_FAIL;

	// ----- �L�[�{�[�h�̃A�N�Z�X�����l��(���͐���J�n)
	m_pDIDevKeyboard->Acquire();

	return S_OK;
}

//����������������������������������������������������������������������������������������
//	Name        : �L�[�{�[�h�̏I������
//	Description : �L�[�{�[�h�Ɋւ�����̌�n�������A�������I������
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CInput::UninitKeyboard(void)
{
	SAFE_RELEASE(m_pDIDevKeyboard);
}

//����������������������������������������������������������������������������������������
//	Name        : �L�[�{�[�h�̍X�V����
//	Description : �L�[�{�[�h�Ɋւ�����y�я������X�V����
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CInput::UpdateKeyboard(void)
{
	// ----- �f�o�C�X����f�[�^���擾�B���s������L�[�{�[�h�ւ̃A�N�Z�X�����擾
	if(FAILED(m_pDIDevKeyboard->GetDeviceState(sizeof(m_keyState), m_keyState)))
		m_pDIDevKeyboard->Acquire();
}

//����������������������������������������������������������������������������������������
//	Name        : �}�E�X�̏�����
//	Description : �}�E�X�Ɋւ����������������
//	Arguments   : hWnd / �E�B���h�E�n���h��
//	Returns     : ����
//����������������������������������������������������������������������������������������
HRESULT CInput::InitMouse(HWND hWnd)
{
	// ----- �f�o�C�X�I�u�W�F�N�g���쐬
	if(FAILED(m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIDevMouse, NULL)) || m_pDIDevMouse == NULL)
		return E_FAIL;

	// ----- �f�[�^�t�H�[�}�b�g��ݒ�
	if(FAILED(m_pDIDevMouse->SetDataFormat(&c_dfDIMouse2)))
		return E_FAIL;

	// ----- �������[�h��ݒ�(�t�H�A�O���E���h����r�����[�h)
	if(FAILED(m_pDIDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		return E_FAIL;

	// ----- �}�E�X�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDIDevMouse->Acquire();

	return S_OK;
}

//����������������������������������������������������������������������������������������
//	Name        : �}�E�X�̏I������
//	Description : �}�E�X�Ɋւ�����̌�n�������A�������I������
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CInput::UninitMouse(void)
{
	SAFE_RELEASE(m_pDIDevMouse);
}

//����������������������������������������������������������������������������������������
//	Name        : �}�E�X�̍X�V����
//	Description : �}�E�X�Ɋւ�����y�я������X�V����
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CInput::UpdateMouse(void)
{
	// ----- �f�o�C�X����f�[�^���擾�B���s������}�E�X�ւ̃A�N�Z�X�����擾
	if(FAILED(m_pDIDevMouse->GetDeviceState(sizeof(m_mouseState), &m_mouseState)))
		m_pDIDevMouse->Acquire();
}
/*
//����������������������������������������������������������������������������������������
//	Name        : �Q�[���p�b�h�̏�����
//	Description : �Q�[���p�b�h�Ɋւ����������������
//	Arguments   : hWnd / �E�B���h�E�n���h��
//	Returns     : ����
//����������������������������������������������������������������������������������������
HRESULT CInput::InitJoypad(HWND hWnd)
{
	// ----- �ϐ��錾
	int nLoop;

	// ----- ������
	for(nLoop = 0; nLoop < MAX_CONTROLER; nLoop ++)
		m_pDIDevJoypad[nLoop] = NULL;

	// ----- �f�o�C�X�I�u�W�F�N�g���쐬(�ڑ�����Ă���W���C�p�b�h��񋓂���)
	if(FAILED(m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, NULL, DIEDFL_ATTACHEDONLY)))
		return E_FAIL;

	// ----- �W���C�p�b�h�̐���������
	for(nLoop = 0; nLoop < MAX_CONTROLER; nLoop ++)
	{
		// ----- �W���C�p�b�h���Ȃ��ꍇ�͖�����
		if(m_pDIDevJoypad[nLoop] == NULL)
			continue;

		// ----- �f�[�^�t�H�[�}�b�g�̐ݒ�
		if(FAILED(m_pDIDevJoypad[nLoop]->SetDataFormat(&c_dfDIJoystick)))
			return E_FAIL;

		// ----- �������x���̐ݒ�
		if(FAILED(m_pDIDevJoypad[nLoop]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
			return E_FAIL;

		// ----- �f�o�C�X�ւ̓��͐���J�n
		m_pDIDevJoypad[nLoop]->Acquire();
	}

	return S_OK;
}

//����������������������������������������������������������������������������������������
//	Name        : �W���C�X�e�B�b�N�₢���킹�p�R�[���o�b�N�֐�
//	Description : 
//	Arguments   : 
//	Returns     : 
//����������������������������������������������������������������������������������������
BOOL CALLBACK CInput::EnumJoyCallback(const DIDEVICEINSTANCE *lpddi, VOID *pvRef)
{
	// ----- �ϐ��錾
	DIDEVCAPS	diDevCaps;		// �f�o�C�X���

	// ----- �W���C�X�e�B�b�N�p�f�o�C�X�I�u�W�F�N�g���쐬
	if(FAILED(m_pDInput->CreateDevice(lpddi->guidInstance, &m_pDIDevJoypad[m_nNumCont], NULL)))
		return DIENUM_CONTINUE;		// �񋓂𑱂���

	// ----- �W���C�X�e�B�b�N�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if(FAILED(m_pDIDevJoypad[m_nNumCont]->GetCapabilities(&diDevCaps)))
	{
		if(m_pDIDevJoypad[m_nNumCont])
			m_pDIDevJoypad[m_nNumCont]->Release();
		m_pDIDevJoypad[m_nNumCont] = NULL;
		return DIENUM_CONTINUE;		// �񋓂𑱂���
	}

	// ----- �K�萔�ɒB������I��
	m_nNumCont ++;
	if(m_nNumCont == MAX_CONTROLER)
		return DIENUM_STOP;			// �񋓂��I������
	else
		return DIENUM_CONTINUE;		// �񋓂𑱂���
}

//����������������������������������������������������������������������������������������
//	Name        : �Q�[���p�b�h�̏I������
//	Description : �Q�[���p�b�h�Ɋւ�����̌�n�������A�������I������
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CInput::UninitJoypad(void)
{
	for(int nLoop = 0; nLoop < MAX_CONTROLER; nLoop ++)
		SAFE_RELEASE(m_pDIDevJoypad[nLoop]);
}

//����������������������������������������������������������������������������������������
//	Name        : �Q�[���p�b�h�̍X�V����
//	Description : �Q�[���p�b�h�Ɋւ�����y�я������X�V����
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CInput::UpdateJoypad(void)
{
	// ----- �ϐ��錾
	int nLoop;

	for(nLoop = 0; nLoop < MAX_CONTROLER; nLoop ++)
	{
		if(m_pDIDevJoypad[nLoop])
		{
			// ----- �f�o�C�X����f�[�^���擾
			if(FAILED(m_pDIDevJoypad[nLoop]->GetDeviceState(sizeof(DIJOYSTATE), &m_joyState[nLoop])))
				m_pDIDevJoypad[nLoop]->Acquire();
		}
	}
}

*/


//����������������������������������������������������������������������������������������
//	Name        : �L�[���͍X�V
//	Description : �L�[���͂��X�V����
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CInput::UpdateInputKey(void)
{
	for(int nCntKey = 0; nCntKey < 256; nCntKey ++)
	{
		g_trgKey[nCntKey] = (m_keyState[nCntKey]^g_prsKey[nCntKey]) & m_keyState[nCntKey];
		g_rlsKey[nCntKey] = (m_keyState[nCntKey]^g_prsKey[nCntKey]) & g_prsKey[nCntKey];

		g_prsKey[nCntKey] = m_keyState[nCntKey];
	}
}

//����������������������������������������������������������������������������������������
//	Name        : �}�E�X���͍X�V
//	Description : �}�E�X�ɂ��L�[���͂��X�V����
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CInput::UpdateClick(void)
{
	for(int nCntClick = 0; nCntClick < 8; nCntClick ++)
	{
		g_trgClick[nCntClick] = (m_mouseState.rgbButtons[nCntClick]^g_prsClick[nCntClick]) & m_mouseState.rgbButtons[nCntClick];
		g_rlsClick[nCntClick] = (m_mouseState.rgbButtons[nCntClick]^g_prsClick[nCntClick]) & g_prsClick[nCntClick];

		g_prsClick[nCntClick] = m_mouseState.rgbButtons[nCntClick];
	}
}

//����������������������������������������������������������������������������������������
//	Name        : �J�[�\���X�V
//	Description : �}�E�X�J�[�\���̍��W���X�V����
//	Arguments   : hWnd / �E�B���h�E�n���h��
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CInput::UpdateCursor(HWND hWnd)
{
	GetCursorPos(&g_curPos);
	ScreenToClient(hWnd, &g_curPos);
}


//========================================================================================
// namespace:
//========================================================================================

//����������������������������������������������������������������������������������������
//	Name        : �v���X�L�[�擾
//	Description : �v���X�L�[���擾����
//	Arguments   : key / ���̓L�[
//	Returns     : �L�[���
//����������������������������������������������������������������������������������������
BYTE Input::GetPrsKey(int key)
{
	// ----- �ϐ��錾
	BYTE prsKey;

	// ----- �L�[�����m�F
	prsKey = g_prsKey[key] & 0x80;

	return prsKey;
}

//����������������������������������������������������������������������������������������
//	Name        : �g���K�L�[�擾
//	Description : �g���K�L�[���擾����
//	Arguments   : key / ���̓L�[
//	Returns     : �L�[���
//����������������������������������������������������������������������������������������
BYTE Input::GetTrgKey(int key)
{
	// ----- �ϐ��錾
	BYTE trgKey;

	// ----- �L�[�����m�F
	trgKey = g_trgKey[key] & 0x80;

	return trgKey;
}

//����������������������������������������������������������������������������������������
//	Name        : �����[�X�L�[�擾
//	Description : �����[�X�L�[���擾����
//	Arguments   : key / ���̓L�[
//	Returns     : �L�[���
//����������������������������������������������������������������������������������������
BYTE Input::GetRlsKey(int key)
{
	// ----- �ϐ��錾
	BYTE rlsKey;

	// ----- �L�[�����m�F
	rlsKey = g_rlsKey[key] & 0x80;

	return rlsKey;
}

//����������������������������������������������������������������������������������������
//	Name        : �v���X�N���b�N�擾
//	Description : �v���X�N���b�N���擾����
//	Arguments   : click / �N���b�N���
//	Returns     : �N���b�N���
//����������������������������������������������������������������������������������������
BYTE Input::GetPrsClick(int click)
{
	return g_prsClick[click];
}

//����������������������������������������������������������������������������������������
//	Name        : �g���K�N���b�N�擾
//	Description : �g���K�N���b�N���擾����
//	Arguments   : click / �N���b�N���
//	Returns     : �N���b�N���
//����������������������������������������������������������������������������������������
BYTE Input::GetTrgClick(int click)
{
	return g_trgClick[click];
}

//����������������������������������������������������������������������������������������
//	Name        : �����[�X�N���b�N�擾
//	Description : �����[�X�N���b�N���擾����
//	Arguments   : click / �N���b�N���
//	Returns     : �N���b�N���
//����������������������������������������������������������������������������������������
BYTE Input::GetRlsClick(int click)
{
	return g_rlsClick[click];
}

//����������������������������������������������������������������������������������������
//	Name        : �J�[�\�����W�擾
//	Description : �}�E�X�J�[�\���̍��W���擾����
//	Arguments   : None.
//	Returns     : �J�[�\�����W
//����������������������������������������������������������������������������������������
POINT& Input::GetCurPos(void)
{
	return g_curPos;
}


//========================================================================================
//	End of File
//========================================================================================