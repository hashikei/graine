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

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �N���X��`
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
class CUI : public CCharacter
{
private:
	CCharacter *m_MaxGrane;
	CCharacter *m_ClearBlock;

	int			m_nMaxGrane;
	int			m_nCurrentGrane;

	int			m_nMaxClearBlock;
	int			m_nCurrentClearBlock;

	static const LPCTSTR COL_TEX_FILENAME;

	bool Initialize(const LPCTSTR pszFName);	// ������
protected:
		
public:
	virtual void Init(D3DXVECTOR3 pos);			// ������
	virtual void Uninit();
	virtual void Update();
	virtual void Animation();// �A�j��			����N���X�ɂ������������̂���
	virtual void Draw();
	static CUI* Create(const LPCTSTR pszFName);			// ����

	// ----- �Z�b�^�[
	
	// ----- �Q�b�^�[
	

};
 
//========================================================================================
//	End of File
//========================================================================================