//========================================================================================
//		File        : PreInclude.h
//		Program     : ���O�C���N���[�h���C�u����
//
//		Description : ���炩���߃C���N���[�h���郉�C�u����
//
//		History     : 2014/07/17	�쐬�J�n
//
//																Author : Kei Hashimoto
//========================================================================================

#pragma once

//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// �C���N���[�h
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
// ----- �G���[���b�Z�[�W�{�b�N�X�𗘗p
#ifndef _DEBUG_MESSAGEBOX
#define _DEBUG_MESSAGEBOX
#endif

// ----- ���𑜓x�^�C�}�[�𗘗p
#ifndef __OPT_HIGH_TIMER
#define __OPT_HIGH_TIMER
#endif

// ----- �\�P�b�g�ʐM�𗘗p
//#ifndef __OPT_WINSOCK
//#define __OPT_WINSOCK
//#endif

#ifdef __OPT_WINSOCK
#include <WinSock2.h>

// ----- ���C�u�����̃����N
#pragma comment(lib, "ws2_32")
#endif

// ----- math.h�̃}�N���𗘗p
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

// ----- �}���`�X���b�h�ɂ��NowLoading���������s
//#ifndef _MULTI_THREAD_NOWLOADING
//#define _MULTI_THREAD_NOWLOADING
//#endif


//========================================================================================
//	End of File
//========================================================================================