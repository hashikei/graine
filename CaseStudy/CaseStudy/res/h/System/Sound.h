//========================================================================================
//		File        : Sound.h
//		Program     : サウンドクラス
//
//		Description : サウンドクラスの定義
//
//		History     : 2013/07/04	作成開始
//					  2014/05/23	ファイル名変更
//						   08/07	シングルトンクラス化
//
//																Author : Kei Hashimoto
//========================================================================================

#pragma once
#define DIRECTSOUND_VERSION	(0x0800)

//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../System/PreInclude.h"
#
#include <dsound.h>

// ----- ライブラリのリンク
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CSound
{
// ===== メンバ定数
public:
	static const int MAX_DUP = 16;	// 最大同時発音数

// ===== メンバ変数
private:
	static LPDIRECTSOUND8	m_pDirectSound;		// DirectSoundオブジェクト

// ===== メンバ関数
public:
	static CSound& GetInstance();							// インスタンス取得
	HRESULT InitSound(HWND hWnd);							// サウンド関連初期化
	void Release(void);										// サウンド関連後始末
	static LPDIRECTSOUNDBUFFER8 LoadSound(LPTSTR file);		// サウンドのロード
	HRESULT CopyBuffer(LPDIRECTSOUNDBUFFER8 pBuffer[][MAX_DUP], int elem);	// セカンダリバッファのコピー

private:
	CSound();
	CSound(const CSound&) {}
	CSound &operator=(const CSound&) {}
	virtual ~CSound();
};


//========================================================================================
//	End of File
//========================================================================================