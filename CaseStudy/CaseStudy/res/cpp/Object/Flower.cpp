//========================================================================================
//		File        : Flower.h
//		Program     : ��
//
//		Description : 
//
//		History     : 2015/05/26	�쐬�J�n
//						   
//
//																Author : ���񂽂܃Y
//========================================================================================


//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �C���N���[�h
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
#include "../../h/System/Input.h"
#include "../../h/Object/Flower.h"

//========================================================================================
// public:
//========================================================================================

//����������������������������������������������������������������������������������������
//	Name        : �R���X�g���N�^
//	Arguments   : None.
//����������������������������������������������������������������������������������������
CFlower::CFlower()
{
	m_nPhase = FLOWER_PHASE_INIT;
	m_stageType = CStage::ST_LIGHT;
}
//����������������������������������������������������������������������������������������
//	Name        : ������
//	Description : ������
//	Arguments   : 
//	Returns     : 
//����������������������������������������������������������������������������������������
void CFlower::Init(D3DXVECTOR3 pos,D3DXVECTOR3 dir,const LPCTSTR pszFName)
{
	m_kuki = CObject2D::Create(pszFName);
	//m_kuki->Init(D3DXVECTOR3(1,1,0));

	// �L�����N�^�[�̏�����
	CCharacter::Init();
	Resize(D3DXVECTOR2(FLOWER_SIZE_X,FLOWER_SIZE_Y));
	pos += dir * (FLOWER_SIZE_X / 2);
	Translate(pos);

	m_angle = AngleOf2Vector(pos,D3DXVECTOR3(0,1,0));

	if(m_angle > 0)
		RotateZ((float)m_angle);

	m_lastTime		= CTimer::GetTime();

	if(rand() % 2 == 0)
		m_rotSpd = 3;
	else if(rand() % 2 == 1)
		m_rotSpd = -3;

	// �A�j���[�V����������
	StartAnimation();

	UVDivision(0, FLOWER_ANIME_SIZE_X, FLOWER_ANIME_SIZE_Y);
}
//����������������������������������������������������������������������������������������
//	Name        : ������
//	Description : ������
//	Arguments   : 
//	Returns     : 
//����������������������������������������������������������������������������������������
void CFlower::Uninit()
{
	CCharacter::Uninit();

}
//����������������������������������������������������������������������������������������
//	Name        : ����
//	Description : �I�u�W�F�N�g�𐶐�����
//	Arguments   : pszFName / �ǂݍ��݃t�@�C����
//	Returns     : �I�u�W�F�N�g�f�[�^
//����������������������������������������������������������������������������������������
CFlower* CFlower::Create(const LPCTSTR pszFName)
{
	// ----- �ϐ��錾
	CFlower* pObj;
	// ----- ����������
	pObj = new CFlower();
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
void CFlower::Update()
{	

	switch(m_nPhase)
	{
	case FLOWER_PHASE_INIT:
		m_lastTime = CTimer::GetTime();
		m_nPhase++;
		break;
	case FLOWER_PHASE_START:
		RotationZ((float)(m_nowTime - m_lastTime) * m_rotSpd);
		m_nowTime = CTimer::GetTime();
		if(m_nowTime - m_lastTime > abs(m_rotSpd)){
			m_nPhase = FLOWER_PHASE_FLOWER;
		}
		break;
	case FLOWER_PHASE_FLOWER:
		RotationZ(m_rotSpd * abs(m_rotSpd));
		break;
	case FLOWER_PHASE_WAIT:
		RotationZ(m_rotSpd);
		break;
	case FLOWER_PHASE_UNINIT:
		break;
	}

	Translate(m_pos);

	Animation();

}
//����������������������������������������������������������������������������������������
//	Name        : �A�j��
//	Description : �A�j���[�����
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CFlower::Animation()
{
	// ��Ԃɂ���ăA�j���[�V�����ω�
	FrameAnimation(0, 0, 1, 1, 0.5f);
}
//����������������������������������������������������������������������������������������
//	Name        : �A�j��
//	Description : �A�j���[�����
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CFlower::Draw()
{
	CCharacter::DrawAlpha();

	//m_kuki->DrawAlpha();
}
//�Q�̃x�N�g��AB�̂Ȃ��p�x�Ƃ����߂�
double CFlower::AngleOf2Vector(D3DXVECTOR3 A, D3DXVECTOR3 B )
{
	//�x�N�g��A��B�̒������v�Z����
	double length_A = D3DXVec3Length(&A);
	double length_B = D3DXVec3Length(&B);

	//���ςƃx�N�g���������g����cos�Ƃ����߂�
	double cos_sita = D3DXVec3Dot(&A,&B) / ( length_A * length_B );

	//cos�Ƃ���Ƃ����߂�
	double sita = acos( cos_sita );	

	//���W�A���łȂ�0�`180�̊p�x�łق����ꍇ�̓R�����g�O��
	sita = sita * 180.0 / (3.1415);

	return sita;
}
//========================================================================================
//	End of File
//========================================================================================