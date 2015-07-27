//========================================================================================
//		File        : MapData.cpp
//		Program     : �}�b�v�f�[�^�N���X
//
//		Description : �}�b�v�f�[�^�N���X�̎���
//
//		History     : 2015/05/18	�쐬�J�n
//
//																Author : Kei Hashimoto
//========================================================================================


//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �x��������
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
#pragma warning(disable:4996)

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �C���N���[�h
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
#include "../../h/System/PreInclude.h"
#
#include <tchar.h>
#include <fstream>
#include <string>
#include <sstream>
#include "../../h/System/MapData.h"
#include "../../h/Scene/Game.h"

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �����o���̐錾
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// ----- �萔
// private:
LPCTSTR CMapData::MAPDATA_LIST[MAX_STAGEID] = {		// �}�b�v�f�[�^�̃t�@�C�������X�g
	_T("res/data/map/Stage1.csv"),
	_T("res/data/map/Stage2.csv"),
	_T("res/data/map/Stage3.csv"),
	_T("res/data/map/Stage4.csv"),
	_T("res/data/map/Stage5.csv"),
};
const int	CMapData::INIT_OBJECT_NUM	= 1000;		// �����I�u�W�F�N�g��
const int	CMapData::LAYOUTOBJ_HFRAME	= 6;		// ���C�A�E�g�I�u�W�F�N�g�̉�������
const int	CMapData::LAYOUTOBJ_VFRAME	= 2;		// ���C�A�E�g�I�u�W�F�N�g�̏c������

// ----- �ϐ�
// private:
LPFIELDBLOCK_ARRAY	CMapData::m_pFieldBlock;	// �t�B�[���h�u���b�N���X�g
LPCHARACTER_ARRAY	CMapData::m_pLayoutBlock;	// ���C�A�E�g�u���b�N���X�g
LPCHARACTER_ARRAY	CMapData::m_pLayoutObject;	// ���C�A�E�g�I�u�W�F�N�g���X�g
D3DXVECTOR2			CMapData::m_startPoint;		// �J�n�ʒu

float	CMapData::m_leftWallX;		// ����X���W
float	CMapData::m_rightWallX;		// �E��X���W
float	CMapData::m_topWallY;		// ���Y���W
float	CMapData::m_bottomWallY;	// ����Y���W

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �O���[�o���ϐ��錾
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
static CMapData& g_mapData = CMapData::GetInstance();		// �}�b�v�f�[�^�N���X�̎��̐���


//========================================================================================
// public:
//========================================================================================

//����������������������������������������������������������������������������������������
//	Name        : �C���X�^���X�擾
//	Description : �}�b�v�f�[�^�N���X�̃C���X�^���X���擾����
//	Arguments   : None.
//	Returns     : �}�b�v�f�[�^�N���X
//����������������������������������������������������������������������������������������
CMapData& CMapData::GetInstance()
{
	// ----- �C���X�^���X����
	static CMapData mapData;

	return mapData;
}

//����������������������������������������������������������������������������������������
//	Name        : �}�b�v�f�[�^���[�h
//	Description : �C�ӂ̃}�b�v�f�[�^�����[�h����
//	Arguments   : id / �X�e�[�WID
//	Returns     : ����(true:����)
//����������������������������������������������������������������������������������������
bool CMapData::LoadData(int id)
{
	// ----- ����������
	m_pFieldBlock.clear();
	m_pLayoutBlock.clear();
	m_pLayoutObject.clear();

	// ----- �}�b�v�f�[�^�ǂݍ���
	std::ifstream ifs(MAPDATA_LIST[id]);
	if (ifs.fail()) {
#ifdef _DEBUG_MESSAGEBOX
		LPTSTR str = new TCHAR[256];
		_swprintf(str, _T("MapData::Load map data error! stage ID %d."), id);
		MessageBox(NULL, str, _T("error"), MB_OK | MB_ICONERROR);
		delete[] str;
#endif
		return false;
	}
	std::istreambuf_iterator<char> it(ifs);
	std::istreambuf_iterator<char> last;
	std::string str(it, last);

	// ----- �}�b�v�f�[�^�o�^
	// �o�^����
	std::stringstream ss(str);
	std::string tmp;

	// �J�n�ʒu�ǂݍ���
	getline(ss, tmp, ',');		// X���W�o�^
	m_startPoint.x = stof(tmp);
	getline(ss, tmp, ',');		// Y���W�o�^
	m_startPoint.y = stof(tmp);
	getline(ss, tmp);			// ���s���X�L�b�v

	// ���E�ǈʒu�ǂݍ���
	getline(ss, tmp, ',');		// ���ǂ�X���W�o�^
	m_leftWallX = stof(tmp);
	getline(ss, tmp, ',');		// �E�ǂ�X���W�o�^
	m_rightWallX = stof(tmp);
	getline(ss, tmp);			// ���s���X�L�b�v

	// �㉺�ǈʒu�ǂݍ���
	getline(ss, tmp, ',');		// ��ǂ�Y���W�o�^
	m_topWallY = stof(tmp);
	getline(ss, tmp, ',');		// ���ǂ�Y���W�o�^
	m_bottomWallY = stof(tmp);
	getline(ss, tmp);			// ���s���X�L�b�v

	// �t�B�[���h�u���b�N�̃f�[�^�ǂݍ���
	CCharacter* pObj = NULL;
	CFieldBlock* pFBlock = NULL;
	int			eid = 0;
	int			prevBid = -1;
	int			cnt = 0;
	float		width = 0.0f;
	float		height = 0.0f;
	D3DXVECTOR3	color(0.0f, 0.0f, 0.0f);
	int			type = -1;
	while (getline(ss, tmp, ',')) {
		switch (cnt % MAX_DATAPARAM) {
		case DP_BID:
			break;

		case DP_EID:
			eid = stoi(tmp);
			if (eid <= 0) {
				pFBlock = CFieldBlock::Create();
				pFBlock->Init();
			}
			break;

		case DP_TEX:
		{
#ifdef _MULTI_THREAD_NOWLOADING
			EnterCriticalSection(CGame::GetCriticalSection());
#endif
			LPTSTR ws = new TCHAR[tmp.size() + 1];
			mbstowcs(ws, tmp.c_str(), tmp.size());
			ws[tmp.size()] = '\0';
			pObj = CCharacter::Create(ws);
			pObj->Init();
			delete[] ws;
#ifdef _MULTI_THREAD_NOWLOADING
			LeaveCriticalSection(CGame::GetCriticalSection());
#endif
			break;
		}

		case DP_POSX:
			pObj->TranslateX(stof(tmp));
			break;

		case DP_POSY:
			pObj->TranslateY(stof(tmp));
			break;

		case DP_POSZ:
			pObj->TranslateZ(stof(tmp));
			break;

		case DP_WIDTH:
			width = stof(tmp);
			break;

		case DP_HEIGHT:
			height = stof(tmp);
			pObj->Resize(D3DXVECTOR2(width, height));
			break;

		case DP_ANGLE:
			pObj->RotateZ(stof(tmp));
			break;

		case DP_COLR:
			color.x = stof(tmp);
			break;

		case DP_COLG:
			color.y = stof(tmp);
			break;

		case DP_COLB:
			color.z = stof(tmp);
			pObj->SetColor(color);
			break;

		case DP_COLA:
			pObj->SetAlpha(stoi(tmp));
			break;

		case DP_COLFLG:
			if (eid <= 0) {
				m_pFieldBlock.push_back(pFBlock);
			}
			if(stoi(tmp) > 0)
				m_pFieldBlock.back()->SetElement(pObj);
			break;

		case DP_TYPE:
		{
			// 0:���ʂ̃t�B�[���h�u���b�N
			// 1:�N���A�����t�B�[���h�u���b�N
			// 2:��Q�t�B�[���h�u���b�N
			// 3:���C�A�E�g�u���b�N
			// 4:���C�A�E�g�I�u�W�F�N�g
			type = stoi(tmp);
			switch(type)
			{
				case BT_NORMAL:
				case BT_CLEAR:
				case BT_OVER:
					m_pFieldBlock.back()->SetType(type);
					break;
		
				case BT_LAYOUT:
					m_pLayoutBlock.push_back(pObj);
					break;

				case BT_LAYOUTOBJ:
					m_pLayoutObject.push_back(pObj);
					break;
			}
				
			break;
		}

		case DP_TEX_NO:
			if(type == BT_LAYOUTOBJ)
				m_pLayoutObject.back()->UVDivision(stoi(tmp), LAYOUTOBJ_HFRAME, LAYOUTOBJ_VFRAME);
			break;

		default:
			break;
		}

		++cnt;	// ���̃f�[�^��
		if (cnt >= MAX_DATAPARAM) {
			getline(ss, tmp);		// ���s���X�L�b�v
			cnt = 0;
		}
	}

	return true;
}

//����������������������������������������������������������������������������������������
//	Name        : �}�b�v�f�[�^�j��
//	Description : ���ݕۑ����Ă���}�b�v�f�[�^��j������
//	Arguments   : id / �X�e�[�WID
//	Returns     : ����(true:����)
//����������������������������������������������������������������������������������������
void CMapData::DeleteData()
{
	for (LPFIELDBLOCK_ARRAY_IT it = m_pFieldBlock.begin(); it != m_pFieldBlock.end(); ++it) {
		SAFE_RELEASE((*it));
	}
	for (LPCHARACTER_ARRAY_IT it = m_pLayoutBlock.begin(); it != m_pLayoutBlock.end(); ++it) {
		SAFE_RELEASE((*it));
	}
	for (LPCHARACTER_ARRAY_IT it = m_pLayoutObject.begin(); it != m_pLayoutObject.end(); ++it) {
		SAFE_RELEASE((*it));
	}
}

//����������������������������������������������������������������������������������������
//	Name        : �t�B�[���h�u���b�N���X�g�擾
//	Description : �t�B�[���h�u���b�N���X�g���擾����
//	Arguments   : pObjList / �t�B�[���h�u���b�N���X�g�̊i�[��|�C���^
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CMapData::GetFieldBlockList(LPFIELDBLOCK_ARRAY* pObjList)
{
	pObjList->resize(m_pFieldBlock.size());
	std::copy(m_pFieldBlock.begin(), m_pFieldBlock.end(), pObjList->begin());
}

//����������������������������������������������������������������������������������������
//	Name        : ���C�A�E�g�u���b�N���X�g�擾
//	Description : ���C�A�E�g�u���b�N���X�g���擾����
//	Arguments   : pObjList / ���C�A�E�g�u���b�N���X�g�̊i�[��|�C���^
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CMapData::GetLayoutBlockList(LPCHARACTER_ARRAY* pObjList)
{
	pObjList->resize(m_pLayoutBlock.size());
	std::copy(m_pLayoutBlock.begin(), m_pLayoutBlock.end(), pObjList->begin());
}

//����������������������������������������������������������������������������������������
//	Name        : ���C�A�E�g�I�u�W�F�N�g���X�g�擾
//	Description : ���C�A�E�g�I�u�W�F�N�g���X�g���擾����
//	Arguments   : pObjList / ���C�A�E�g�I�u�W�F�N�g���X�g�̊i�[��|�C���^
//	Returns     : None.
//����������������������������������������������������������������������������������������
void CMapData::GetLayoutObjectList(LPCHARACTER_ARRAY* pObjList)
{
	pObjList->resize(m_pLayoutObject.size());
	std::copy(m_pLayoutObject.begin(), m_pLayoutObject.end(), pObjList->begin());
}

//����������������������������������������������������������������������������������������
//	Name        : �N���A�����u���b�N���擾
//	Description : �N���A�����t�B�[���h�u���b�N�����擾����
//	Arguments   : None.
//	Returns     : �N���A�����u���b�N��
//����������������������������������������������������������������������������������������
int CMapData::GetClearBlockNum()
{
	int num = 0;
	for (LPFIELDBLOCK_ARRAY_IT it = m_pFieldBlock.begin(); it != m_pFieldBlock.end(); ++it) {
		if ((*it)->GetType() == BT_CLEAR)
			++num;
	}

	return num;
}


//========================================================================================
// private:
//========================================================================================

//����������������������������������������������������������������������������������������
//	Name        : �R���X�g���N�^
//	Arguments   : None.
//����������������������������������������������������������������������������������������
CMapData::CMapData()
{
	m_pFieldBlock.reserve(INIT_OBJECT_NUM);
	m_pLayoutBlock.reserve(INIT_OBJECT_NUM);
	m_pLayoutObject.reserve(INIT_OBJECT_NUM);
	m_startPoint = D3DXVECTOR2(0.0f, 0.0f);

	m_leftWallX		= 0.0f;
	m_rightWallX	= 0.0f;
	m_topWallY		= 0.0f;
	m_bottomWallY	= 0.0f;
}

//����������������������������������������������������������������������������������������
//	Name        : �f�X�g���N�^
//	Arguments   : None.
//����������������������������������������������������������������������������������������
CMapData::~CMapData()
{
}


//========================================================================================
//	End of File
//========================================================================================