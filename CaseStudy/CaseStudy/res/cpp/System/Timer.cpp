//========================================================================================
//		File        : Timer.cpp
//		Program     : タイマー処理クラス
//
//		Description : タイマー処理クラスの実装
//
//		History     : 2014/08/03	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include "../../h/System/Timer.h"

//――――――――――――――――――――――――――――――――――――――――――――
// マクロ定義
//――――――――――――――――――――――――――――――――――――――――――――
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if(x){ delete x; x=NULL; }
#endif

//――――――――――――――――――――――――――――――――――――――――――――
// メンバ実体宣言
//――――――――――――――――――――――――――――――――――――――――――――
bool		CTimer::m_bPerform;		// 高解像度タイマーがサポートされているか
LONGLONG	CTimer::m_sysFreq;		// システム周波数
LONGLONG	CTimer::m_startQuad;	// 開始周波数
int			CTimer::m_fps;			// フレームレート
int			CTimer::m_count;		// 経過フレーム数
double		CTimer::m_time;			// 経過時間(秒)
double		CTimer::m_startTime;	// 開始時間(秒)
double		CTimer::m_frameTime;	// 経過フレームの経過時間(秒)

//――――――――――――――――――――――――――――――――――――――――――――
// グローバル変数宣言
//――――――――――――――――――――――――――――――――――――――――――――
static CTimer& g_timer = CTimer::GetInstance();		// タイマー処理クラスの実体生成


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : インスタンス取得
//	Description : タイマー処理クラスのインスタンスを取得する
//	Arguments   : None.
//	Returns     : タイマー処理クラス
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CTimer& CTimer::GetInstance()
{
	// ----- インスタンス生成
	static CTimer timer;

	return timer;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 割り込みスタート
//	Description : 任意のFPSで割り込みをスタートする
//	Arguments   : None.
//	Returns     : 成否(true:成功)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CTimer::Start(int fps)
{
	// ----- 事前準備
	m_fps = fps;

	// ----- 開始時間を設定
	if(m_bPerform)
	{	// 高解像度タイマーに対応
		LARGE_INTEGER freq;		// システム周波数

		// 現在のカウント数を取得
		if(!QueryPerformanceCounter(&freq))
			return false;
		m_startQuad = freq.QuadPart;	// 開始時間を設定
	}
	else
	{	// 高解像度タイマーに非対応
		m_startQuad = (LONGLONG)timeGetTime();	// 低解像度タイマーとして開始時間を設定
	}

	// ----- 開始準備
	m_startTime	= m_startQuad / (double)m_sysFreq;	// 開始時間を設定
	m_count		= 0;								// 経過フレーム数を初期化
	m_time		= 0;								// 経過時間を初期化

	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : フレーム数呼び出し
//	Description : 前回呼び出し時からの経過フレームを返す
//	Arguments   : None.
//	Returns     : 経過フレーム数
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
int CTimer::Run()
{
	// ----- 変数宣言
	int		count	= 0;		// カウント数
	double	time	= 0.0;		// 経過時間

	// ----- スタート時からの経過フレームを取得
	if(m_bPerform)
	{	// 高解像度タイマーに対応
		LARGE_INTEGER freq;		// システム周波数

		// 現在のカウント数を取得
		if(!QueryPerformanceCounter(&freq))
			return -1;	// カウント数の取得に失敗
		time  = (double)(freq.QuadPart - m_startQuad) / (double)m_sysFreq;		// 経過時間を演算
		count = (int)((freq.QuadPart - m_startQuad) / (m_sysFreq / m_fps));		// 経過フレーム数を演算
	}
	else
	{	// 高解像度タイマーに非対応
		m_time = (double)(((LONGLONG)timeGetTime() - m_startQuad) / m_sysFreq);			// 低解像度タイマーとして経過時間を演算
		count  = (int)(((LONGLONG)timeGetTime() - m_startQuad) / (m_sysFreq / m_fps));	// 低解像度タイマーとして経過フレーム数を演算
	}

	// ----- 前回呼び出し時からの経過フレームを取得
	if(count != m_count)
	{
		int proc	= count - m_count;		// 経過フレーム数を演算
		m_count		= count;				// 経過フレーム数を保存
		m_frameTime	= time - m_time;		// 経過フレームの経過時間を演算
		m_time		= time;					// 経過時間を保存

		return proc;	// 経過フレーム数
	}

	return 0;
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CTimer::CTimer()
{
	// ----- ゼロクリア
	m_bPerform	= false;
	m_sysFreq	= 0;
	m_startQuad	= 0;
	m_fps		= 0;
	m_count		= 0;
	m_time		= 0.0;
	m_frameTime	= 0.0;
	
	// ----- 変数宣言
	LARGE_INTEGER freq;		// システム周波数

	// ----- システム周波数取得
	if(QueryPerformanceFrequency(&freq))
	{	// 高解像度タイマーに対応
		m_sysFreq	= freq.QuadPart;	// システム周波数を設定
		m_bPerform	= true;
	}
	else
	{	// 高解像度タイマーに非対応
		m_sysFreq = (LONGLONG)1000;		// 低解像度タイマーの使用設定(timeGetTime()で代用)
	}

	// ----- 開始準備
	m_fps = DEFAULT_FPS;		// フレームレートを設定
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CTimer::~CTimer()
{
}


//========================================================================================
//	End of File
//========================================================================================