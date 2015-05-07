//========================================================================================
//		File        : Score.cpp
//		Program     : スコアクラス
//
//		Description : スコアクラスの実装
//
//		History     : 2014/11/27	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include "../../h/Object/Score.h"

//――――――――――――――――――――――――――――――――――――――――――――
// メンバ実体宣言
//――――――――――――――――――――――――――――――――――――――――――――
// public:
const LONG	CScore::MAX_SCORE		= 1000000;		// スコアの最大値
const int	CScore::SCORE_DIGIT		= 7;			// 最大スコアの桁数

// private:
const LPCTSTR		CScore::TEX_FILENAME	= _T("res/img/Score.png");		// テクスチャファイル名
const D3DXVECTOR2	CScore::OBJ_SIZE		= D3DXVECTOR2(48.0f, 48.0f);	// オブジェクトサイズ
const int			CScore::WIDTH_FRAME		= 10;							// アニメーションフレームの横分割数
const int			CScore::HEIGHT_FRAME	= 1;							// アニメーションフレームの縦分割数


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CScore::CScore()
{
	m_score = 0;	// 描画スコア
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CScore::~CScore()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : オブジェクトデータを初期化する
//	Arguments   : pos / 出現位置
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CScore::Init(const D3DXVECTOR3& pos)
{
	// ----- データ初期化
	m_score = 0;	// 描画スコア

	// ----- オブジェクト初期化
	int numCnt = 0;
	for(LPNUMBER_ARRAY_IT it = m_pNumberList.begin(); it != m_pNumberList.end(); ++it) {
		(*it)->Init(OBJ_SIZE);		// 初期化
		(*it)->Translate(D3DXVECTOR3(pos.x + (OBJ_SIZE.x * numCnt), pos.y, pos.z));	// 移動処理
		(*it)->UVDivision(0, WIDTH_FRAME, HEIGHT_FRAME);	// アニメーション準備

		++numCnt;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : オブジェクトデータの後始末をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CScore::Uninit()
{
	for(LPNUMBER_ARRAY_IT it = m_pNumberList.begin(); it != m_pNumberList.end(); ++it) {
		(*it)->Uninit();
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : オブジェクトを更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CScore::Update()
{
	for(LPNUMBER_ARRAY_IT it = m_pNumberList.begin(); it != m_pNumberList.end(); ++it) {
		(*it)->Update();
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : オブジェクトをアルファ無効で描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CScore::Draw()
{
	for(LPNUMBER_ARRAY_IT it = m_pNumberList.begin(); it != m_pNumberList.end(); ++it) {
		(*it)->Draw();
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : オブジェクトをアルファ有効で描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CScore::DrawAlpha()
{
	for(LPNUMBER_ARRAY_IT it = m_pNumberList.begin(); it != m_pNumberList.end(); ++it) {
		(*it)->DrawAlpha();
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成
//	Description : オブジェクトを生成する
//	Arguments   : None.
//	Returns     : オブジェクトデータ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CScore* CScore::Create()
{
	// ----- 変数宣言
	CScore* pScore;

	// ----- 初期化処理
	pScore = new CScore();
	if(pScore)
	{
		if(!pScore->Initialize())
		{
			SAFE_DELETE(pScore);
		}
	}

	return pScore;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 解放処理
//	Description : オブジェクトデータを解放する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CScore::Release()
{
	Finalize();
	delete this;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : 描画スコアを移動する
//	Arguments   : pos / 出現座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CScore::Translate(const D3DXVECTOR3& pos)
{
	// ----- 移動処理
	int numCnt = 0;
	for(LPNUMBER_ARRAY_IT it = m_pNumberList.begin(); it != m_pNumberList.end(); ++it) {
		(*it)->Translate(D3DXVECTOR3(pos.x + (OBJ_SIZE.x * numCnt), pos.y, pos.z));
		++numCnt;
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : スコア更新
//	Description : 描画スコアを更新する
//	Arguments   : score / 描画スコア
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CScore::SetScore(LONG score)
{
	// ----- スコア更新
	m_score = score;

	// ----- オブジェクト更新
	int digit = SCORE_DIGIT - 1;	// スコア桁番号
	while(score > 0) {
		int num = score % 10;	// 描画する数字を算出
		m_pNumberList[digit]->UVDivision(num, WIDTH_FRAME, HEIGHT_FRAME);	// 数字を反映
		--digit;		// 次の桁へ
		score /= 10;	// 次の桁へ
	}
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : オブジェクトデータを初期化する
//	Arguments   : None.
//	Returns     : 成否
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CScore::Initialize()
{
	// ----- オブジェクト生成
	for(int i = 0; i < SCORE_DIGIT; ++i) {
		CNumber* pNum = CNumber::Create(TEX_FILENAME);
		if(pNum == NULL) {
#ifdef _DEBUG_MESSAGEBOX
			::MessageBox(NULL, _T("CScore::Initialize Numberの生成に失敗しました。"), _T("error"), MB_OK);
#endif
			return false;
		}
		m_pNumberList.push_back(pNum);
	}
	
	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : オブジェクトデータの後始末をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CScore::Finalize()
{
	for(LPNUMBER_ARRAY_IT it = m_pNumberList.begin(); it != m_pNumberList.end(); ++it) {
		SAFE_RELEASE((*it));
	}

	m_pNumberList.clear();
}


//========================================================================================
//	End of File
//========================================================================================