//========================================================================================
//		File        : Object2Dsub.cpp
//		Program     : 2D�I�u�W�F�N�g�x�[�X�̊g��
//
//		Description : Update�̎����i��ɏd�͓I�ȁj
//			
//
//		History     : 2015/5/6	�쐬�J�n
//
//																Author : ���񂽂܃Y
//========================================================================================


//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �C���N���[�h
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
#include "../../h/System/PreInclude.h"
#
#include <math.h>
#include "../../h/Object/Object2Dsub.h"

//========================================================================================
// public:
//========================================================================================
//����������������������������������������������������������������������������������������
//	Name        : �R���X�g���N�^
//	Arguments   : None.
//����������������������������������������������������������������������������������������
CObject2Dsub::CObject2Dsub()
{
	m_nPhase = 0;

	m_fGAccel = 0.0f;
	m_fGravity = GRAVITY_ZERO;	// �d��

	m_accel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
}
//����������������������������������������������������������������������������������������
//	Name        : Update
//	Arguments   : None.
//����������������������������������������������������������������������������������������
void CObject2Dsub::Update()
{
	CObject2D::Update();
	// �d�͂�t����(�������K��)
	if (m_fGravity != GRAVITY_ZERO){
		m_fGAccel += m_fGravity;		// �������x���グ�Ă�
		m_pos.y -= m_fGAccel;
	}
	else{
		m_fGAccel = 0.0f;				// �������x��0��
	}

	// �ʒu�𔽉f
	Translate(m_pos);
	// �g�嗦�𔽉f
	Scale(m_scale);
}

//����������������������������������������������������������������������������������������
//	Name        : SetGravity
//	Arguments   : �d��.
//����������������������������������������������������������������������������������������
void CObject2Dsub::SetGravity(float g)
{
	m_fGravity = g;
}

//========================================================================================
//	End of File
//========================================================================================