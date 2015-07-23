//========================================================================================
//		File        : PlayersGroup.h
//		Program     : �v���C���[�ǂ�
//
//		Description : �v���C���[�ǂ��̓���,��΂���
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
#include "../../h/Scene/GameMain.h"
#include "../../h/Object/PlayersGroup.h"

//========================================================================================
// public:
//========================================================================================
JOYINFOEX JoyInfoEx1;
JOYINFOEX JoyInfoEx1Prev;
// �\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// using�錾
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
using namespace Input;
//����������������������������������������������������������������������������������������
//	Name        : �R���X�g���N�^
//	Arguments   : None.
//����������������������������������������������������������������������������������������
CPlayersGroup::CPlayersGroup()
{
	m_nCurrentControllNo = 0;		// ���삷����擪�ɃZ�b�g
	m_bOver = false;
}
//����������������������������������������������������������������������������������������
//	Name        : ����
//	Description : �I�u�W�F�N�g�𐶐�����
//	Arguments   : pszFName / �ǂݍ��݃t�@�C����
//	Returns     : �I�u�W�F�N�g�f�[�^
//����������������������������������������������������������������������������������������
CPlayersGroup* CPlayersGroup::Create(const LPCTSTR pszFName)
{
	// ----- �ϐ��錾
	CPlayersGroup* pObj;

	// ----- ����������
	pObj = new CPlayersGroup();

	pObj->SetTexture(pszFName);

	return pObj;
}
//����������������������������������������������������������������������������������������
//	Name        : ������
//	Description : �����ȏ�����
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CPlayersGroup::Init()
{
	for(m_listIt = m_list.begin(); m_listIt != m_list.end();)
	{
		// ����ȁAUTSUWA���Ȃ��ƒ�������Ȃ���
		CPlayer* p = *m_listIt;

		// ������
		p->Init();

		// �����΂ɍŌ�ȁ�(�Ă����񂱂ꂢ��Ȃ��ǂ�)
		++m_listIt;
	}

	if(m_list.size() == 0){
		AddPlayer(D3DXVECTOR3(CMapData::GetStartPoint().x + 5,CMapData::GetStartPoint().y,0));
		AddPlayer(D3DXVECTOR3(CMapData::GetStartPoint().x - 5,CMapData::GetStartPoint().y,0));
	}
	m_bOver = false;
}
//����������������������������������������������������������������������������������������
//	Name        : ��n��
//	Description : �����Ȍ�n��
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CPlayersGroup::Uninit()
{
	// �v�f�S���폜
	for(m_listIt = m_list.begin(); m_listIt != m_list.end();)
	{
		// ����ȁAUTSUWA���Ȃ��ƒ�������Ȃ���
		CPlayer* p = *m_listIt;

		p->Uninit();
		SAFE_RELEASE(p)
		m_listIt = m_list.erase( m_listIt );
		continue;
		// �����΂ɍŌ�ȁ�(�Ă����񂱂ꂢ��Ȃ��ǂ�)
		++m_listIt;
	}

	m_nCurrentControllNo = 0;
	m_pStage = NULL;
}

