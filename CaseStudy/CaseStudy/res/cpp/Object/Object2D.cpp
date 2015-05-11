//========================================================================================
//		File        : Object2D.cpp
//		Program     : 2Dオブジェクトベース
//
//		Description : 2Dオブジェクトベースの実装
//					  原点：ポリゴンの中央
//
//		History     : 2014/06/11	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include <math.h>
#include "../../h/Object/Object2D.h"

//――――――――――――――――――――――――――――――――――――――――――――
// メンバ実体宣言
//――――――――――――――――――――――――――――――――――――――――――――
CObject2D::OBJ2D_INFO* CObject2D::m_pObj2DInfoTop = NULL;


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CObject2D::CObject2D()
{
	m_pDevice		= NULL;
	m_pTexture		= NULL;
	m_pObj2DInfo	= NULL;

	// 頂点データ初期化
	for(int i = 0; i < 4; ++i)
	{
		m_vtx[i].vtx = DEFAULT_VTX3D_VTX[i];	// 画面全体
		m_vtx[i].col = DEFAULT_VTX_COL_W;		// 白色
		m_vtx[i].uv  = DEFAULT_VTX_UV[i];		// UV全体
	}
	
	m_size		= D3DXVECTOR2(0.0f, 0.0f);
	m_halfSize	= D3DXVECTOR2(0.0f, 0.0f);
	m_defSize	= D3DXVECTOR2(0.0f, 0.0f);
	m_offset	= D3DXVECTOR2(0.0f, 0.0f);

	m_colRadius	= 0.0f;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CObject2D::~CObject2D()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : オブジェクトデータをデフォルト値で初期化する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Init()
{
	// ----- 頂点データ初期化
	for(int i = 0; i < 4; ++i)
	{
		m_vtx[i].vtx = DEFAULT_VTX3D_VTX[i];	// 画面全体
		m_vtx[i].col = DEFAULT_VTX_COL_W;		// 白色
		m_vtx[i].uv  = DEFAULT_VTX_UV[i];		// UV全体
	}
	m_material	= DEFAULT_MATERIAL;		// マテリアル

	m_bExist	= true;		// 存在フラグ
	m_bDraw		= true;		// 描画フラグ
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : オブジェクトデータを初期化する
//	Arguments   : size / オブジェクトサイズ
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Init(const D3DXVECTOR2& size)
{
	// ----- 頂点データ初期化
	CObject2D::Init();

	// ----- サイズ設定
	Resize(size);
	m_defSize = m_size;	// デフォルトサイズ

	// ----- 当たり判定用半径設定
	m_colRadius = sqrt((m_vtx[1].vtx.x * m_vtx[1].vtx.x) + (m_vtx[1].vtx.y * m_vtx[1].vtx.y));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : オブジェクトを初期化する
//	Arguments   : width  / オブジェクト幅
//				  height / オブジェクト高さ
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Init(const float width, const float height)
{
	CObject2D::Init(D3DXVECTOR2(width, height));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : オブジェクトを初期化する
//	Arguments   : size / オブジェクトサイズ
//				  pos  / 出現位置(オブジェクトの中央)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Init(const D3DXVECTOR2& size, const D3DXVECTOR3& pos)
{
	// ----- 頂点データ初期化
	CObject2D::Init(size);

	// ----- 描画位置設定
	Translate(pos);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : オブジェクトを初期化する
//	Arguments   : width  / オブジェクト幅
//				  height / オブジェクト高さ
//				  x      / 出現位置(X座標)
//				  y      / 出現位置(Y座標)
//				  z      / 出現位置(Z座標)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Init(const float width, const float height, const float x, const float y, const float z)
{
	Init(D3DXVECTOR2(width, height), D3DXVECTOR3(x, y, z));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : オブジェクトデータの後始末をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Uninit(void)
{
	memset(m_vtx, 0, sizeof(m_vtx));	// 頂点情報クリア
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : オブジェクトを更新する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Update(void)
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : オブジェクトを透過無しで描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Draw()
{
	// ----- 描画対象でなければ未描画
	if(!m_bDraw)
		return;

	// ----- ワールド変換行列を設定
    m_pDevice->SetTransform(D3DTS_WORLD, &m_world);

	// ----- 頂点フォーマットの設定
	m_pDevice->SetFVF(FVF_VERTEX3D);

	// ----- 各種無効化
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	// ライティング無効化

	// ----- 背景テクスチャの設定及びポリゴンの描画
	m_pDevice->SetMaterial(&m_material);
	m_pDevice->SetTexture(0, m_pTexture);
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_vtx, sizeof(VERTEX3D));
	
	// ----- 各種有効化
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);	// ライティング有効化
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : オブジェクトを透過有りで描画する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::DrawAlpha()
{
	// ----- 描画対象でなければ未描画
	if(!m_bDraw)
		return;

	// ----- ワールド変換行列を設定
    m_pDevice->SetTransform(D3DTS_WORLD, &m_world);

	// ----- 頂点フォーマットの設定
	m_pDevice->SetFVF(FVF_VERTEX3D);

	// ----- 各種無効化
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	// ライティング無効化
	
	// ----- アルファ ブレンド有効化
    m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ----- アルファ テスト有効化
    m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	
	// ----- 背景テクスチャの設定及びポリゴンの描画
	m_pDevice->SetMaterial(&m_material);
	m_pDevice->SetTexture(0, m_pTexture);
	m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_vtx, sizeof(VERTEX3D));
	
    // ----- アルファ ブレンド無効化
    m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// ----- アルファ テスト無効化
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	
	// ----- 各種有効化
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);	// ライティング有効化
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : ビルボード描画
//	Description : オブジェクトを透過無しでビルボードとして描画する
//	Arguments   : target / 対象の座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::DrawBillBoard(const D3DXVECTOR3& target)
{
	// ----- 描画対象でなければ未描画
	if(!m_bDraw)
		return;

	// ----- ビルボード回転行列を算出
	// 準備
	D3DXVECTOR3 pos = GetPosition();
	D3DXMATRIX rot;

	// 逆行列算出
	D3DXMatrixIdentity(&rot);
	D3DXMatrixLookAtLH(&rot, &target, &pos, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixInverse(&rot, NULL, &rot);

	// 回転行列のみにする
	rot._41 = 0.0f;
	rot._42 = 0.0f;
	rot._43 = 0.0f;

	// ----- ビルボード処理
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);
	D3DXMatrixIdentity(&m_world);
	m_world *= rot * trans;

	// ----- 描画処理
	Draw();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : ビルボード描画
//	Description : オブジェクトを透過有りでビルボードとして描画する
//	Arguments   : target / 対象の座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::DrawBillBoardAlpha(const D3DXVECTOR3& target)
{
	// ----- 描画対象でなければ未描画
	if(!m_bDraw)
		return;

	// ----- ビルボード回転行列を算出
	// 準備
	D3DXVECTOR3 pos = GetPosition();
	D3DXMATRIX rot;

	// 逆行列算出
	D3DXMatrixIdentity(&rot);
	D3DXMatrixLookAtLH(&rot, &target, &pos, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixInverse(&rot, NULL, &rot);

	// 回転行列のみにする
	rot._41 = 0.0f;
	rot._42 = 0.0f;
	rot._43 = 0.0f;

	// ----- ビルボード処理
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, pos.x, pos.y, pos.z);
	D3DXMatrixIdentity(&m_world);
	m_world *= rot * trans;

	// ----- 描画処理
	DrawAlpha();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成
//	Description : オブジェクトを生成する
//	Arguments   : pszFName / 読み込みファイル名
//	Returns     : オブジェクトデータ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CObject2D* CObject2D::Create(const LPCTSTR pszFName)
{
	// ----- 変数宣言
	CObject2D* pObj;

	// ----- 初期化処理
	pObj = new CObject2D();
	if(pObj)
	{
		if(!pObj->Initialize(pszFName))
		{
			SAFE_DELETE(pObj);
		}
	}

	return pObj;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 解放処理
//	Description : オブジェクトデータを解放する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Release()
{
	CObject::Release();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 頂点カラー設定
//	Description : 2Dオブジェクトの頂点カラーを設定する(0～255)
//	Arguments   : color / 頂点カラー(RGB)
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::SetColor(const D3DXVECTOR3& color)
{
	// 頂点カラー設定
	for(int i = 0; i < 4; ++i) {
		m_vtx[i].col = D3DCOLOR_RGBA((int)color.x, (int)color.y, (int)color.z, D3DCOLOR_RGBA_TO_ALPHA(m_vtx[i].col));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 透過度設定
//	Description : 2Dオブジェクトの透過度を設定する(0～255)
//	Arguments   : alpha / 透過度
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::SetAlpha(float alpha)
{
	// 透過度設定
	for(int i = 0; i < 4; ++i) {
		m_vtx[i].col = D3DCOLOR_RGBA(D3DCOLOR_RGBA_TO_R(m_vtx[i].col),
									 D3DCOLOR_RGBA_TO_G(m_vtx[i].col),
									 D3DCOLOR_RGBA_TO_B(m_vtx[i].col),
									 (int)alpha);
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを相対位置へ移動する
//	Arguments   : vec / 移動量
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Translation(const D3DXVECTOR3& vec)
{
	CObject::Translation(vec);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを相対位置(X方向)へ移動する
//	Arguments   : x / 移動量
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::TranslationX(float x)
{
	CObject::TranslationX(x);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを相対位置(Y方向)へ移動する
//	Arguments   : y / 移動量
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::TranslationY(float y)
{
	CObject::TranslationY(y);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを相対位置(Z方向)へ移動する
//	Arguments   : z / 移動量
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::TranslationZ(float z)
{
	CObject::TranslationZ(z);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを絶対位置へ移動する
//	Arguments   : pos / 移動先座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Translate(const D3DXVECTOR3& pos)
{
	CObject::Translate(pos);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを絶対位置(X方向)へ移動する
//	Arguments   : x / 移動先座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::TranslateX(float x)
{
	CObject::TranslateX(x);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを絶対位置(Y方向)へ移動する
//	Arguments   : y / 移動先座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::TranslateY(float y)
{
	CObject::TranslateY(y);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを絶対位置(Z方向)へ移動する
//	Arguments   : z / 移動先座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::TranslateZ(float z)
{
	CObject::TranslateZ(z);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを相対的に回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Rotation(const D3DXVECTOR3& angle)
{
	CObject::Rotation(angle);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを相対的にX軸回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::RotationX(float angle)
{
	CObject::RotationX(angle);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを相対的にY軸回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::RotationY(float angle)
{
	CObject::RotationY(angle);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを相対的にZ軸回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::RotationZ(float angle)
{
	CObject::RotationZ(angle);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを相対的にクォータニオンで任意軸回転する
//	Arguments   : pV    / 回転軸の方向ベクトル
//				  angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//void CObject2D::RotationQuaternion(const D3DXVECTOR3* pV, float angle)
//{
//	CObject::RotationQuaternion(pV, angle);
//}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを絶対的に回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Rotate(const D3DXVECTOR3& angle)
{
	CObject::Rotate(angle);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを絶対的にX軸回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::RotateX(float angle)
{
	CObject::RotateX(angle);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを絶対的にY軸回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::RotateY(float angle)
{
	CObject::RotateY(angle);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを絶対的にZ軸回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::RotateZ(float angle)
{
	CObject::RotateZ(angle);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを絶対的にクォータニオンで任意軸回転する
//	Arguments   : pV    / 回転軸の方向ベクトル
//				  angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//void CObject2D::RotateQuaternion(const D3DXVECTOR3* pV, float angle)
//{
//	CObject::RotateQuaternion(pV, angle);
//}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 拡縮処理
//	Description : オブジェクトを相対的に拡縮する
//	Arguments   : scale / 拡縮率
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Scaling(const D3DXVECTOR3& scale)
{
	// ----- 拡縮処理
	CObject::Scaling(scale);

	// ----- サイズ更新
	m_size = D3DXVECTOR2(m_size.x * m_scale.x, m_size.y * m_scale.y);
	m_halfSize = m_size * 0.5f;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 拡縮処理
//	Description : オブジェクトを相対的にX方向に拡縮する
//	Arguments   : x / 拡縮率
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::ScalingX(float x)
{
	// ----- 拡縮処理
	CObject::ScalingX(x);

	// ----- サイズ更新
	m_size = D3DXVECTOR2(m_size.x * m_scale.x, m_size.y * m_scale.y);
	m_halfSize = m_size * 0.5f;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 拡縮処理
//	Description : オブジェクトを相対的にY方向に拡縮する
//	Arguments   : y / 拡縮率
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::ScalingY(float y)
{
	// ----- 拡縮処理
	CObject::ScalingY(y);

	// ----- サイズ更新
	m_size = D3DXVECTOR2(m_size.x * m_scale.x, m_size.y * m_scale.y);
	m_halfSize = m_size * 0.5f;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 拡縮処理
//	Description : オブジェクトを絶対的に拡縮する
//	Arguments   : scale / 拡縮率
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Scale(const D3DXVECTOR3& scale)
{
	// ----- 拡縮処理
	CObject::Scale(scale);
	
	// ----- サイズ更新
	m_size = D3DXVECTOR2(m_defSize.x * m_scale.x, m_defSize.y * m_scale.y);
	m_halfSize = m_size * 0.5f;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 拡縮処理
//	Description : オブジェクトを絶対的にX方向に拡縮する
//	Arguments   : x / 拡縮率
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::ScaleX(float x)
{
	// ----- 拡縮処理
	CObject::ScaleX(x);
	
	// ----- サイズ更新
	m_size = D3DXVECTOR2(m_defSize.x * m_scale.x, m_defSize.y * m_scale.y);
	m_halfSize = m_size * 0.5f;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 拡縮処理
//	Description : オブジェクトを絶対的にY方向に拡縮する
//	Arguments   : y / 拡縮率
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::ScaleY(float y)
{
	// ----- 拡縮処理
	CObject::ScaleY(y);
	
	// ----- サイズ更新
	m_size = D3DXVECTOR2(m_defSize.x * m_scale.x, m_defSize.y * m_scale.y);
	m_halfSize = m_size * 0.5f;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 当たり判定
//	Description : 衝突対象の2Dオブジェクトとの当たり判定を行う(当たった瞬間のみ判定)
//	Arguments   : id   / 当たり判定ID
//				  pCol / 衝突対象オブジェクト
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject2D::CollisionEnter(int id, const CObject2D* pCol)
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
//	Description : 衝突対象の2Dオブジェクトとの当たり判定を行う(当たっている間のみ判定)
//	Arguments   : id   / 当たり判定ID
//				  pCol / 衝突対象オブジェクト
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject2D::CollisionStay(int id, const CObject2D* pCol)
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

		default:
			break;
	}

	return ret;
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 当たり判定
//	Description : 衝突対象の2Dオブジェクトとの当たり判定を行う(離れた瞬間のみ判定)
//	Arguments   : id   / 当たり判定ID
//				  pCol / 衝突対象オブジェクト
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject2D::CollisionExit(int id, const CObject2D* pCol)
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

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : フェード処理
//	Description : 対象の2Dオブジェクトを任意の間隔でフェード処理する
//	Arguments   : nFade / フェード間隔
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::FadeColor(int nFade)
{
	// ----- 変数宣言
	DWORD dwTempRed;		// r値格納バッファ
	DWORD dwTempGreen;		// g値格納バッファ
	DWORD dwTempBlue;		// b値格納バッファ

	// ----- 初期化処理
	dwTempRed	= 0;
	dwTempGreen	= 0;
	dwTempBlue	= 0;

	// ----- エラー処理
	if(nFade < 0)
		nFade = 0;
	if(nFade > 255)
		nFade = 255;
	
	// ----- フェードイン
	for(int nCntVtx = 0; nCntVtx < 4; nCntVtx ++)
	{
		dwTempRed	= ((m_vtx + nCntVtx)->col & 0x00ff0000);					// r値抽出
		dwTempGreen	= ((m_vtx + nCntVtx)->col & 0x0000ff00);					// g値抽出
		dwTempBlue	= ((m_vtx + nCntVtx)->col & 0x000000ff);					// b値抽出
		(m_vtx + nCntVtx)->col ^= (dwTempRed | dwTempGreen | dwTempBlue);		// アルファ値クリア
		dwTempRed	>>= 16;														// 整数へ変換
		dwTempGreen	>>= 8;														// 整数へ変換
		dwTempRed = nFade;
		dwTempGreen = nFade;
		dwTempBlue = nFade;
		dwTempRed	<<= 16;														// r位置へ戻す
		dwTempGreen	<<= 8;														// g位置へ戻す
		(m_vtx + nCntVtx)->col |= (dwTempRed | dwTempGreen | dwTempBlue);		// フェード反映
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : フェードイン
//	Description : 対象の2Dオブジェクトを任意の間隔でフェードインする
//	Arguments   : nFade / フェード間隔
//	Returns     : 完了フラグ(true:完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject2D::FadeInColor(int nFade)
{
	// ----- 変数宣言
	DWORD dwTempRed;		// r値格納バッファ
	DWORD dwTempGreen;		// g値格納バッファ
	DWORD dwTempBlue;		// b値格納バッファ
	bool  complete;			// 完了フラグ

	// ----- 初期化処理
	dwTempRed	= 0;
	dwTempGreen	= 0;
	dwTempBlue	= 0;
	complete	= false;

	// ----- エラー処理
	if(nFade < 0)
		nFade = 0;
	if(nFade > 255)
		nFade = 255;
	
	// ----- フェードイン
	for(int nCntVtx = 0; nCntVtx < 4; nCntVtx ++)
	{
		dwTempRed	= ((m_vtx + nCntVtx)->col & 0x00ff0000);					// r値抽出
		dwTempGreen	= ((m_vtx + nCntVtx)->col & 0x0000ff00);					// g値抽出
		dwTempBlue	= ((m_vtx + nCntVtx)->col & 0x000000ff);					// b値抽出
		(m_vtx + nCntVtx)->col ^= (dwTempRed | dwTempGreen | dwTempBlue);		// アルファ値クリア
		dwTempRed	>>= 16;														// 整数へ変換
		dwTempGreen	>>= 8;														// 整数へ変換
		if(dwTempRed < 255)														// フェード更新
			dwTempRed += nFade;
		if(dwTempGreen < 255)													// フェード更新
			dwTempGreen += nFade;
		if(dwTempBlue < 255)													// フェード更新
			dwTempBlue += nFade;
		if(dwTempRed >= 255)													// エラー処理
		{
			dwTempRed = 255;
			complete  = true;
		}
		if(dwTempGreen >= 255)													// エラー処理
		{
			dwTempGreen = 255;
			complete    = true;
		}
		if(dwTempBlue >= 255)													// エラー処理
		{
			dwTempBlue = 255;
			complete   = true;
		}
		dwTempRed	<<= 16;														// r位置へ戻す
		dwTempGreen	<<= 8;														// g位置へ戻す
		(m_vtx + nCntVtx)->col |= (dwTempRed | dwTempGreen | dwTempBlue);		// フェード反映
	}

	return complete;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : フェードアウト
//	Description : 対象の2Dオブジェクトを任意の間隔でフェードアウトする
//	Arguments   : nFade / フェード間隔
//	Returns     : 完了フラグ(true:完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject2D::FadeOutColor(int nFade)
{
	// ----- 変数宣言
	DWORD dwTempRed;		// r値格納バッファ
	DWORD dwTempGreen;		// g値格納バッファ
	DWORD dwTempBlue;		// b値格納バッファ
	int   nRed;				// r値格納バッファ
	int   nGreen;			// g値格納バッファ
	int   nBlue;			// b値格納バッファ
	bool  complete;			// 完了フラグ

	// ----- 初期化処理
	dwTempRed	= 0;
	dwTempGreen	= 0;
	dwTempBlue	= 0;
	nRed		= 0;
	nGreen		= 0;
	nBlue		= 0;
	complete	= false;

	// ----- エラー処理
	if(nFade < 0)
		nFade = 0;
	if(nFade > 255)
		nFade = 255;
	
	// ----- フェードアウト
	for(int nCntVtx = 0; nCntVtx < 4; nCntVtx ++)
	{
		dwTempRed	= ((m_vtx + nCntVtx)->col & 0x00ff0000);					// r値抽出
		dwTempGreen	= ((m_vtx + nCntVtx)->col & 0x0000ff00);					// g値抽出
		dwTempBlue	= ((m_vtx + nCntVtx)->col & 0x000000ff);					// b値抽出
		(m_vtx + nCntVtx)->col ^= (dwTempRed | dwTempGreen | dwTempBlue);		// アルファ値クリア
		nRed	= dwTempRed   >> 16;											// 整数へ変換
		nGreen	= dwTempGreen >> 8;												// 整数へ変換
		nBlue	= dwTempBlue;													// 整数へ変換
		if(nRed > 0)															// フェード更新
			nRed -= nFade;
		if(nGreen > 0)															// フェード更新
			nGreen -= nFade;
		if(nBlue > 0)															// フェード更新
			nBlue -= nFade;
		if(nRed <= 0)															// エラー処理
		{
			nRed	 = 0;
			complete = true;
		}
		if(nGreen <= 0)															// エラー処理
		{
			nGreen	 = 0;
			complete = true;
		}
		if(nBlue <= 0)															// エラー処理
		{
			nBlue	 = 0;
			complete = true;
		}
		dwTempRed	= nRed   << 16;												// r位置へ戻す
		dwTempGreen	= nGreen << 8;												// g位置へ戻す
		dwTempBlue	= nBlue;													// b位置へ戻す
		(m_vtx + nCntVtx)->col |= (dwTempRed | dwTempGreen | dwTempBlue);		// フェード反映
	}

	return complete;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 透過フェード処理
//	Description : 対象の2Dオブジェクトを任意の間隔で透過的にフェード処理する
//	Arguments   : nFade / フェード間隔
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::FadeAlpha(int nFade)
{
	// ----- 変数宣言
	DWORD dwTempAlpha;		// アルファ値格納バッファ

	// ----- 初期化処理
	dwTempAlpha = 0;

	// ----- エラー処理
	if(nFade < 0)
		nFade = 0;
	if(nFade > 255)
		nFade = 255;
	
	// ----- フェードイン
	for(int nCntVtx = 0; nCntVtx < 4; nCntVtx ++)
	{
		dwTempAlpha = ((m_vtx + nCntVtx)->col & 0xff000000);	// アルファ値抽出
		(m_vtx + nCntVtx)->col ^= dwTempAlpha;					// アルファ値クリア
		dwTempAlpha >>= 24;										// 整数へ変換
		dwTempAlpha = nFade;
		dwTempAlpha <<= 24;										// アルファ位置へ戻す
		(m_vtx + nCntVtx)->col |= dwTempAlpha;					// フェード反映
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 透過フェードイン
//	Description : 対象の2Dオブジェクトを任意の間隔で透過的にフェードインする
//	Arguments   : nFade / フェード間隔
//	Returns     : 完了フラグ(true:完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject2D::FadeInAlpha(int nFade)
{
	// ----- 変数宣言
	DWORD dwTempAlpha;		// アルファ値格納バッファ
	bool  complete;			// 完了フラグ

	// ----- 初期化処理
	dwTempAlpha = 0;
	complete	= false;

	// ----- エラー処理
	if(nFade < 0)
		nFade = 0;
	if(nFade > 255)
		nFade = 255;
	
	// ----- フェードイン
	for(int nCntVtx = 0; nCntVtx < 4; nCntVtx ++)
	{
		dwTempAlpha = ((m_vtx + nCntVtx)->col & 0xff000000);	// アルファ値抽出
		(m_vtx + nCntVtx)->col ^= dwTempAlpha;					// アルファ値クリア
		dwTempAlpha >>= 24;										// 整数へ変換
		if(dwTempAlpha < 255)									// フェード更新
			dwTempAlpha += nFade;
		if(dwTempAlpha >= 255)									// エラー処理
		{
			dwTempAlpha	= 255;
			complete	= true;
		}
		dwTempAlpha <<= 24;										// アルファ位置へ戻す
		(m_vtx + nCntVtx)->col |= dwTempAlpha;					// フェード反映
	}

	return complete;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 透過フェードアウト
//	Description : 対象の2Dオブジェクトを任意の間隔で透過的にフェードアウトする
//	Arguments   : nFade / フェード間隔
//	Returns     : 完了フラグ(true:完了)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject2D::FadeOutAlpha(int nFade)
{
	// ----- 変数宣言
	DWORD dwTempAlpha;		// アルファ値格納バッファ
	int   nAlpha;			// アルファ値格納バッファ
	bool  complete;			// 完了フラグ

	// ----- 初期化処理
	dwTempAlpha = 0;
	nAlpha		= 0;
	complete	= false;

	// ----- エラー処理
	if(nFade < 0)
		nFade = 0;
	if(nFade > 255)
		nFade = 255;
	
	// ----- フェードイン
	for(int nCntVtx = 0; nCntVtx < 4; nCntVtx ++)
	{
		dwTempAlpha = ((m_vtx + nCntVtx)->col & 0xff000000);	// アルファ値抽出
		(m_vtx + nCntVtx)->col ^= dwTempAlpha;					// アルファ値クリア
		nAlpha = dwTempAlpha >> 24;								// 整数へ変換
		if(nAlpha > 0)											// フェード更新
			nAlpha -= nFade;
		if(nAlpha <= 0)											// エラー処理
		{
			nAlpha	= 0;
			complete	= true;
		}
		dwTempAlpha = nAlpha << 24;								// アルファ位置へ戻す
		(m_vtx + nCntVtx)->col |= dwTempAlpha;					// フェード反映
	}

	return complete;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : オブジェクトサイズ変更
//	Description : オブジェクトのサイズを変更する
//	Arguments   : size / オブジェクトサイズ
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Resize(const D3DXVECTOR2& size)
{
	// サイズ設定
	m_size = size;
	m_halfSize = m_size * 0.5f;

	// オブジェクトに反映
	m_vtx[0].vtx.x = -m_halfSize.x;
	m_vtx[0].vtx.y =  m_halfSize.y;
	m_vtx[1].vtx.x =  m_halfSize.x;
	m_vtx[1].vtx.y =  m_halfSize.y;
	m_vtx[2].vtx.x = -m_halfSize.x;
	m_vtx[2].vtx.y = -m_halfSize.y;
	m_vtx[3].vtx.x =  m_halfSize.x;
	m_vtx[3].vtx.y = -m_halfSize.y;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : オブジェクトサイズ変更
//	Description : オブジェクトのサイズを変更する
//	Arguments   : width  / オブジェクト幅
//				  height / オブジェクト高さ
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Resize(const float width, const float height)
{
	Resize(D3DXVECTOR2(width, height));
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : UV値を分割
//	Description : UV値を任意の数に分割し、現在の数値のUV値を設定する
//				  ※左上・右上・左下・右下の順に数値を数える
//	Arguments   : num    / 参照番号
//				  width  / 横分割数
//				  height / 縦分割数
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::UVDivision(int num, int width, int height)
{
	// ----- UV値を算出
	float left		= (1.0f / width) * ((num) % width);
	float right		= (1.0f / width) * ((num) % width) + (1.0f / width);
	float top		= (1.0f / height) * ((num) / width);
	float bottom	= (1.0f / height) * ((num) / width) + (1.0f / height);

	// ----- UV値を設定
	m_vtx[0].uv = D3DXVECTOR2(left,  top);
	m_vtx[1].uv = D3DXVECTOR2(right, top);
	m_vtx[2].uv = D3DXVECTOR2(left,  bottom);
	m_vtx[3].uv = D3DXVECTOR2(right, bottom);
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
void CObject2D::FrameAnimation(int start, int end, int width, int height, double time)
{
	int		maxSeg		= end - start + 1;		// アニメーションコマ数
	int		segment		= (int)floor(CTimer::GetTime() / time) % maxSeg;	// 描画コマ数

	UVDivision(segment + start, width, height);
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : オブジェクトデータを初期化する
//	Arguments   : pszFName / 読み込みファイル名
//	Returns     : 成否
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject2D::Initialize(const LPCTSTR pszFName)
{
	// ----- オブジェクト初期化
	if(!CObject::Initialize())
		return false;
	
	// ----- テクスチャ読み込み
	if(FAILED(Load(CGraphics::GetDevice(), pszFName)))
	{
		return false;
	}
	
	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : オブジェクトデータの後始末をする
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject2D::Finalize(void)
{
	if (m_pObj2DInfo) {
		if (--(m_pObj2DInfo->dwReference) == 0) {
			SAFE_RELEASE(m_pObj2DInfo->pTexture);
			free((void*)m_pObj2DInfo->pszFName);
			for (OBJ2D_INFO* p = m_pObj2DInfoTop; p; p = p->pNext) {
				if (m_pObj2DInfo == p) {
					if (p->pPrev)
						m_pObj2DInfo->pPrev->pNext = p->pNext;
					else
						m_pObj2DInfoTop = p->pNext;
					if (p->pNext)
						p->pNext->pPrev = p->pPrev;
					break;
				}
			}
			delete m_pObj2DInfo;
		}
		m_pTexture   = NULL;
		m_pObj2DInfo = NULL;
	}
	m_pDevice = NULL;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : テクスチャ読み込み
//	Description : テクスチャデータを読み込む
//	Arguments   : pDevice  / デバイス
//				  pszFName / テクスチャファイル名
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
HRESULT CObject2D::Load(const LPDIRECT3DDEVICE9 pDevice, const LPCTSTR pszFName)
{
	if (pDevice == NULL) {
		return E_POINTER;
	}

	// デバイス保存
	m_pDevice = pDevice;

//	Finalize();
	
	// ファイル名変換
	int strLen = lstrlen(pszFName);
	char* fname = new char[strLen + 1];
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, 0, pszFName, strLen, fname, strLen + 1, NULL, NULL);
	fname[strLen] = '\0';
#else
	lstrcpy(fname, pszFName);
	fname[strLen] = '\0';
#endif

	// ファイル名が無い場合も正常終了
	if (fname == NULL || *fname == '\0') {
		m_pTexture = NULL;
		delete fname;
		return S_OK;
	}

	// テクスチャプールをチェック
	for (OBJ2D_INFO* p = m_pObj2DInfoTop; p; p = p->pNext) {
		if (_stricmp(p->pszFName, fname) == 0) {
			++(p->dwReference);
			m_pTexture = p->pTexture;
			m_pObj2DInfo = p;
			delete fname;
			return S_OK;
		}
	}

	// ファイル名の複製を作成
	LPSTR pszDup = _strdup(fname);
	if (pszDup == NULL) {
		delete fname;
		return E_OUTOFMEMORY;
	}

	// テクスチャファイルを読み込む
	D3DXIMAGE_INFO info;
	HRESULT hr = D3DXCreateTextureFromFileExA(pDevice, fname,
		D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, 0, &info, NULL, &m_pTexture);
	if (FAILED(hr)) {
		free(pszDup);
		delete fname;
		return hr;
	}

	// テクスチャ プールに追加
	m_pObj2DInfo = new OBJ2D_INFO;
	if (m_pObj2DInfo == NULL) {
		free(pszDup);
		delete fname;
		return E_OUTOFMEMORY;
	}

	m_pObj2DInfo->pPrev = NULL;
	m_pObj2DInfo->pNext = m_pObj2DInfoTop;
	if (m_pObj2DInfoTop)
		m_pObj2DInfoTop->pPrev = m_pObj2DInfo;
	m_pObj2DInfoTop = m_pObj2DInfo;

	m_pObj2DInfo->pTexture = m_pTexture;
	m_pObj2DInfo->pszFName = pszDup;
	m_pObj2DInfo->info = info;
	m_pObj2DInfo->dwReference = 1;
	m_offset = D3DXVECTOR2(0.0f, 0.0f);
	m_size = D3DXVECTOR2((float)info.Width, (float)info.Height);
	
	delete fname;

	return hr;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 当たり判定
//	Description : 衝突対象の2Dオブジェクトとバウンディングボックスによる当たり判定を行う
//	Arguments   : pCol / 衝突対象オブジェクト
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject2D::BoundingBox(const CObject2D* pCol)
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
//	Description : 衝突対象の2Dオブジェクトとバウンディングサークルによる当たり判定を行う
//	Arguments   : pCol / 衝突対象オブジェクト
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject2D::BoundingCircle(const CObject2D* pCol)
{
	float dist	= (pCol->GetPosX() - this->GetPosX()) * (pCol->GetPosX() - this->GetPosX()) + (pCol->GetPosY() - this->GetPosY()) * (pCol->GetPosY() - this->GetPosY());	// オブジェクト間の距離
	float r		= m_colRadius + pCol->GetColRadius();		// オブジェクト間の衝突距離
	if(dist < (r * r))
		return true;

	return false;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 当たり判定
//	Description : 衝突対象の2Dオブジェクトと、矩形と円との当たり判定を行う(判定対象が円の場合)
//				  矩形が回転していても可
//	Arguments   : pCol / 衝突対象オブジェクト
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject2D::JudgeSquareCircle(const CObject2D* pCol)
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
//	Description : 衝突対象の2Dオブジェクトと、矩形と円との当たり判定を行う(判定対象が矩形の場合)
//				  矩形が回転していても可
//	Arguments   : pCol / 衝突対象オブジェクト
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject2D::JudgeCircleSquare(const CObject2D* pCol)
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
//	Description : 衝突対象の2Dオブジェクトと矩形同士の当たり判定を行う(回転していても可)
//	Arguments   : pCol / 衝突対象オブジェクト
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject2D::JudgeSquareSquare(const CObject2D* pCol)
{
	// ----- 行列演算済み頂点を算出
	// 自オブジェクト
	D3DXVECTOR3 vtx[4] = {m_vtx[1].vtx, m_vtx[0].vtx, m_vtx[2].vtx, m_vtx[3].vtx};
	float vtxX[4];
	float vtxY[4];
	for(int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&vtx[i], &vtx[i], &m_world);
		vtxX[i] = vtx[i].x;
		vtxY[i] = vtx[i].y;
	}
	// 対象オブジェクト
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
		// ----- 自オブジェクトを矩形とするパターン
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
		
		// ----- 対象オブジェクトを矩形とするパターン
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

#ifdef __USE_INPUT_OBJECT2D
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : マウスとの当たり判定(四角形)
//	Description : マウスカーソルがオブジェクト(板ポリサイズ)と当たっているかチェックする
//	Arguments   : None.
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject2D::CheckMouseOverBox()
{
	// ----- 事前準備
	POINT curPos = Input::GetCurPos();	// マウスカーソルの座標
	
	// ----- 座標系変換
	curPos.x -= (int)(SCREEN_WIDTH * 0.5f);
	curPos.y = SCREEN_HEIGHT - curPos.y;
	curPos.y -= (int)(SCREEN_HEIGHT * 0.5f);

	// ----- 当たり判定
	if(	curPos.x > GetLeftPos()  &&
		curPos.x < GetRightPos() &&
		curPos.y > GetTopPos()   &&
		curPos.y < GetBottomPos()) {
		return true;
	}

	return false;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : マウスとの当たり判定(円)
//	Description : マウスカーソルがオブジェクト(指定半径サイズ)と当たっているかチェックする
//	Arguments   : radius / 判定する半径
//	Returns     : 判定結果(true:当たっている)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject2D::CheckMouseOverCircle(float radius)
{
	// ----- 事前準備
	POINT curPos = Input::GetCurPos();	// マウスカーソルの座標
	
	// ----- 座標系変換
	curPos.x -= (int)(SCREEN_WIDTH * 0.5f);
	curPos.y = SCREEN_HEIGHT - curPos.y;
	curPos.y -= (int)(SCREEN_HEIGHT * 0.5f);

	// ----- マウスとオブジェクトの距離を算出
	float x = curPos.x - m_pos.x;
	float y = curPos.y - m_pos.y;
	float dist = sqrt((x * x) + (y * y));

	// ----- 当たり判定
	if(dist < radius) {
		return true;
	}

	return false;
}
#endif


//========================================================================================
//	End of File
//========================================================================================