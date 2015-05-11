//========================================================================================
//		File        : Character.cpp
//		Program     : キャラクタベース
//
//		Description : キャラクタベースの実装
//					  原点：ポリゴンの中央
//
//		History     : 2015/05/11	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include <math.h>
#include "../../h/System/System.h"
#include "../../h/Object/Character.h"

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
const float	CCharacter::DEFAULT_GRAVITY	= 9.8f;		// 重力のデフォルト値


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CCharacter::CCharacter()
{
	m_status	= ST_NONE;
	m_gravity	= 0.0f;
	m_graAccel	= 0.0f;

	m_timeSeed	= 0.0;

	m_colRadius		= 0.0f;
	m_colStartLine	= D3DXVECTOR2(0.0f, 0.0f);
	m_colEndLine	= D3DXVECTOR2(0.0f, 0.0f);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CCharacter::~CCharacter()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : キャラクタデータをデフォルト値で初期化する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCharacter::Init()
{
	// ----- 頂点データ初期化
	CObject2D::Init();

	// ----- 当たり判定用パラメータ設定
	m_colRadius = sqrt((m_vtx[1].vtx.x * m_vtx[1].vtx.x) + (m_vtx[1].vtx.y * m_vtx[1].vtx.y));	// 半径

	m_status	= ST_NONE;
	m_gravity	= DEFAULT_GRAVITY;
	m_graAccel	= 0.0f;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : キャラクタデータを初期化する
//	Arguments   : size / キャラクタサイズ
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCharacter::Init(const D3DXVECTOR2& size)
{
	// ----- 頂点データ初期化
	CCharacter::Init();
	CObject2D::Init(size);

	// ----- 当たり判定用パラメータ設定
	m_colRadius = sqrt((m_vtx[1].vtx.x * m_vtx[1].vtx.x) + (m_vtx[1].vtx.y * m_vtx[1].vtx.y));	// 半径
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : キャラクタを初期化する
//	Arguments   : width  / キャラクタ幅
//				  height / キャラクタ高さ
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCharacter::Init(const float width, const float height)
{
	CCharacter::Init(D3DXVECTOR2(width, height));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : キャラクタを初期化する
//	Arguments   : size / キャラクタサイズ
//				  pos  / 出現位置(キャラクタの中央)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCharacter::Init(const D3DXVECTOR2& size, const D3DXVECTOR3& pos)
{
	// ----- 頂点データ初期化
	CCharacter::Init(size);

	// ----- 描画位置設定
	Translate(pos);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : キャラクタを初期化する
//	Arguments   : width  / キャラクタ幅
//				  height / キャラクタ高さ
//				  x      / 出現位置(X座標)
//				  y      / 出現位置(Y座標)
//				  z      / 出現位置(Z座標)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCharacter::Init(const float width, const float height, const float x, const float y, const float z)
{
	CCharacter::Init(D3DXVECTOR2(width, height), D3DXVECTOR3(x, y, z));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : キャラクタデータの後始末をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCharacter::Uninit(void)
{
	CObject2D::Uninit();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : キャラクタを更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCharacter::Update(void)
{
	CObject2D::Update();

	// ----- 重力処理
	if(CheckStatus(ST_FLYING)) {
		m_graAccel += m_gravity;	// 落下速度を徐々に上げていく
		TranslationY(-m_graAccel);	// 落下処理
	} else {
		// 重力加速度を初期化
		if(m_graAccel > 0.0f)
			m_graAccel = 0.0f;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : キャラクタを透過無しで描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCharacter::Draw()
{
	CObject2D::Draw();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : キャラクタを透過有りで描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCharacter::DrawAlpha()
{
	CObject2D::DrawAlpha();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : ビルボード描画
//	Description : キャラクタを透過無しでビルボードとして描画する
//	Arguments   : target / 対象の座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCharacter::DrawBillBoard(const D3DXVECTOR3& target)
{
	CObject2D::DrawBillBoard(target);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : ビルボード描画
//	Description : キャラクタを透過有りでビルボードとして描画する
//	Arguments   : target / 対象の座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCharacter::DrawBillBoardAlpha(const D3DXVECTOR3& target)
{
	CObject2D::DrawBillBoardAlpha(target);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成
//	Description : キャラクタを生成する
//	Arguments   : pszFName / 読み込みファイル名
//	Returns     : キャラクタデータ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CCharacter* CCharacter::Create(const LPCTSTR pszFName)
{
	// ----- 変数宣言
	CCharacter* pChara;

	// ----- 初期化処理
	pChara = new CCharacter();
	if(pChara)
	{
		if(!pChara->Initialize(pszFName))
		{
			SAFE_DELETE(pChara);
		}
	}

	return pChara;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 解放処理
//	Description : キャラクタデータを解放する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCharacter::Release()
{
	CObject2D::Release();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : フレームアニメーション開始準備
//	Description : フレームアニメーション開始時間を設定する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCharacter::StartAnimation()
{
	m_timeSeed = CTimer::GetTime();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : フレームアニメーション
//	Description : アニメーションするコマ番号と、秒を指定して、フレームアニメーションを行う
//	Arguments   : start  / 開始コマの参照番号
//				  end    / 最終コマの参照番号
//				  width  / 横分割数
//				  height / 縦分割数
//				  time   / コマ送り間隔(秒)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCharacter::FrameAnimation(int start, int end, int width, int height, double time)
{
	double	subTime		= CTimer::GetTime() - m_timeSeed;					// アニメーション経過時間
	int		maxSeg		= end - start + 1;									// アニメーションコマ数
	int		segment		= ((int)floor(subTime / time) % maxSeg) + start;	// 描画コマ番号

	UVDivision(segment, width, height);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 当たり判定
//	Description : 衝突対象の2Dキャラクタとの当たり判定を行う(当たった瞬間のみ判定)
//	Arguments   : id   / 当たり判定ID
//				  pCol / 衝突対象キャラクタ
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CCharacter::CollisionEnter(int id, const CCharacter* pCol)
{
	// ----- 事前準備
	static bool trg = false;	// トリガー判定用
	bool ret = false;			// 判定結果

	// ----- 当たり判定
	switch(id)
	{
		// バウンディングボックス
		case COL2D_BOUNDINGBOX:
			ret = BoundingBox(pCol);
			break;
			
		// バウンディングサークル
		case COL2D_BOUNDINGCIRCLE:
			ret = BoundingCircle(pCol);
			break;

		// 矩形と円との当たり判定(衝突対象が円の場合)
		case COL2D_SQUARECIRCLE:
			ret = JudgeSquareCircle(pCol);
			break;

		// 矩形と円との当たり判定(衝突対象が矩形の場合)
		case COL2D_CIRCLESQUARE:
			ret = JudgeCircleSquare(pCol);
			break;

		// 矩形同士の当たり判定(回転していても可)
		case COL2D_SQUARESQUARE:
			ret = JudgeSquareSquare(pCol);
			break;

		// 矩形と線分との当たり判定(衝突対象が線分の場合)
		case COL2D_SQUARELINE:
			ret = JudgeSquareLine(pCol);
			break;

		// 矩形と線分との当たり判定(衝突対象が矩形の場合)
		case COL2D_LINESQUARE:
			ret = JudgeLineSquare(pCol);
			break;

		default:
			break;
	}

	// ----- トリガー判定
	if(!trg && ret) {
		trg = true;
		ret = true;
	} else {
		if(trg && !ret) {
			trg = false;
		}
		ret = false;
	}

	return ret;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 当たり判定
//	Description : 衝突対象の2Dキャラクタとの当たり判定を行う(当たっている間のみ判定)
//	Arguments   : id   / 当たり判定ID
//				  pCol / 衝突対象キャラクタ
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CCharacter::CollisionStay(int id, const CCharacter* pCol)
{
	// ----- 事前準備
	bool ret = false;		// 判定結果

	// ----- 当たり判定
	switch(id)
	{
		// バウンディングボックス
		case COL2D_BOUNDINGBOX:
			ret = BoundingBox(pCol);
			break;
			
		// バウンディングサークル
		case COL2D_BOUNDINGCIRCLE:
			ret = BoundingCircle(pCol);
			break;
			
		// 矩形と円との当たり判定(判定対象が円の場合)
		case COL2D_SQUARECIRCLE:
			ret = JudgeSquareCircle(pCol);
			break;

		// 矩形と円との当たり判定(判定対象が矩形の場合)
		case COL2D_CIRCLESQUARE:
			ret = JudgeCircleSquare(pCol);
			break;

		// 矩形同士の当たり判定(回転していても可)
		case COL2D_SQUARESQUARE:
			ret = JudgeSquareSquare(pCol);
			break;

		// 矩形と線分との当たり判定(衝突対象が線分の場合)
		case COL2D_SQUARELINE:
			ret = JudgeSquareLine(pCol);
			break;

		// 矩形と線分との当たり判定(衝突対象が矩形の場合)
		case COL2D_LINESQUARE:
			ret = JudgeLineSquare(pCol);
			break;

		default:
			break;
	}

	return ret;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 当たり判定
//	Description : 衝突対象の2Dキャラクタとの当たり判定を行う(離れた瞬間のみ判定)
//	Arguments   : id   / 当たり判定ID
//				  pCol / 衝突対象キャラクタ
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CCharacter::CollisionExit(int id, const CCharacter* pCol)
{
	// ----- 事前準備
	static bool rls = false;	// リリース判定用
	bool ret = false;			// 判定結果

	// ----- 当たり判定
	switch(id)
	{
		// バウンディングボックス
		case COL2D_BOUNDINGBOX:
			ret = BoundingBox(pCol);
			break;
			
		// バウンディングサークル
		case COL2D_BOUNDINGCIRCLE:
			ret = BoundingCircle(pCol);
			break;

		// 矩形と円との当たり判定(判定対象が円の場合)
		case COL2D_SQUARECIRCLE:
			ret = JudgeSquareCircle(pCol);
			break;

		// 矩形と円との当たり判定(判定対象が矩形の場合)
		case COL2D_CIRCLESQUARE:
			ret = JudgeCircleSquare(pCol);
			break;

		// 矩形同士の当たり判定(回転していても可)
		case COL2D_SQUARESQUARE:
			ret = JudgeSquareSquare(pCol);
			break;

		// 矩形と線分との当たり判定(衝突対象が線分の場合)
		case COL2D_SQUARELINE:
			ret = JudgeSquareLine(pCol);
			break;

		// 矩形と線分との当たり判定(衝突対象が矩形の場合)
		case COL2D_LINESQUARE:
			ret = JudgeLineSquare(pCol);
			break;

		default:
			break;
	}

	// ----- リリース判定
	if(!rls && ret) {
		rls = true;
		ret = false;
	} else {
		if(rls && !ret) {
			rls = false;
			return true;
		}
		ret = false;
	}

	return ret;
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : キャラクタデータを初期化する
//	Arguments   : pszFName / 読み込みファイル名
//	Returns     : 成否
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CCharacter::Initialize(const LPCTSTR pszFName)
{
	// ----- キャラクタ初期化
	if(!CObject2D::Initialize(pszFName))
		return false;
	
	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : キャラクタデータの後始末をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CCharacter::Finalize(void)
{
	CObject2D::Finalize();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 当たり判定
//	Description : 衝突対象の2Dキャラクタとバウンディングボックスによる当たり判定を行う
//	Arguments   : pCol / 衝突対象キャラクタ
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CCharacter::BoundingBox(const CCharacter* pCol)
{
	if(	this->GetLeftPos()	 < pCol->GetRightPos()	&&
		this->GetRightPos()	 > pCol->GetLeftPos()	&&
		this->GetTopPos()	 > pCol->GetBottomPos()	&&
		this->GetBottomPos() < pCol->GetTopPos())
		return true;

	return false;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 当たり判定
//	Description : 衝突対象の2Dキャラクタとバウンディングサークルによる当たり判定を行う
//	Arguments   : pCol / 衝突対象キャラクタ
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CCharacter::BoundingCircle(const CCharacter* pCol)
{
	float dist	= (pCol->GetPosX() - this->GetPosX()) * (pCol->GetPosX() - this->GetPosX()) + (pCol->GetPosY() - this->GetPosY()) * (pCol->GetPosY() - this->GetPosY());	// キャラクタ間の距離
	float r		= m_colRadius + pCol->GetColRadius();		// キャラクタ間の衝突距離
	if(dist < (r * r))
		return true;

	return false;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 当たり判定
//	Description : 衝突対象の2Dキャラクタと、矩形と円との当たり判定を行う(衝突対象が円の場合)
//				  矩形が回転していても可
//	Arguments   : pCol / 衝突対象キャラクタ
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CCharacter::JudgeSquareCircle(const CCharacter* pCol)
{
	// ----- 行列演算済み頂点を算出
	D3DXVECTOR3 vtx[4] = {m_vtx[1].vtx, m_vtx[0].vtx, m_vtx[2].vtx, m_vtx[3].vtx};
	float vtxX[4];
	float vtxY[4];
	for(int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&vtx[i], &vtx[i], &m_world);
		vtxX[i] = vtx[i].x;
		vtxY[i] = vtx[i].y;
	}

	// ----- 矩形の4頂点と円との当たり判定
	for(int i = 0; i < 4; ++i) {
		float x = vtxX[i] - pCol->GetPosX();
		float y = vtxY[i] - pCol->GetPosY();
		float r = pCol->GetColRadius();

		if((x * x) + (y * y) < (r * r))
			return true;
	}

	// ----- 矩形の4辺と円との当たり判定
	for(int i = 0; i < 4; ++i) {
		float x = vtxX[(i + 1) % 4] - vtxX[i];
		float y = vtxY[(i + 1) % 4] - vtxY[i];
		float t = -(x * (vtxX[i] - pCol->GetPosX()) + y * (vtxY[i] - pCol->GetPosY()) / (x * x + y * y));
		if(t < 0)
			t = 0;
		if(t > 1)
			t = 1;

		x = vtxX[i] + x * t;
		y = vtxY[i] + y * t;

		if(sqrt((pCol->GetPosX() - x) * (pCol->GetPosX() - x) + (pCol->GetPosY() - y) * (pCol->GetPosY() - y)) < pCol->GetColRadius())
			return true;
	}

	// ----- 矩形内(面)と円との当たり判定
	float x, y;
	float qx, qy;
	float t0, t1;
	
	x	= vtxX[1] - vtxX[0];
	y	= vtxY[1] - vtxY[0];
	qx	= pCol->GetPosX() - vtxX[0];
	qy	= pCol->GetPosY() - vtxY[0];
	t0	= atan2(x * qx + y * qy, x * qy - y * qx);

	x	= vtxX[3] - vtxX[2];
	y	= vtxY[3] - vtxY[2];
	qx	= pCol->GetPosX() - vtxX[2];
	qy	= pCol->GetPosY() - vtxY[2];
	t1	= atan2(x * qx + y * qy, x * qy - y * qx);
	
	if(t0 >= 0 && t0 <= M_PI / 2 && t1 >= 0 && t1 <= M_PI / 2)
		return true;

	return false;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 当たり判定
//	Description : 衝突対象の2Dキャラクタと、矩形と円との当たり判定を行う(衝突対象が矩形の場合)
//				  矩形が回転していても可
//	Arguments   : pCol / 衝突対象キャラクタ
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CCharacter::JudgeCircleSquare(const CCharacter* pCol)
{
	// ----- 行列演算済み頂点を算出
	D3DXVECTOR3 vtx[4] = {pCol->GetVertex(1).vtx, pCol->GetVertex(0).vtx, pCol->GetVertex(2).vtx, pCol->GetVertex(3).vtx};
	float vtxX[4];
	float vtxY[4];
	for(int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&vtx[i], &vtx[i], &pCol->GetMatrix());
		vtxX[i] = vtx[i].x;
		vtxY[i] = vtx[i].y;
	}

	// ----- 矩形の4頂点と円との当たり判定
	for(int i = 0; i < 4; ++i) {
		float x = vtxX[i] - GetPosX();
		float y = vtxY[i] - GetPosY();
		float r = GetColRadius();

		if((x * x) + (y * y) < (r * r))
			return true;
	}

	// ----- 矩形の4辺と円との当たり判定
	for(int i = 0; i < 4; ++i) {
		float x = vtxX[(i + 1) % 4] - vtxX[i];
		float y = vtxY[(i + 1) % 4] - vtxY[i];
		float t = -(x * (vtxX[i] - GetPosX()) + y * (vtxY[i] - GetPosY()) / (x * x + y * y));
		if(t < 0)
			t = 0;
		if(t > 1)
			t = 1;

		x = vtxX[i] + x * t;
		y = vtxY[i] + y * t;

		if(sqrt((GetPosX() - x) * (GetPosX() - x) + (GetPosY() - y) * (GetPosY() - y)) < GetColRadius())
			return true;
	}

	// ----- 矩形内(面)と円との当たり判定
	float x, y;
	float qx, qy;
	float t0, t1;
	
	x	= vtxX[1] - vtxX[0];
	y	= vtxY[1] - vtxY[0];
	qx	= GetPosX() - vtxX[0];
	qy	= GetPosY() - vtxY[0];
	t0	= atan2(x * qx + y * qy, x * qy - y * qx);

	x	= vtxX[3] - vtxX[2];
	y	= vtxY[3] - vtxY[2];
	qx	= GetPosX() - vtxX[2];
	qy	= GetPosY() - vtxY[2];
	t1	= atan2(x * qx + y * qy, x * qy - y * qx);
	
	if(t0 >= 0 && t0 <= M_PI / 2 && t1 >= 0 && t1 <= M_PI / 2)
		return true;

	return false;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 当たり判定
//	Description : 衝突対象の2Dキャラクタと矩形同士の当たり判定を行う(回転していても可)
//	Arguments   : pCol / 衝突対象キャラクタ
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CCharacter::JudgeSquareSquare(const CCharacter* pCol)
{
	// ----- 行列演算済み頂点を算出
	// 自キャラクタ
	D3DXVECTOR3 vtx[4] = {m_vtx[1].vtx, m_vtx[0].vtx, m_vtx[2].vtx, m_vtx[3].vtx};
	float vtxX[4];
	float vtxY[4];
	for(int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&vtx[i], &vtx[i], &m_world);
		vtxX[i] = vtx[i].x;
		vtxY[i] = vtx[i].y;
	}
	// 対象キャラクタ
	D3DXVECTOR3 colVtx[4] = {m_vtx[1].vtx, m_vtx[0].vtx, m_vtx[2].vtx, m_vtx[3].vtx};
	float colVtxX[4];
	float colVtxY[4];
	for(int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&colVtx[i], &colVtx[i], &pCol->GetMatrix());
		colVtxX[i] = colVtx[i].x;
		colVtxY[i] = colVtx[i].y;
	}

	float radius = 1.0f;	// 判定対象の頂点の半径(点として扱う)
	for(int j = 0; j < 4; ++j) {
		// ----- 自キャラクタを矩形とするパターン
		// 矩形の4頂点と円との当たり判定
		for(int i = 0; i < 4; ++i) {
			float x = vtxX[i] - colVtxX[j];
			float y = vtxY[i] - colVtxY[j];
			float r = radius;

			if((x * x) + (y * y) < (r * r))
				return true;
		}

		// 矩形の4辺と円との当たり判定
		for(int i = 0; i < 4; ++i) {
			float x = vtxX[(i + 1) % 4] - vtxX[i];
			float y = vtxY[(i + 1) % 4] - vtxY[i];
			float t = -(x * (vtxX[i] - colVtxX[j]) + y * (vtxY[i] - colVtxY[j]) / (x * x + y * y));
			if(t < 0)
				t = 0;
			if(t > 1)
				t = 1;

			x = vtxX[i] + x * t;
			y = vtxY[i] + y * t;

			if(sqrt((colVtxX[j] - x) * (colVtxX[j] - x) + (colVtxY[j] - y) * (colVtxY[j] - y)) < radius)
				return true;
		}

		// 矩形内(面)と円との当たり判定
		float x, y;
		float qx, qy;
		float t0, t1;
	
		x	= vtxX[1] - vtxX[0];
		y	= vtxY[1] - vtxY[0];
		qx	= colVtxX[j] - vtxX[0];
		qy	= colVtxY[j] - vtxY[0];
		t0	= atan2(x * qx + y * qy, x * qy - y * qx);

		x	= vtxX[3] - vtxX[2];
		y	= vtxY[3] - vtxY[2];
		qx	= colVtxX[j] - vtxX[2];
		qy	= colVtxY[j] - vtxY[2];
		t1	= atan2(x * qx + y * qy, x * qy - y * qx);
	
		if(t0 >= 0 && t0 <= M_PI / 2 && t1 >= 0 && t1 <= M_PI / 2)
			return true;
		
		// ----- 対象キャラクタを矩形とするパターン
		// 矩形の4頂点と円との当たり判定
		for(int i = 0; i < 4; ++i) {
			float x = colVtxX[i] - vtxX[j];
			float y = colVtxY[i] - vtxY[j];
			float r = radius;

			if((x * x) + (y * y) < (r * r))
				return true;
		}

		// 矩形の4辺と円との当たり判定
		for(int i = 0; i < 4; ++i) {
			float x = colVtxX[(i + 1) % 4] - colVtxX[i];
			float y = colVtxY[(i + 1) % 4] - colVtxY[i];
			float t = -(x * (colVtxX[i] - vtxX[j]) + y * (colVtxY[i] - vtxY[j]) / (x * x + y * y));
			if(t < 0)
				t = 0;
			if(t > 1)
				t = 1;

			x = colVtxX[i] + x * t;
			y = colVtxY[i] + y * t;

			if(sqrt((vtxX[j] - x) * (vtxX[j] - x) + (vtxY[j] - y) * (vtxY[j] - y)) < radius)
				return true;
		}

		// 矩形内(面)と円との当たり判定	
		x	= colVtxX[1] - colVtxX[0];
		y	= colVtxY[1] - colVtxY[0];
		qx	= vtxX[j] - colVtxX[0];
		qy	= vtxY[j] - colVtxY[0];
		t0	= atan2(x * qx + y * qy, x * qy - y * qx);

		x	= colVtxX[3] - colVtxX[2];
		y	= colVtxY[3] - colVtxY[2];
		qx	= vtxX[j] - colVtxX[2];
		qy	= vtxY[j] - colVtxY[2];
		t1	= atan2(x * qx + y * qy, x * qy - y * qx);
	
		if(t0 >= 0 && t0 <= M_PI / 2 && t1 >= 0 && t1 <= M_PI / 2)
			return true;
	}

	return false;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 当たり判定
//	Description : 衝突対象の2Dキャラクタと、矩形と線分との当たり判定を行う(衝突対象が線分の場合)
//	Arguments   : pCol / 衝突対象キャラクタ
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CCharacter::JudgeSquareLine(const CCharacter* pCol)
{
	// ----- 事前準備
	D3DXVECTOR2	line = pCol->GetColEndLine() - pCol->GetColStartLine();	// 線分の方向ベクトル及び大きさ
	D3DXVECTOR3 colVtx[4] = {m_vtx[1].vtx, m_vtx[0].vtx, m_vtx[2].vtx, m_vtx[3].vtx};
	D3DXVECTOR2 colVtx2[4];
	for(int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&colVtx[i], &colVtx[i], &GetMatrix());
		colVtx2[i].x = colVtx[i].x;
		colVtx2[i].y = colVtx[i].y;
	}

	// ----- 当たり判定
	for(int i = 0; i < 4; ++i) {
		D3DXVECTOR2	frame;
		frame.x = colVtx2[(i + 1) % 4].x - colVtx2[i].x;
		frame.y = colVtx2[(i + 1) % 4].y - colVtx2[i].y;

		D3DXVECTOR2 v = colVtx2[i] - pCol->GetColStartLine();

		// 平衡状態かチェック
		float v1 = System::D3DXVec2Cross(&line, &frame);
		if(v1 == 0.0f) {
			continue;
		}

		// 交差しているかチェック
		float v2 = System::D3DXVec2Cross(&v, &line);
		float v3 = System::D3DXVec2Cross(&v, &frame);

		float t1 = v3 / v1;
		float t2 = v2 / v1;

		const float RANGE = 0.00001f;	// 誤差の範囲
		if(	t1 + RANGE >= 0.0f && t1 - RANGE <= 1.0f &&
			t2 + RANGE >= 0.0f && t2 - RANGE <= 1.0f) {
			return true;
		}
	}

	return false;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 当たり判定
//	Description : 衝突対象の2Dキャラクタと、矩形と線分との当たり判定を行う(衝突対象が矩形の場合)
//	Arguments   : pCol / 衝突対象キャラクタ
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CCharacter::JudgeLineSquare(const CCharacter* pCol)
{
	// ----- 事前準備
	D3DXVECTOR2	line = GetColEndLine() - GetColStartLine();	// 線分の方向ベクトル及び大きさ
	D3DXVECTOR3 colVtx[4] = {pCol->GetVertex(1).vtx, pCol->GetVertex(0).vtx, pCol->GetVertex(2).vtx, pCol->GetVertex(3).vtx};
	D3DXVECTOR2 colVtx2[4];
	for(int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&colVtx[i], &colVtx[i], &pCol->GetMatrix());
		colVtx2[i].x = colVtx[i].x;
		colVtx2[i].y = colVtx[i].y;
	}

	// ----- 当たり判定
	for(int i = 0; i < 4; ++i) {
		D3DXVECTOR2	frame;
		frame.x = colVtx2[(i + 1) % 4].x - colVtx2[i].x;
		frame.y = colVtx2[(i + 1) % 4].y - colVtx2[i].y;

		D3DXVECTOR2 v = colVtx2[i] - GetColStartLine();

		// 平衡状態かチェック
		float v1 = System::D3DXVec2Cross(&line, &frame);
		if(v1 == 0.0f) {
			continue;
		}

		// 交差しているかチェック
		float v2 = System::D3DXVec2Cross(&v, &line);
		float v3 = System::D3DXVec2Cross(&v, &frame);

		float t1 = v3 / v1;
		float t2 = v2 / v1;

		const float RANGE = 0.00001f;	// 誤差の範囲
		if(	t1 + RANGE >= 0.0f && t1 - RANGE <= 1.0f &&
			t2 + RANGE >= 0.0f && t2 - RANGE <= 1.0f) {
			return true;
		}
	}

	return false;
}


//========================================================================================
//	End of File
//========================================================================================