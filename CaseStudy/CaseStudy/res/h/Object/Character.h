//========================================================================================
//		File        : Character.h
//		Program     : キャラクタベース
//
//		Description : キャラクタベースの定義
//					  原点：ポリゴンの中央
//
//		History     : 2015/05/11	作成開始
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
#include <vector>
#include "../System/System.h"
#include "../System/Graphics.h"
#include "../System/Timer.h"
#include "../Object/Object2D.h"

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
// 状態
enum _eStatus {
	ST_NONE		= 0,	// 無
	ST_WAIT		= 1,	// 待機
	ST_MOVE		= 2,	// 移動
	ST_FLYING	= 4,	// 浮遊中
	ST_JUMP		= 8,	// ジャンプ
};

// 当たり判定
enum _eCollision2D {
	COL2D_BOUNDINGBOX = 0,		// バウンディングボックス
	COL2D_BOUNDINGCIRCLE,		// バウンディングサークル
	COL2D_SQUARECIRCLE,			// 矩形と円との当たり判定(衝突対象が円の場合)(矩形が回転していても可)
	COL2D_CIRCLESQUARE,			// 矩形と円との当たり判定(衝突対象が矩形の場合)(矩形が回転していても可)
	COL2D_SQUARESQUARE,			// 矩形同士の当たり判定(回転していても可)
	COL2D_SQUARELINE,			// 矩形と線分との当たり判定(衝突対象が線分の場合)
	COL2D_LINESQUARE,			// 矩形と線分との当たり判定(衝突対象が矩形の場合)

	MAX_COLLISION2D
};

//――――――――――――――――――――――――――――――――――――――――――――
// クラス定義
//――――――――――――――――――――――――――――――――――――――――――――
class CCharacter : public CObject2D
{
// ===== using宣言
public:
	using CObject2D::Init;

// ===== メンバ定数
private:
	static const float	DEFAULT_GRAVITY;		// 重力のデフォルト値

// ===== メンバ変数
protected:
	ULONG		m_status;		// 状態
	float		m_gravity;		// 重力
	float		m_graAccel;		// 重力加速度

	// ----- アニメーション用変数
	double		m_timeSeed;		// アニメーション開始時間

	// ----- 当たり判定用変数
	float		m_colRadius;		// 半径
	D3DXVECTOR2	m_colStartLine;		// 線分の始点
	D3DXVECTOR2	m_colEndLine;		// 線分の終点
	D3DXVECTOR2	m_lastColLinePos;	// 線分の当たり判定で最後に衝突した座標

// ===== メンバ関数
public:
	CCharacter();			// コンストラクタ
	virtual ~CCharacter();	// デストラクタ

	virtual void Init();		// 初期化
	virtual void Uninit();		// 後始末
	virtual void Update();		// 更新
	virtual void Draw();		// 描画(アルファ無効)
	virtual void DrawAlpha();	// 描画(アルファ有効)
	virtual void DrawBillBoard(const D3DXVECTOR3& target);		// ビルボード描画(アルファ無効)
	virtual void DrawBillBoardAlpha(const D3DXVECTOR3& target);	// ビルボード描画(アルファ有効)

	static CCharacter* Create(const LPCTSTR pszFName);	// 生成
	virtual void Release();								// 破棄
	
	// ----- セッター
	virtual void SetGravity(float gravity) {m_gravity = gravity;}				// 重力設定
	virtual void SetColRadius(float radius) {m_colRadius = radius;}				// 当たり判定用半径設定
	virtual void SetColStartLine(D3DXVECTOR2 line) {m_colStartLine = line;}		// 当たり判定用線分の始点設定
	virtual void SetColEndLine(D3DXVECTOR2 line) {m_colEndLine = line;}			// 当たり判定用線分の終点設定

	// ----- ゲッター
	virtual int GetStatus() const {return m_status;}							// 状態取得
	virtual float GetGravity() const {return m_gravity;}						// 重力取得
	virtual float GetColRadius() const {return m_colRadius;}					// 当たり判定用半径取得
	virtual D3DXVECTOR2 GetColStartLine() const {return m_colStartLine;}		// 当たり判定用線分の始点取得
	virtual D3DXVECTOR2 GetColEndLine() const {return m_colEndLine;}			// 当たり判定用線分の終点取得
	virtual D3DXVECTOR2 GetLastColLinePos() const {return m_lastColLinePos;}	// 線分の当たり判定で最後に衝突した座標を取得

	// ----- 状態操作関連
	virtual int CheckStatus(int status) {return m_status & status;}		// 状態チェック
	virtual void AddStatus(int status) {m_status |= status;}			// 状態を追加
	virtual void SubStatus(int status) {m_status ^= status;}			// 状態を削減
	virtual void ClearStatus() {m_status = ST_NONE;}					// 状態を無状態に初期化

	// ----- フレームアニメーション関連
	virtual void StartAnimation();		// フレームアニメーション開始準備
	virtual void FrameAnimation(int start, int end, int width, int height, double time);	// フレームアニメーション(秒指定でアニメーション)

	// ----- 当たり判定関連
	virtual bool CollisionEnter(int id, const CCharacter* pCol);	// 当たった瞬間
	virtual bool CollisionStay(int id, const CCharacter* pCol);		// 当たっている間
	virtual bool CollisionExit(int id, const CCharacter* pCol);		// 離れた瞬間
	
	virtual void Resize(const D3DXVECTOR2& size);	// オブジェクトサイズ変更
	virtual void Resize(const float width, const float height);	// オブジェクトサイズ変更

protected:
	virtual bool Initialize(const LPCTSTR pszFName);	// 初期化
	virtual void Finalize();							// 後始末

	// ----- 当たり判定
	virtual bool BoundingBox(const CCharacter* pCol);			// バウンディングボックス
	virtual bool BoundingCircle(const CCharacter* pCol);		// バウンディングサークル
	virtual bool JudgeSquareCircle(const CCharacter* pCol);		// 矩形と円との当たり判定(衝突対象が円の場合)(矩形が回転していても可)
	virtual bool JudgeCircleSquare(const CCharacter* pCol);		// 矩形と円との当たり判定(衝突対象が矩形の場合)(矩形が回転していても可)
	virtual bool JudgeSquareSquare(const CCharacter* pCol);		// 矩形同士の当たり判定(回転していても可)
	virtual bool JudgeSquareLine(const CCharacter* pCol);		// 矩形と線分との当たり判定(衝突対象が線分の場合)
	virtual bool JudgeLineSquare(const CCharacter* pCol);		// 矩形と線分との当たり判定(衝突対象が矩形の場合)
};

//――――――――――――――――――――――――――――――――――――――――――――
// ユーザ型定義
//――――――――――――――――――――――――――――――――――――――――――――
typedef std::vector<CCharacter*>	LPCHARACTER_ARRAY;		// キャラクタクラスのリスト
typedef LPCHARACTER_ARRAY::iterator	LPCHARACTER_ARRAY_IT;	// キャラクタクラスリストのイテレータ


//========================================================================================
//	End of File
//========================================================================================