//����������������������������������������������������������������������������������������
//	Name        : �X�V
//	Description : �����ȍX�V
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CPlayersGroup::Update()
{
	int currentNo = 0;			// ���̔ԍ�
	CPlayer* Player = NULL;			// ���삷����

	// �����p
	int			throwNo = 0;

	JoyInfoEx1.dwSize = sizeof(JOYINFOEX);
	JoyInfoEx1.dwFlags = JOY_RETURNALL; // �S�Ă̏����擾

	bool bThrow =false;

	if(joyGetPosEx(0, &JoyInfoEx1) == JOYERR_NOERROR){
	if (GetTrgKey(DIK_UP) ||(JoyInfoEx1.dwYpos < 0x7FFF - 0x4000 && !(JoyInfoEx1Prev.dwYpos < 0x7FFF - 0x4000))){
//		CGameMain::PlaySE(SE_RIDE);
		if(m_list.size() > (unsigned int)m_nCurrentControllNo + 1){
			CGameMain::PlaySE(SE_RIDE);
			// ���n���̂݌���
			if(!(GetPlayer(m_nCurrentControllNo + 1)->GetStatus() & ST_FLYING))
				m_nCurrentControllNo++;
		}
	}
	if(GetTrgKey(DIK_DOWN) ||(JoyInfoEx1.dwYpos < 0x7FFF + 0x4000 && !(JoyInfoEx1Prev.dwYpos < 0x7FFF + 0x4000))){
		CGameMain::PlaySE(SE_GOUSEI);
		if(m_nCurrentControllNo > 0){
			for(int i = 0;i < m_nCurrentControllNo;i++){
				GetPlayer(i)->EnableDelete();
			}
			switch(m_nCurrentControllNo)
			{
			case PLAYER_ARROW:
				GetPlayer(m_nCurrentControllNo)->SetGrane(PLAYER_ARROW);
				break;
			case PLAYER_JACK:
				GetPlayer(m_nCurrentControllNo)->SetGrane(PLAYER_JACK);
				break;
			case PLAYER_STONE:
				GetPlayer(m_nCurrentControllNo)->SetGrane(PLAYER_STONE);
				break;
			}
			m_nCurrentControllNo = 0;
		}
	}

	if (GetTrgKey(DIK_Z)|| ((JoyInfoEx1.dwButtons & JOY_BUTTON6) && !(JoyInfoEx1Prev.dwButtons & JOY_BUTTON6))){
		bThrow = true;
		}
	}
	if(m_list.size() == 0)
	{
		m_bOver = true;
		return ;
	}
	// �v�f�S���X�V
	for(m_listIt = m_list.begin(); m_listIt != m_list.end();)
	{
		// UTSUWA���Ȃ��ƒ�������Ȃ���
		CPlayer* p = *m_listIt;

		p->SetStage(m_pStage);

		// �v���C���[�Ɍ��݂̔ԍ����Z�b�g
		p->SetNo(currentNo);

		// ���݂̔ԍ�������ԍ��Ɠ����Ȃ�Player�𑀍�ݒ�ɂ���ȊO�͂��̑��ݒ��
		if(p->GetNo() == m_nCurrentControllNo){
			p->SetType(P_TYPE_PLAYER);
			Player = p;
		}

		if(p->GetType() != P_TYPE_WAIT){
			// ���ɕt���Ă���z��
			if(p->GetNo() > m_nCurrentControllNo){
				p->SetType(P_TYPE_OTHER);
				// ���삷���ݒ�
				for(unsigned int i = 1;i < m_list.size() + 1;i++)
				{
					if(GetPlayer(p->GetNo() - i)->GetType() != P_TYPE_WAIT){
						p->SetPlayer(GetPlayer(p->GetNo() - i));
						Player = p;
						break;
					}
				}
			}
			// ������A��
			if(p->GetNo() < m_nCurrentControllNo){
				if(p->GetType() == P_TYPE_THROW_READY_READY){
				
				}
				if(p->GetType() == P_TYPE_PLAYER){
					// �Ǐ]����v���C���[�����̓z��
					Player = GetPlayer(p->GetNo() + 1);
					p->SetPlayer(Player);
					p->SetType(P_TYPE_THROW_READY_READY);
					p->SetLastTime();
				}
				if(bThrow){
					for(int i = 0;i < m_nCurrentControllNo;++i){
						if(p->GetType() == P_TYPE_THROW_READY){
							CGameMain::PlaySE(SE_THROW);
							p->SetType(P_TYPE_THROW);
							bThrow = false;
						}
					}
				}
			}
		}
		// �X�V
		p->Update();

		// ��������폜
		if(p->GetPosY() < -1000){
			p->EnableDelete();
		}

		if(p->GetDelete()){
			// ���삷���̏ꍇ���̓z�𑀍�ݒ�ɂ���
			switch(p->GetType())
			{
			case P_TYPE_PLAYER:
				if(m_nCurrentControllNo > 0){
					m_nCurrentControllNo--;
					GetPlayer(m_nCurrentControllNo)->SetType(P_TYPE_PLAYER);
				}
				else if(GetPlayer(m_nCurrentControllNo + 1)){
					GetPlayer(m_nCurrentControllNo + 1)->SetType(P_TYPE_PLAYER);
				}
				break;
			case P_TYPE_OTHER:
				break;
			case P_TYPE_THROW_READY_READY:
				break;
			case P_TYPE_THROW_READY:
				break;
			case P_TYPE_THROW:
				m_nCurrentControllNo--;
				break;
			case P_TYPE_FLOWER:
				m_nCurrentControllNo--;
				break;
			}
			// �폜
			p->Uninit();
			SAFE_RELEASE(p)
			m_listIt = m_list.erase(m_listIt);
			continue;
		}
		
		if( p->GetType() == P_TYPE_FLOWER){
		
		}

		// �ԍ����X�V����
		++currentNo;

		// �����΂ɍŌ�ȁ�
		++m_listIt;
	}

	// �f�o�b�O�p
	if (GetTrgKey(DIK_1)){		// 1
		if(m_list.size() < 9){
		AddPlayer(D3DXVECTOR3(GetPlayer(m_nCurrentControllNo)->GetPosition().x,
			GetPlayer(m_nCurrentControllNo)->GetPosition().y + 100,
			GetPlayer(m_nCurrentControllNo)->GetPosition().z));
		}
	}
	if (GetTrgKey(DIK_2)){		// 2
		RedusePlayer();
	}

	JoyInfoEx1Prev = JoyInfoEx1;
}
//����������������������������������������������������������������������������������������
//	Name        : �`��
//	Description : �����ȍX�V
//	Arguments   : �Ȃ���
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CPlayersGroup::Draw()
{
	// �v�f�S���`��
	for(m_listIt = m_list.begin(); m_listIt != m_list.end();)
	{
		// UTSUWA���Ȃ��ƒ�������Ȃ���
		CPlayer* p = *m_listIt;

		// �`��
		p->Draw();

		// �����΂ɍŌ�ȁ�
		++m_listIt;
	}
}
//����������������������������������������������������������������������������������������
//	Name        : Player���擾
//	Description : Player���擾
//	Arguments   : Player�̔ԍ�
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
CPlayer* CPlayersGroup::GetPlayer(int no)
{
	for(std::list<CPlayer*>::iterator it = m_list.begin(); it != m_list.end();)
	{
		// UTSUWA���Ȃ��ƒ�������Ȃ���
		CPlayer* p = *it;

		// ���ʔԍ����ꏏ��������Ԃ�
		if(p->GetNo() == no){
			return p;
		}

		// �����΂ɍŌ�ȁ�
		++it;
	}

	// �Ȃ�������NULL��
	return NULL;
}
//����������������������������������������������������������������������������������������
//	Name        : Player�ǉ�
//	Description : Player�ǉ�
//	Arguments   : Player�̃|�C���^
//	Returns     : �Ȃ���
//����������������������������������������������������������������������������������������
void CPlayersGroup::AddPlayer(CPlayer* p)
{
	m_list.push_back(p);
}
//����������������������������������������������������������������������������������������
//	Name        : Player�ǉ�
//	Description : �Ō����Player�ǉ�
//	Arguments   : Player�̃|�C���^
//	Returns     : nai!
//����������������������������������������������������������������������������������������
void CPlayersGroup::AddPlayer()
{
	CPlayer* p;		// �ǉ�������
	// ����
	p = CPlayer::Create(m_lpTex);
	// ������
	p->Init();

	// :::: ���X�g�ɒǉ� ::::: //
	m_list.push_back(p);
}
//����������������������������������������������������������������������������������������
//	Name        : Player�ǉ�
//	Description : �Ō���ɍ��W
//	Arguments   : Player�̃|�C���^
//	Returns     : nai!
//����������������������������������������������������������������������������������������
void CPlayersGroup::AddPlayer(D3DXVECTOR3 pos)
{
	CPlayer* p;	// �ǉ�������
	// ����
	p = CPlayer::Create(m_lpTex);
	// ������
	p->Init(pos);
	// :::: ���X�g�ɒǉ� ::::: //
	m_list.push_back(p);
}

