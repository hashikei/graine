//========================================================================================
//		File        : ChangeScene.cpp
//		Program     : シーン遷移システム
//
//		Description : シーン遷移システムの実装
//
//		History     : 2015/02/20	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include <tchar.h>
#include "../../h/System/System.h"
#include "../../h/System/ChangeScene.h"

//――――――――――――――――――――――――――――――――――――――――――――
// グローバル変数宣言
//――――――――――――――――――――――――――――――――――――――――――――
static CChangeScene& g_changeScene = CChangeScene::GetInstance();		// シーン遷移システムの実体生成

//――――――――――――――――――――――――――――――――――――――――――――
// メンバ実体宣言
//――――――――――――――――――――――――――――――――――――――――――――
// ----- メンバ定数
// private:
const LPCTSTR CChangeScene::TEX_FILENAME[MAX_TEXLIST] = {
	_T("res/img/Fade.jpg"),		// 単色フェード用テクスチャファイル名
};

// ----- メンバ変数
// private:
CObject2D* CChangeScene::m_pNormalFade;


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : インスタンス取得
//	Description : シーン遷移システムのインスタンスを取得する
//	Arguments   : None.
//	Returns     : シーン遷移システム
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CChangeScene& CChangeScene::GetInstance()
{
	// ----- インスタンス生成
	static CChangeScene changeScene;

	return changeScene;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 位置設定
//	Description : 単色フェード用オブジェクトの位置を設定する
//	Arguments   : pos / 出現位置
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CChangeScene::SetNormalFadePos(const D3DXVECTOR3& pos)
{
	CreateNormalFade();

	m_pNormalFade->Translate(pos);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 頂点カラー設定
//	Description : 単色フェード用オブジェクトの頂点カラーを設定する(0～255)
//	Arguments   : color / 頂点カラー(RGB)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CChangeScene::SetNormalFadeColor(const D3DXVECTOR3& color)
{
	CreateNormalFade();

	m_pNormalFade->SetColor(color);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 透過度設定
//	Description : 単色フェード用オブジェクトの透過度を設定する(0～255)
//	Arguments   : alpha / 透過度
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CChangeScene::SetNormalFadeAlpha(int alpha)
{
	CreateNormalFade();

	m_pNormalFade->SetAlpha(alpha);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : フェードイン
//	Description : 単色で透過するフェードイン処理
//	Arguments   : z     / 描画位置(Z座標)
//				  alpha / 加算するアルファ値
//	Returns     : 完了フラグ(true:完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CChangeScene::NormalFadeIn(float z, int alpha)
{
	// ----- 事前準備
	CreateNormalFade();		// オブジェクト生成(未生成なら)

	// ----- フェード処理
	bool ret = m_pNormalFade->FadeInAlpha(alpha);
	
	// ----- 描画位置調整
	m_pNormalFade->TranslateZ(z);

	return ret;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : フェードアウト
//	Description : 単色で透過するフェードアウト処理
//	Arguments   : z     / 描画位置(Z座標)
//				  alpha / 加算するアルファ値
//	Returns     : 完了フラグ(true:完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CChangeScene::NormalFadeOut(float z, int alpha)
{
	// ----- 事前準備
	CreateNormalFade();		// オブジェクト生成(未生成なら)

	// ----- フェード処理
	bool ret = m_pNormalFade->FadeOutAlpha(alpha);

	// ----- 描画位置調整
	m_pNormalFade->TranslateZ(z);

	return ret;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : 単色フェード用オブジェクトを描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CChangeScene::DrawNormalFade()
{
	// ----- 描画処理
	m_pNormalFade->DrawScreenAlpha();
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CChangeScene::CChangeScene()
{
	m_pNormalFade = NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CChangeScene::~CChangeScene()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : テクスチャ生成
//	Description : 単色フェード用テクスチャを生成する
//	Arguments   : None.
//	Returns     : 0:失敗, 1:成功, 2:生成済み
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
int CChangeScene::CreateNormalFade()
{
	// ----- オブジェクトが未生成だったら新規で生成
	if(!m_pNormalFade) {
		m_pNormalFade = CObject2D::Create(TEX_FILENAME[TL_NORMALFADE]);
		if(m_pNormalFade == NULL) {
#ifdef _DEBUG_MESSAGEBOX
			::MessageBox(NULL, _T("CChangeScene::NormalFadeの生成に失敗しました。"), _T("error"), MB_OK);
#endif
			return 0;
		}

		// 初期化処理
		m_pNormalFade->Init();
		m_pNormalFade->Translate(D3DXVECTOR3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.0f));

		return 1;
	}

	return 2;
}


//========================================================================================
// namespace:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 左右の遷移(画面に出現)
//	Description : 画面に出現するよう、左右に滑るような遷移
//	Arguments   : pTex / テクスチャ
//				  x    / X方向移動量
//	Returns     : 完了フラグ(true:遷移完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool ChangeScene::SideSliderIn(CObject2D* pTex, float x)
{
	// ----- 移動準備
	bool bLeft = true;
	if(x < 0)
		bLeft = false;

	// ----- 移動処理
	pTex->TranslationX(x);
	// 境界判定
	if(bLeft) {
		if(pTex->GetPosX() > SCREEN_WIDTH) {
			pTex->TranslateX((float)SCREEN_WIDTH);
			return true;
		}
	} else {
		if(pTex->GetPosX() + pTex->GetWidth() < 0.0f) {
			pTex->TranslateX(-pTex->GetWidth());
			return true;
		}
	}

	return false;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 左右の遷移(画面から退避)
//	Description : 画面から退避するよう、左右に滑るような遷移
//	Arguments   : pTex / テクスチャ
//				  x    / X方向移動量
//	Returns     : 完了フラグ(true:遷移完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool ChangeScene::SideSliderOut(CObject2D* pTex, float x)
{
	// ----- 移動準備
	bool bLeft = true;
	if(x < 0)
		bLeft = false;

	// ----- 移動処理
	pTex->TranslationX(x);
	// 境界判定
	if(bLeft) {
		if(pTex->GetPosX() + pTex->GetWidth() > SCREEN_WIDTH) {
			pTex->TranslateX((float)SCREEN_WIDTH - pTex->GetWidth());
			return true;
		}
	} else {
		if(pTex->GetPosX() < 0.0f) {
			pTex->TranslateX(0.0f);
			return true;
		}
	}

	return false;
}


//========================================================================================
//	End of File
//========================================================================================