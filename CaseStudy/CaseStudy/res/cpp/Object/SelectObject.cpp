//========================================================================================
//		File        : SelectObject.cpp
//		Program     : シーン遷移システム
//
//		Description : シーン遷移システムの実装
//
//		History     : 2015/02/20	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================

//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include <tchar.h>
#include "../../h/System/System.h"
#include "../../h/Object/SelectObject.h"
#include "../../h/System/Input.h"


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CSelectObject::CSelectObject()
{

}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CSelectObject::~CSelectObject()
{

}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成
//	Description : オブジェクトを生成する
//	Arguments   : pszFName / 読み込みファイル名
//	Returns     : オブジェクトデータ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CSelectObject* CSelectObject::Create(const LPCTSTR pszFName)
{
	// ----- 変数宣言
	CSelectObject* pObj;

	// ----- 初期化処理
	pObj = new CSelectObject();
	if (pObj)
	{
		if (!pObj->Initialize(pszFName))
		{
			SAFE_DELETE(pObj);
		}
	}

	return pObj;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : シーン遷移 仮2 (画面から出現)
//	Description : 上から出現
//	Arguments   : pTex / テクスチャ
//				  y    / Y方向移動量
//	Returns     : 完了フラグ(true:遷移完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
int CSelectObject::PlayerUpdate(int nStatus)
{
	if (m_scale.x > 0)
		ScaleX(-1.0f);

	switch (nStatus)
	{
	case S_STATUS_WAIT:

		break;
	case S_STATUS_LEFT:
		break;
	case S_STATUS_RIGHT:
		break;
	}

	CCharacter::Update();

	return nStatus;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : シーン遷移 仮2 (画面から出現)
//	Description : 上から出現
//	Arguments   : pTex / テクスチャ
//				  y    / Y方向移動量
//	Returns     : 完了フラグ(true:遷移完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
int CSelectObject::ArrowUpdate(int nNo,int nStatus)
{
#define ARROW_LEFT	1
#define ARROW_RIGHT 2

	if (nNo == ARROW_RIGHT)
		if (GetScaleX() > 0.0f)
			ScaleX(-1.0f);
	
	switch (nStatus)
	{
		case S_STATUS_WAIT:
			if (nNo == ARROW_LEFT)
			{
				if (m_pos.x < LEFT_ARROW_INIT_POS_X)
					TranslationX(1.0f);
			}
			else
			{
				if (m_pos.x > RIGHT_ARROW_INIT_POS_X)
					TranslationX(-1.0f);
			}
			break;
		case S_STATUS_LEFT:
			if (nNo == ARROW_LEFT)
			{
				for (int i = 0; i <= MOVE_COUNT; i++)
				{
					if (m_pos.x > LEFT_ARROW_INIT_POS_X - MOVE_COUNT)
						TranslationX(-1.0f);

					if (i >= MOVE_COUNT)
						return S_STATUS_WAIT;
				}
			}
			else
			{
				if (m_pos.x > RIGHT_ARROW_INIT_POS_X)
					TranslationX(-1.0f);
			}
			break;
		case S_STATUS_RIGHT:
			if (nNo == ARROW_RIGHT)
			{
				for (int i = 0; i <= MOVE_COUNT; i++)
				{
					if (m_pos.x < RIGHT_ARROW_INIT_POS_X + MOVE_COUNT)
						TranslationX(1.0f);
					if (i >= MOVE_COUNT)
						return S_STATUS_WAIT;
				}
			}
			else
			{
				if (m_pos.x < LEFT_ARROW_INIT_POS_X)
					TranslationX(1.0f);
			}
			break;
	}

	CCharacter::Update();

	return nStatus;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : シーン遷移 仮2 (画面から出現)
//	Description : 上から出現
//	Arguments   : pTex / テクスチャ
//				  y    / Y方向移動量
//	Returns     : 完了フラグ(true:遷移完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CSelectObject::RogoUpdate(int nNo,int nStatus,int nStage)
{
#define ROGO_STAGE_1	1
#define ROGO_STAGE_2	2
	switch (nStatus)
	{
	case S_STATUS_WAIT:
		break;
	case S_STATUS_LEFT:
		if (nNo == ROGO_STAGE_1)
		{
			if (nStage == S_STAGE_1)
			{
				TranslateX(ROGO_INIT_POS_X);
				TranslateY(ROGO_INIT_POS_Y);
			}
			else
			{
				TranslateX(ROGO_SCREEN_OUT_POS_X);
				TranslateY(ROGO_SCREEN_OUT_POS_Y);
			}
		}
		else
		{
			if (nStage == S_STAGE_2)
			{
				TranslateX(ROGO_INIT_POS_X);
				TranslateY(ROGO_INIT_POS_Y);
			}
			else
			{
				TranslateX(ROGO_SCREEN_OUT_POS_X);
				TranslateY(ROGO_SCREEN_OUT_POS_Y);
			}
		}
		break;
	case S_STATUS_RIGHT:
		if (nNo == ROGO_STAGE_1)
		{
			if (nStage == S_STAGE_1)
			{
				TranslateX(ROGO_INIT_POS_X);
				TranslateY(ROGO_INIT_POS_Y);
			}
			else
			{
				TranslateX(ROGO_SCREEN_OUT_POS_X);
				TranslateY(ROGO_SCREEN_OUT_POS_Y);
			}
		}
		else
		{
			if (nStage == S_STAGE_2)
			{
				TranslateX(ROGO_INIT_POS_X);
				TranslateY(ROGO_INIT_POS_Y);
			}
			else
			{
				TranslateX(ROGO_SCREEN_OUT_POS_X);
				TranslateY(ROGO_SCREEN_OUT_POS_Y);
			}
		}
		break;
	}


	CCharacter::Update();
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : シーン遷移 仮2 (画面から出現)
//	Description : 上から出現
//	Arguments   : pTex / テクスチャ
//				  y    / Y方向移動量
//	Returns     : 完了フラグ(true:遷移完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
int CSelectObject::StageUpdate(int nStatus,int nStage)
{
	switch (nStatus)
	{
	case S_STATUS_WAIT:
		if (nStage == S_STAGE_1)
		{

			if (m_rot.z != 0.0f)
				RotationZ(2.0f);
		}
		else
		{
			if (m_rot.z != 180.0f)
				RotationZ(2.0f);
		}
		if (m_rot.z >= 360.0f)
			RotateZ(0.0f);
		break;
	case S_STATUS_LEFT:
		break;
	case S_STATUS_RIGHT:
		break;
	}

	CCharacter::Update();

	return nStatus;
}
//========================================================================================
//	End of File
//========================================================================================