//========================================================================================
//		File        : Timer.h
//		Program     : タイマー処理クラス
//
//		Description : タイマー処理クラスの定義
//
//		History     : 2014/08/03	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================

#pragma once

//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../System/PreInclude.h"
#
#include <windows.h>

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CTimer
{
// ===== メンバ定数
protected:
	static const int DEFAULT_FPS = 60;		// デフォルトFPS

// ===== メンバ変数
protected:
	static bool		m_bPerform;		// 高解像度タイマーがサポートされているか
	static LONGLONG	m_sysFreq;		// システム周波数
	static LONGLONG	m_startQuad;	// 開始周波数
	static int		m_fps;			// フレームレート
	static int		m_count;		// 経過フレーム数
	static double	m_time;			// 経過時間(秒)
	static double	m_startTime;	// 開始時間(秒)
	static double	m_frameTime;	// 経過フレームの経過時間(秒)
	
// ===== メンバ関数
public:
	static CTimer& GetInstance();	// インスタンス取得

	bool	Start(int fps = DEFAULT_FPS);	// 割り込みスタート
	int		Run();							// フレーム数呼び出し

	// ----- ゲッター
	static double	GetFps() {return (double)m_count / m_time;}	// フレーム数取得
	static double	GetTime() {return m_time;}					// ゲーム開始時からの経過時間を取得(秒)
	static double	GetStartTime() {return m_startTime;}		// 開始時間を取得(秒)
	static double	GetFrameTime() {return m_frameTime;}		// 経過フレームの経過時間取得(秒)
	
private:
	CTimer();
	CTimer(const CTimer&) {}
	CTimer &operator=(const CTimer&) {}
	virtual ~CTimer();
};


//========================================================================================
//	End of File
//========================================================================================