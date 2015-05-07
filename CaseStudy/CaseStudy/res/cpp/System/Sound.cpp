//========================================================================================
//		File        : Sound.cpp
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


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include "../../h/System/Sound.h"

//――――――――――――――――――――――――――――――――――――――――――――
// マクロ定義
//――――――――――――――――――――――――――――――――――――――――――――
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)			if(x){ x->Release(); x=NULL; }
#endif

//――――――――――――――――――――――――――――――――――――――――――――
// メンバ実体宣言
//――――――――――――――――――――――――――――――――――――――――――――
LPDIRECTSOUND8	CSound::m_pDirectSound;

//――――――――――――――――――――――――――――――――――――――――――――
// グローバル変数宣言
//――――――――――――――――――――――――――――――――――――――――――――
static CSound& g_sound = CSound::GetInstance();		// サウンドクラスの実体生成


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : インスタンス取得
//	Description : サウンドクラスのインスタンスを取得する
//	Arguments   : None.
//	Returns     : サウンドクラス
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CSound& CSound::GetInstance()
{
	// ----- インスタンス生成
	static CSound sound;

	return sound;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : サウンド処理の初期化
//	Description : サウンド処理を初期化する
//	Arguments   : hWnd / ウィンドウハンドル
//	Returns     : 成否
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
HRESULT CSound::InitSound(HWND hWnd)
{
	// ----- DirectSoundオブジェクトの作成
	if(FAILED(DirectSoundCreate8(NULL, &m_pDirectSound, NULL)))
		return E_FAIL;

	// ----- 協調レベル設定
	if(FAILED(m_pDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
		return E_FAIL;

	return S_OK;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : サウンドの解放
//	Description : サウンドを解放する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CSound::Release(void)
{
	SAFE_RELEASE(m_pDirectSound);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : サウンドのロード
//	Description : サウンドをロードする
//	Arguments   : file / サウンドデータのファイル名
//	Returns     : セカンダリバッファ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
LPDIRECTSOUNDBUFFER8 CSound::LoadSound(LPTSTR file)
{
	// ----- MMIO(マルチメディア入出力)
	LPDIRECTSOUNDBUFFER		pBaseBuffer = NULL;		// 曲データの総合バッファ
	LPDIRECTSOUNDBUFFER8	pBuffer = NULL;			// 曲データのバッファ
	DSBUFFERDESC buff;								// バッファ設定構造体

	HMMIO hMmio = NULL;								// MMIOハンドル
	MMIOINFO mmioInfo;								// マルチメディアデータ構造体

	MMRESULT mmRes;									// MM処理結果格納変数
	MMCKINFO riffChunk, formatChunk, dataChunk;		// チャンクデータ構造体
	DWORD size;										// データサイズ格納用
	WAVEFORMATEX pcm;								// 曲データフォーマット構造体

	LPVOID	pBlock;		// セカンダリバッファのデータ書き込み先アドレス
	DWORD	dwSize;		// セカンダリバッファのサイズ

	// ----- １．ハンドルをもとにファイルを開く
	memset(&mmioInfo, 0, sizeof(MMIOINFO));
	hMmio = mmioOpen(file, &mmioInfo, MMIO_READ);
	if(!hMmio)
		return NULL;

	// ----- ２．ファイル解析① RIFFチャンク検索
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');					// 検索キーワード
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);		// キーワードをもとに検索

	if(mmRes != MMSYSERR_NOERROR)		// 見つからなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// ----- ３．ファイル解析② フォーマットチャンク検索
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');							// 検索キーワード
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);		// キーワードをもとに検索

	if(mmRes != MMSYSERR_NOERROR)		// 見つからなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	size = mmioRead(hMmio, (HPSTR)&pcm, formatChunk.cksize);		// 検索情報をもとに読み込み

	if(size != formatChunk.cksize)		// 正しく読み込めなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	mmioAscend(hMmio, &formatChunk, 0);

	// ----- ４．ファイル解析③ データチャンク検索
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');						// 検索キーワード
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);		// キーワードをもとに検索

	if(mmRes != MMSYSERR_NOERROR)		// 見つからなかったら異常終了
	{
		mmioClose(hMmio, 0);
		return NULL;
	}

	// ----- ５．データ読込
	char *pData = new char[dataChunk.cksize];					// 必要な大きさの領域を確保
	size = mmioRead(hMmio, (HPSTR)pData, dataChunk.cksize);		// データを読み込む

	if(size != dataChunk.cksize)		// 正しく読み込めなかったら異常終了
	{
		delete[] pData;
		return NULL;
	}

	// ----- ６．曲を読み込む「セカンダリバッファ」を用意
	ZeroMemory(&buff, sizeof(DSBUFFERDESC));		// 初期化
	buff.dwSize = sizeof(DSBUFFERDESC);				// 各種設定
	buff.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER;
	buff.dwBufferBytes = size;
	buff.lpwfxFormat = &pcm;

	// ----- 総合バッファ作成
	if(FAILED(m_pDirectSound->CreateSoundBuffer(&buff, &pBaseBuffer, NULL)))
		return NULL;

	// ----- サウンドバッファを取り出す
	if(FAILED(pBaseBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID *)&pBuffer)))
		return NULL;

	// ----- 総合バッファ破棄
	pBaseBuffer->Release();

	// ----- ７．セカンダリバッファにデータを転送
	// ----- バッファをロック
	if(FAILED(pBuffer->Lock(0, 0, &pBlock, &dwSize, NULL, NULL, DSBLOCK_ENTIREBUFFER)))
		return NULL;

	memcpy(pBlock, pData, dwSize);		// データ転送
	delete[] pData;						// 元の読み込み領域破棄

	// ----- バッファロック解除
	pBuffer->Unlock(pBlock, dwSize, 0, 0);

	// ----- セカンダリバッファを返す
	return pBuffer;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : セカンダリバッファのコピー
//	Description : セカンダリバッファをMAX_DUP分コピーする
//	Arguments   : pBuffer / セカンダリバッファリスト
//				  elem    / 配列の要素数(一列目の数)
//	Returns     : 処理結果
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
HRESULT CSound::CopyBuffer(LPDIRECTSOUNDBUFFER8 pBuffer[][MAX_DUP], int elem)
{
	// ----- 変数宣言
	HRESULT hr;		// 処理結果

	// ----- コピー処理
	for(int i = 0; i < elem; ++i)
	{
		for(int j = 1; j < MAX_DUP; ++j)
		{
			hr = m_pDirectSound->DuplicateSoundBuffer(pBuffer[i][0], (LPDIRECTSOUNDBUFFER*)&pBuffer[i][j]);
			if(FAILED(hr))
				return hr;
		}
	}

	return hr;
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CSound::CSound()
{
	m_pDirectSound = NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CSound::~CSound()
{
}


//========================================================================================
//	End of File
//========================================================================================