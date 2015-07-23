//========================================================================================
//		File        : Input.h
//		Program     : ���̓f�o�C�X�N���X
//
//		Description : ���̓f�o�C�X�N���X�̒�`
//
//		History     : 2013/07/02	�쐬�J�n
//						   10/24	�W���C�p�b�h�֘A���R�����g�A�E�g���Ė�����
//					  2014/08/07	�V���O���g���N���X��
//
//																Author : Kei Hashimoto
//========================================================================================

#pragma once

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �C���N���[�h
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
#include "../System/PreInclude.h"
#
#include <dinput.h>

// ----- ���C�u�����̃����N
#pragma comment(lib, "dinput8.lib")

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �萔��`
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
#define MAX_CONTROLER	(2)		// �R���g���[���ő吔

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �N���X��`
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
class CInput
{
// ===== �����o�ϐ�
private:
	BYTE					m_keyState[256];					// �L�[�{�[�h���z��
	DIMOUSESTATE2			m_mouseState;						// �}�E�X���z��
	DIJOYSTATE				m_joyState[MAX_CONTROLER];			// �Q�[���p�b�h���z��

	LPDIRECTINPUT8			m_pDInput;							// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^

	LPDIRECTINPUTDEVICE8	m_pDIDevKeyboard;					// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
	LPDIRECTINPUTDEVICE8	m_pDIDevMouse;						// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�}�E�X)
	LPDIRECTINPUTDEVICE8	m_pDIDevJoypad[MAX_CONTROLER];		// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�Q�[���p�b�h)

	int						m_numCnt;							// ���������Q�[���p�b�h�̐�

// ===== �����o�֐�
public:
	static CInput& GetInstance();						// �C���X�^���X�擾
	HRESULT	InitInput(HINSTANCE hInst, HWND hWnd);		// Input������
	void	Release(void);								// Input��n��
	void	UpdateInput(HWND hWnd);						// ���͏��X�V

private:
	CInput();
	CInput(const CInput&) {}
	CInput &operator=(const CInput&) {}
	virtual ~CInput();

	// �L�[�{�[�h�֘A
	HRESULT InitKeyboard(HWND hWnd);
	void UninitKeyboard(void);
	void UpdateKeyboard(void);
	void UpdateInputKey(void);

	// �}�E�X�֘A
	HRESULT InitMouse(HWND hWnd);
	void UninitMouse(void);
	void UpdateMouse(void);
	void UpdateClick(void);
	void UpdateCursor(HWND hWnd);

	// �Q�[���p�b�h�֘A
	HRESULT InitJoypad(HWND hWnd);
	void UninitJoypad(void);
	void UpdateJoypad(void);
	BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE *lpddi, VOID *pvRef);
};

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// ���O��Ԓ�`
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
namespace Input
{
	BYTE	GetPrsKey(int key);			// �v���X�L�[�擾
	BYTE	GetTrgKey(int key);			// �g���K�L�[�擾
	BYTE	GetRlsKey(int key);			// �����[�X�L�[�擾
	BYTE	GetPrsClick(int click);		// �v���X�N���b�N�擾
	BYTE	GetTrgClick(int click);		// �g���K�N���b�N�擾
	BYTE	GetRlsClick(int click);		// �����[�X�N���b�N�擾

	POINT&	GetCurPos(void);			// �J�[�\�����W�擾
}


//========================================================================================
//	End of File
//========================================================================================