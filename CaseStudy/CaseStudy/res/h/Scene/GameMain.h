//========================================================================================
//		File        : GameMain.h
//		Program     : ゲームのメインクラス
//
//		Description : ゲームのメインクラスの定義
//
//		History     : 2014/04/17	作成開始
//						   05/23	ファイル名変更
//
//																Author : Kei Hashimoto
//========================================================================================

#pragma once

//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../System/PreInclude.h"
#
#include "../System/Graphics.h"
#include "../System/Sound.h"
#include "../System/ChangeScene.h"
#include "../System/MapData.h"
#include "../Scene/Scene.h"

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CGameMain
{
// ===== メンバ変数
private:
	static CGraphics*	m_pGraph;			// グラフィックデバイス
	static CSound*		m_pSound;			// サウンドデバイス

	static TCHAR	m_szDebug[];			// デバッグ用文字列
	static double	m_fps;					// フレーム数カウント用

	// ----- ゲーム用オブジェクト
	static CScene*	m_sceneList[];			// 全シーンのリスト(SCENE_IDにて種別)
	static bool		m_bEnd;					// ゲーム終了フラグ

	static LPDIRECTSOUNDBUFFER8	m_pBGM[];	// BGMリスト
	static LPDIRECTSOUNDBUFFER8	m_pSE[][CSound::MAX_DUP];	// SEリスト
	static int		m_lastPlaySE[];		// 直近に再生したSE番号

	static CScene*	m_pScene;			// 現在のシーン
	static int		m_curSceneID;		// 現在のシーンID

	static CChangeScene*	m_pChangeScene;		// シーン遷移システム
	static CMapData*		m_pMapData;			// マップデータ
	
// ===== メンバ関数
public:
	CGameMain();
	virtual ~CGameMain();

	static CGameMain* Create(CGraphics*, CSound*);
	void Release();
	void Render();
	void Update();
	void SetFPS(double fps) {m_fps = fps;}
	static void AddDebugStr(LPCTSTR psz) {lstrcat(m_szDebug, psz);}	// デバッグ用文字列セット

	// ----- セッター
	static void SetScene(int id);				// シーン切り替え
	
	bool GetEndFlg(void) {return m_bEnd;}			// ゲーム終了フラグ取得
	static void	GameEnd(void) {m_bEnd = true;}		// ゲーム終了

	// ----- サウンド関連
	static HRESULT	PlayBGM(int id, int loop = 0, int priority = 0);	// BGM再生
	static HRESULT	PlaySE(int id, int loop = 0, int priority = 0);		// SE再生
	static void		StopBGM(int id);									// BGM停止
	static void		StopAllSE();										// SE停止

private:
	bool Initialize(CGraphics*, CSound*);
	void Finalize();
	void Draw();
};


//========================================================================================
//	End of File
//========================================================================================