//����������������������������������������������������������������������������������������
//	Name        : �v���C���[�ʒu�X�V
//	Description : �S�v���C���[��Z���W���X�V
//	Arguments   : z / Z���W
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CPlayersGroup::PlayersTranslateZ(float z)
{
	for(std::list<CPlayer*>::iterator it = m_list.begin(); it != m_list.end(); ++it) {
		(*it)->TranslateZ(z);
	}
}

//����������������������������������������������������������������������������������������
//	Name        : �G�o�ʒu�X�V
//	Description : �S�G�o��Z���W���X�V
//	Arguments   : z / Z���W
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CPlayersGroup::TactilesTranslateZ(float z)
{
	for(std::list<CPlayer*>::iterator it = m_list.begin(); it != m_list.end(); ++it) {
		(*it)->TactileTranslateZ(z);
	}
}

//����������������������������������������������������������������������������������������
//	Name        : �N���A���o
//	Description : �N���A���̂��˂ۂ�̓��쉉�o����
//	Arguments   : None.
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CPlayersGroup::ClearDirection()
{
	for(std::list<CPlayer*>::iterator it = m_list.begin(); it != m_list.end(); ++it) {
		(*it)->ClearDirection();
	}
}

//����������������������������������������������������������������������������������������
//	Name        : Player�폜
//	Description : �Ō����Player�폜
//	Arguments   : Player�̃|�C���^
//	Returns     : �Ȃ��I
//����������������������������������������������������������������������������������������
void CPlayersGroup::RedusePlayer()
{
	// 1�̂������Ȃ������猸�炳�Ȃ�
	if(m_list.size() == 1)
		return;

	// UTSUWA
	CPlayer* p = m_list.back();
	// ��n��
	p->Uninit();
	// �J��
	SAFE_RELEASE(p)

	// :::: ���X�g����폜 ::::: //
	m_list.pop_back();
}

//========================================================================================
//	End of File
//========================================================================================