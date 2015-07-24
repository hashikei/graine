//========================================================================================
//		File        : Input.cpp
//		Program     : 入力デバイスクラス
//
//		Description : 入力デバイスクラスの実装
//
//		History     : 2013/07/02	作成開始
//						   10/24	ジョイパッド関連をコメントアウトして無効化
//					  2014/08/07	シングルトンクラス化
//
//																Author : Kei Hashimoto
//========================================================================================


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include <windows.h>
#include "../../h/System/Input.h"

//――――――――――――――――――――――――――――――――――――――――――――
// マクロ定義
//――――――――――――――――――――――――――――――――――――――――――――
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)			if(x){ x->Release(); x=NULL; }
#endif

//――――――――――――――――――――――――――――――――――――――――――――
// メンバ実体宣言
//――――――――――――――――――――――――――――――――――――――――――――
// ----- メンバ定数
// private:
BYTE					CInput::m_keyState[256];					// キーボード情報配列
DIMOUSESTATE2			CInput::m_mouseState;						// マウス情報配列
DIJOYSTATE				CInput::m_joyState[MAX_CONTROLER];			// ゲームパッド情報配列

LPDIRECTINPUT8			CInput::m_pDInput;							// IDirectInput8インターフェースへのポインタ

LPDIRECTINPUTDEVICE8	CInput::m_pDIDevKeyboard;					// IDirectInputDevice8インターフェースへのポインタ(キーボード)
LPDIRECTINPUTDEVICE8	CInput::m_pDIDevMouse;						// IDirectInputDevice8インターフェースへのポインタ(マウス)
LPDIRECTINPUTDEVICE8	CInput::m_pDIDevJoypad[MAX_CONTROLER];		// IDirectInputDevice8インターフェースへのポインタ(ゲームパッド)

int						CInput::m_numCnt;							// 見つかったゲームパッドの数

//――――――――――――――――――――――――――――――――――――――――――――
// グローバル変数宣言
//――――――――――――――――――――――――――――――――――――――――――――
static CInput& g_input = CInput::GetInstance();		// 入力デバイスクラスの実体生成

static BYTE	g_prsKey[256];		// プレスキー情報配列
static BYTE	g_trgKey[256];		// トリガキー情報配列
static BYTE	g_rlsKey[256];		// リリースキー情報配列
static BYTE	g_prsClick[8];		// プレスクリック情報配列
static BYTE	g_trgClick[8];		// トリガクリック情報配列
static BYTE	g_rlsClick[8];		// リリースクリック情報配列

static POINT g_curPos;			// カーソル座標


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : インスタンス取得
//	Description : 入力デバイスクラスのインスタンスを取得する
//	Arguments   : None.
//	Returns     : 入力デバイスクラス
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CInput& CInput::GetInstance()
{
	// ----- インスタンス生成
	static CInput input;

	return input;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 入力処理の初期化
//	Description : 入力処理全般を初期化する
//	Arguments   : hInst / インスタンス
//				  hWnd  / ウィンドウハンドル
//	Returns     : 成否
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
HRESULT CInput::InitInput(HINSTANCE hInst, HWND hWnd)
{
	// ----- DirectInputオブジェクトの作成
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION,
				IID_IDirectInput8, (void **)&m_pDInput, NULL)))
		return E_FAIL;

	// ----- キーボードの初期化
	if(FAILED(InitKeyboard(hWnd)))
		return E_FAIL;
	
	// ----- マウスの初期化
	if(FAILED(InitMouse(hWnd)))
		return E_FAIL;
	
	// ----- ゲームパッドの初期化
//	if(FAILED(InitJoypad(hWnd)))
//		return E_FAIL;

	return S_OK;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 入力処理の終了処理
