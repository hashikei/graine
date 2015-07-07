//========================================================================================
//		File        : FieldBlock.cpp
//		Program     : フィールドブロックベース
//
//		Description : フィールドブロックベースの実装
//			
//
//		History     : 2015/5/6	作成開始
//
//																Author : きんたまズ
//========================================================================================


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include <math.h>
#include "../../h/Object/FieldBlock.h"

//――――――――――――――――――――――――――――――――――――――――――――
// 定数定義
//――――――――――――――――――――――――――――――――――――――――――――
const int CFieldBlock::INITIALIZE_ELEMENT_NUM = 30;		// 初期ブロック要素数


//========================================================================================
// public:
//========================================================================================
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CFieldBlock::CFieldBlock()
{
	m_bCol = false;
	m_nType = 0;
	m_pElement.reserve(INITIALIZE_ELEMENT_NUM);

	m_nFlower = 0;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CFieldBlock::~CFieldBlock()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : オブジェクトを初期化する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CFieldBlock::Init()
{
	// ----- オブジェクト初期化
	CCharacter::Init();

	for (LPCHARACTER_ARRAY_IT it = m_pElement.begin(); it != m_pElement.end(); ++it)
		(*it)->Init();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : オブジェクトを初期化する
//	Arguments   : size / オブジェクトサイズ
//				  pos  / 出現位置(オブジェクトの中央)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CFieldBlock::Init(const D3DXVECTOR2& size, const D3DXVECTOR3& pos)
{
	// ----- 頂点データ初期化
	CFieldBlock::Init();

	// ----- サイズ設定
	Resize(size);

	// ----- 描画位置設定
	Translate(pos);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : オブジェクトの後始末をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CFieldBlock::Uninit(void)
{
	// ----- オブジェクト後始末
	CCharacter::Uninit();

	for (LPCHARACTER_ARRAY_IT it = m_pElement.begin(); it != m_pElement.end(); ++it)
		(*it)->Uninit();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : オブジェクトを更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CFieldBlock::Update()
{
	// ----- オブジェクト更新
	CCharacter::Update();

	for (LPCHARACTER_ARRAY_IT it = m_pElement.begin(); it != m_pElement.end(); ++it)
		(*it)->Update();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : オブジェクトを描画する(透過なし)
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CFieldBlock::Draw()
{
	for (LPCHARACTER_ARRAY_IT it = m_pElement.begin(); it != m_pElement.end(); ++it)
		(*it)->Draw();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : オブジェクトを描画する(透過あり)
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CFieldBlock::DrawAlpha()
{
	for (LPCHARACTER_ARRAY_IT it = m_pElement.begin(); it != m_pElement.end(); ++it)
		(*it)->DrawAlpha();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成
//	Description : オブジェクトを生成する
//	Arguments   : None.
//	Returns     : オブジェクトデータ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CFieldBlock* CFieldBlock::Create()
{
	// ----- 変数宣言
	CFieldBlock* pObj;

	// ----- 初期化処理
	pObj = new CFieldBlock();
	if (pObj)
	{
		if (!pObj->Initialize())
		{
			SAFE_DELETE(pObj);
		}
	}

	return pObj;
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : オブジェクトを初期化する
//	Arguments   : None.
//	Returns     : 成否
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CFieldBlock::Initialize()
{
	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : オブジェクトの終了処理をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CFieldBlock::Finalize(void)
{
}


//========================================================================================
//	End of File
//========================================================================================