//========================================================================================
//		File        : Scene.h
//		Program     : シーンベース
//
//		Description : ゲームシーンのベース部分
//
//		History     : 2013/12/18	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================

#pragma once

//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../System/PreInclude.h"
#
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
enum _eSceneId
{
	SID_TITLE = 0,		// タイトル
	SID_SELECT,			// 楽曲選択
	SID_GAME,			// ゲーム本編
	SID_RESULT,			// リザルト

	MAX_SCENEID,		// シーン数
};

// ----- BGM関連
enum _eBGMID
{
	BGM_TITLE = 0,		// タイトル
	BGM_SELECT,			// 楽曲選択
	BGM_RESULT,			// リザルト

	MAX_BGMID
};
const LPTSTR BGM_FILENAME[MAX_BGMID] = {
	_T("res/bgm/Title.wav"),		// タイトル
	_T("res/bgm/Select.wav"),		// 楽曲選択
	_T("res/bgm/Result.wav"),		// リザルト
};

// ----- SE関連
enum _eSEID
{
	SE_HAND_CLAP = 0,	// ハンドクラップ
	SE_SELECT,			// 選択音
	SE_MUSIC_FIX,		// 楽曲決定音

	MAX_SEID
};
const LPTSTR SE_FILENAME[MAX_SEID] = {
	_T("res/se/HandClap.wav"),	// ハンドクラップ
	_T("res/se/Select.wav"),	// 選択音
	_T("res/se/MusicFix.wav"),	// 楽曲決定音
};

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CScene
{
// ===== メンバ変数
protected:

// ===== メンバ関数
public:		 
	CScene();
	virtual ~CScene();

	virtual void Init() = 0;		// 初期化
	virtual void Uninit() = 0;		// 後始末
	virtual void Update() = 0;		// 更新
	virtual void Draw() = 0;		// 描画
	virtual void Release();			// 解放
				 
protected:		 
	//virtual bool Initialize(CMesh** ppMesh, LPDIRECTSOUNDBUFFER8 pBGM) = 0;		// 初期化
	virtual bool Initialize() = 0;		// 初期化
	virtual void Finalize() = 0;		// 後始末
};


//========================================================================================
//	End of File
//========================================================================================