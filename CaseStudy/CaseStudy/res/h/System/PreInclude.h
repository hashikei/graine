//========================================================================================
//		File        : PreInclude.h
//		Program     : 事前インクルードライブラリ
//
//		Description : あらかじめインクルードするライブラリ
//
//		History     : 2014/07/17	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================

#pragma once

//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
// ----- エラーメッセージボックスを利用
//#ifndef _DEBUG_MESSAGEBOX
//#define _DEBUG_MESSAGEBOX
//#endif

// ----- 高解像度タイマーを利用
#ifndef __OPT_HIGH_TIMER
#define __OPT_HIGH_TIMER
#endif

// ----- ソケット通信を利用
//#ifndef __OPT_WINSOCK
//#define __OPT_WINSOCK
//#endif

#ifdef __OPT_WINSOCK
#include <WinSock2.h>

// ----- ライブラリのリンク
#pragma comment(lib, "ws2_32")
#endif

// ----- math.hのマクロを利用
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif


//========================================================================================
//	End of File
//========================================================================================