//	Description : 入力処理全般の後始末をし、処理を終了する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CInput::Release(void)
{
	// ----- キーボードの終了処理
	UninitKeyboard();
	
	// ----- マウスの終了処理
	UninitMouse();
	
	// ----- ジョイパッドの終了処理
//	UninitJoypad();

	// ----- オブジェクトの後始末
	SAFE_RELEASE(m_pDInput);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 入力処理の更新処理
//	Description : 入力処理全般を更新する
//	Arguments   : hWnd / ウィンドウハンドル
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CInput::UpdateInput(HWND hWnd)
{
	// ----- キーボードの更新
	UpdateKeyboard();
	
	// ----- マウスの更新
	UpdateMouse();
	
	// ----- ジョイパッドの更新
//	UpdateJoypad();

	// ----- キー入力の更新
	UpdateInputKey();

	// ----- マウス入力の更新
	UpdateClick();

	// ----- カーソルの更新
	UpdateCursor(hWnd);
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CInput::CInput()
{
	m_pDInput			= NULL;
	m_pDIDevKeyboard	= NULL;
	m_pDIDevMouse		= NULL;
	m_numCnt			= 0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CInput::~CInput()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : キーボードの初期化
//	Description : キーボードに関する情報を初期化する
//	Arguments   : hWnd / ウィンドウハンドル
//	Returns     : 成否
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
HRESULT CInput::InitKeyboard(HWND hWnd)
{
	// ----- デバイスオブジェクトを作成
	if(FAILED(m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevKeyboard, NULL)) || m_pDIDevKeyboard == NULL)
		return E_FAIL;

	// ----- データフォーマットを設定
	if(FAILED(m_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;

	// ----- 協調モードを設定(フォアグラウンド＆非排他モード)
	if(FAILED(m_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		return E_FAIL;

	// ----- キーボードのアクセス権を獲得(入力制御開始)
	m_pDIDevKeyboard->Acquire();

	return S_OK;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : キーボードの終了処理
//	Description : キーボードに関する情報の後始末をし、処理を終了する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CInput::UninitKeyboard(void)
{
	SAFE_RELEASE(m_pDIDevKeyboard);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : キーボードの更新処理
//	Description : キーボードに関する情報及び処理を更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CInput::UpdateKeyboard(void)
{
	// ----- デバイスからデータを取得。失敗したらキーボードへのアクセス権を取得
	if(FAILED(m_pDIDevKeyboard->GetDeviceState(sizeof(m_keyState), m_keyState)))
		m_pDIDevKeyboard->Acquire();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : マウスの初期化
//	Description : マウスに関する情報を初期化する
//	Arguments   : hWnd / ウィンドウハンドル
//	Returns     : 成否
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
HRESULT CInput::InitMouse(HWND hWnd)
{
	// ----- デバイスオブジェクトを作成
	if(FAILED(m_pDInput->CreateDevice(GUID_SysMouse, &m_pDIDevMouse, NULL)) || m_pDIDevMouse == NULL)
		return E_FAIL;

	// ----- データフォーマットを設定
	if(FAILED(m_pDIDevMouse->SetDataFormat(&c_dfDIMouse2)))
		return E_FAIL;

	// ----- 協調モードを設定(フォアグラウンド＆非排他モード)
	if(FAILED(m_pDIDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		return E_FAIL;

	// ----- マウスへのアクセス権を獲得(入力制御開始)
	m_pDIDevMouse->Acquire();

	return S_OK;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : マウスの終了処理
//	Description : マウスに関する情報の後始末をし、処理を終了する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CInput::UninitMouse(void)
{
	SAFE_RELEASE(m_pDIDevMouse);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : マウスの更新処理
//	Description : マウスに関する情報及び処理を更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CInput::UpdateMouse(void)
{
	// ----- デバイスからデータを取得。失敗したらマウスへのアクセス権を取得
	if(FAILED(m_pDIDevMouse->GetDeviceState(sizeof(m_mouseState), &m_mouseState)))
		m_pDIDevMouse->Acquire();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : ゲームパッドの初期化
//	Description : ゲームパッドに関する情報を初期化する
//	Arguments   : hWnd / ウィンドウハンドル
//	Returns     : 成否
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
HRESULT CInput::InitJoypad(HWND hWnd)
{
	// ----- 変数宣言
	int nLoop;

	// ----- 初期化
	for(nLoop = 0; nLoop < MAX_CONTROLER; nLoop ++)
		m_pDIDevJoypad[nLoop] = NULL;

	// ----- デバイスオブジェクトを作成(接続されているジョイパッドを列挙する)
	if(FAILED(m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoyCallback, NULL, DIEDFL_ATTACHEDONLY)))
		return E_FAIL;

	// ----- ジョイパッドの数だけ処理
	for(nLoop = 0; nLoop < MAX_CONTROLER; nLoop ++)
	{
		// ----- ジョイパッドがない場合は未処理
		if(m_pDIDevJoypad[nLoop] == NULL)
			continue;

		// ----- データフォーマットの設定
		if(FAILED(m_pDIDevJoypad[nLoop]->SetDataFormat(&c_dfDIJoystick)))
			return E_FAIL;

		// ----- 協調レベルの設定
		if(FAILED(m_pDIDevJoypad[nLoop]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
			return E_FAIL;

		// ----- デバイスへの入力制御開始
		m_pDIDevJoypad[nLoop]->Acquire();
	}

	return S_OK;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : ジョイスティック問い合わせ用コールバック関数
//	Description : 
//	Arguments   : 
//	Returns     : 
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BOOL CALLBACK CInput::EnumJoyCallback(const DIDEVICEINSTANCE *lpddi, VOID *pvRef)
{
	// ----- 変数宣言
	DIDEVCAPS	diDevCaps;		// デバイス情報

	// ----- ジョイスティック用デバイスオブジェクトを作成
	if(FAILED(m_pDInput->CreateDevice(lpddi->guidInstance, &m_pDIDevJoypad[m_numCnt], NULL)))
		return DIENUM_CONTINUE;		// 列挙を続ける

	// ----- ジョイスティックの能力を調べる
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if(FAILED(m_pDIDevJoypad[m_numCnt]->GetCapabilities(&diDevCaps)))
	{
		if(m_pDIDevJoypad[m_numCnt])
			m_pDIDevJoypad[m_numCnt]->Release();
		m_pDIDevJoypad[m_numCnt] = NULL;
		return DIENUM_CONTINUE;		// 列挙を続ける
	}

	// ----- 規定数に達したら終了
	m_numCnt ++;
	if(m_numCnt >= MAX_CONTROLER)
		return DIENUM_STOP;			// 列挙を終了する
	else
		return DIENUM_CONTINUE;		// 列挙を続ける
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : ゲームパッドの終了処理
//	Description : ゲームパッドに関する情報の後始末をし、処理を終了する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CInput::UninitJoypad(void)
{
	for(int nLoop = 0; nLoop < MAX_CONTROLER; nLoop ++)
		SAFE_RELEASE(m_pDIDevJoypad[nLoop]);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : ゲームパッドの更新処理
//	Description : ゲームパッドに関する情報及び処理を更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CInput::UpdateJoypad(void)
{
	// ----- 変数宣言
	int nLoop;

	for(nLoop = 0; nLoop < MAX_CONTROLER; nLoop ++)
	{
		if(m_pDIDevJoypad[nLoop])
		{
			// ----- デバイスからデータを取得
			if(FAILED(m_pDIDevJoypad[nLoop]->GetDeviceState(sizeof(DIJOYSTATE), &m_joyState[nLoop])))
				m_pDIDevJoypad[nLoop]->Acquire();
		}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : キー入力更新
//	Description : キー入力を更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CInput::UpdateInputKey(void)
{
	for(int nCntKey = 0; nCntKey < 256; nCntKey ++)
	{
		g_trgKey[nCntKey] = (m_keyState[nCntKey]^g_prsKey[nCntKey]) & m_keyState[nCntKey];
		g_rlsKey[nCntKey] = (m_keyState[nCntKey]^g_prsKey[nCntKey]) & g_prsKey[nCntKey];

		g_prsKey[nCntKey] = m_keyState[nCntKey];
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : マウス入力更新
//	Description : マウスによるキー入力を更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CInput::UpdateClick(void)
{
	for(int nCntClick = 0; nCntClick < 8; nCntClick ++)
	{
		g_trgClick[nCntClick] = (m_mouseState.rgbButtons[nCntClick]^g_prsClick[nCntClick]) & m_mouseState.rgbButtons[nCntClick];
		g_rlsClick[nCntClick] = (m_mouseState.rgbButtons[nCntClick]^g_prsClick[nCntClick]) & g_prsClick[nCntClick];

		g_prsClick[nCntClick] = m_mouseState.rgbButtons[nCntClick];
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : カーソル更新
//	Description : マウスカーソルの座標を更新する
//	Arguments   : hWnd / ウィンドウハンドル
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CInput::UpdateCursor(HWND hWnd)
{
	GetCursorPos(&g_curPos);
	ScreenToClient(hWnd, &g_curPos);
}


//========================================================================================
// namespace:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : プレスキー取得
//	Description : プレスキーを取得する
//	Arguments   : key / 入力キー
//	Returns     : キー状態
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BYTE Input::GetPrsKey(int key)
{
	// ----- 変数宣言
	BYTE prsKey;

	// ----- キー押下確認
	prsKey = g_prsKey[key] & 0x80;

	return prsKey;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : トリガキー取得
//	Description : トリガキーを取得する
//	Arguments   : key / 入力キー
//	Returns     : キー状態
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BYTE Input::GetTrgKey(int key)
{
	// ----- 変数宣言
	BYTE trgKey;

	// ----- キー押下確認
	trgKey = g_trgKey[key] & 0x80;

	return trgKey;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : リリースキー取得
//	Description : リリースキーを取得する
//	Arguments   : key / 入力キー
//	Returns     : キー状態
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BYTE Input::GetRlsKey(int key)
{
	// ----- 変数宣言
	BYTE rlsKey;

	// ----- キー押下確認
	rlsKey = g_rlsKey[key] & 0x80;

	return rlsKey;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : プレスクリック取得
//	Description : プレスクリックを取得する
//	Arguments   : click / クリック情報
//	Returns     : クリック状態
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BYTE Input::GetPrsClick(int click)
{
	return g_prsClick[click];
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : トリガクリック取得
//	Description : トリガクリックを取得する
//	Arguments   : click / クリック情報
//	Returns     : クリック状態
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BYTE Input::GetTrgClick(int click)
{
	return g_trgClick[click];
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : リリースクリック取得
//	Description : リリースクリックを取得する
//	Arguments   : click / クリック情報
//	Returns     : クリック状態
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
BYTE Input::GetRlsClick(int click)
{
	return g_rlsClick[click];
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : カーソル座標取得
//	Description : マウスカーソルの座標を取得する
//	Arguments   : None.
//	Returns     : カーソル座標
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
POINT& Input::GetCurPos(void)
{
	return g_curPos;
}


//========================================================================================
//	End of File
//========================================================================================