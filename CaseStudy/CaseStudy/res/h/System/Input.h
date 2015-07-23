//========================================================================================
//		File        : Input.h
//		Program     : 入力デバイスクラス
//
//		Description : 入力デバイスクラスの定義
//
//		History     : 2013/07/02	作成開始
//						   10/24	ジョイパッド関連をコメントアウトして無効化
//					  2014/08/07	シングルトンクラス化
//
//																Author : Kei Hashimoto
//========================================================================================

#pragma once

//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../System/PreInclude.h"
#
#include <dinput.h>

// ----- ライブラリのリンク
#pragma comment(lib, "dinput8.lib")

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
#define MAX_CONTROLER	(2)		// コントローラ最大数

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CInput
{
// ===== メンバ変数
private:
	BYTE					m_keyState[256];					// キーボード情報配列
	DIMOUSESTATE2			m_mouseState;						// マウス情報配列
	DIJOYSTATE				m_joyState[MAX_CONTROLER];			// ゲームパッド情報配列

	LPDIRECTINPUT8			m_pDInput;							// IDirectInput8インターフェースへのポインタ

	LPDIRECTINPUTDEVICE8	m_pDIDevKeyboard;					// IDirectInputDevice8インターフェースへのポインタ(キーボード)
	LPDIRECTINPUTDEVICE8	m_pDIDevMouse;						// IDirectInputDevice8インターフェースへのポインタ(マウス)
	LPDIRECTINPUTDEVICE8	m_pDIDevJoypad[MAX_CONTROLER];		// IDirectInputDevice8インターフェースへのポインタ(ゲームパッド)

	int						m_numCnt;							// 見つかったゲームパッドの数

// ===== メンバ関数
public:
	static CInput& GetInstance();						// インスタンス取得
	HRESULT	InitInput(HINSTANCE hInst, HWND hWnd);		// Input初期化
	void	Release(void);								// Input後始末
	void	UpdateInput(HWND hWnd);						// 入力情報更新

private:
	CInput();
	CInput(const CInput&) {}
	CInput &operator=(const CInput&) {}
	virtual ~CInput();

	// キーボード関連
	HRESULT InitKeyboard(HWND hWnd);
	void UninitKeyboard(void);
	void UpdateKeyboard(void);
	void UpdateInputKey(void);

	// マウス関連
	HRESULT InitMouse(HWND hWnd);
	void UninitMouse(void);
	void UpdateMouse(void);
	void UpdateClick(void);
	void UpdateCursor(HWND hWnd);

	// ゲームパッド関連
	HRESULT InitJoypad(HWND hWnd);
	void UninitJoypad(void);
	void UpdateJoypad(void);
	BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE *lpddi, VOID *pvRef);
};

//――――――――――――――――――――――――――――――――――――――――――――
// 名前空間定義
//――――――――――――――――――――――――――――――――――――――――――――
namespace Input
{
	BYTE	GetPrsKey(int key);			// プレスキー取得
	BYTE	GetTrgKey(int key);			// トリガキー取得
	BYTE	GetRlsKey(int key);			// リリースキー取得
	BYTE	GetPrsClick(int click);		// プレスクリック取得
	BYTE	GetTrgClick(int click);		// トリガクリック取得
	BYTE	GetRlsClick(int click);		// リリースクリック取得

	POINT&	GetCurPos(void);			// カーソル座標取得
}


//========================================================================================
//	End of File
//========================================================================================