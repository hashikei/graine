//========================================================================================
//		File        : Object.cpp
//		Program     : オブジェクトベース
//
//		Description : オブジェクトベースの実装
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
#include "../../h/Object/Object.h"


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CObject::CObject()
{
	D3DXMatrixIdentity(&m_world);
	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_bExist	= false;
	m_bDraw		= false;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CObject::~CObject()
{
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 解放処理
//	Description : オブジェクトデータを解放する
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::Release()
{
	Finalize();
	delete this;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを相対位置へ移動する
//	Arguments   : vec / 移動量
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::Translation(const D3DXVECTOR3& vec)
{
	// ----- 移動量反映
	m_pos += vec;
	
	// ----- 移動処理
	m_world._41 = m_pos.x;
	m_world._42 = m_pos.y;
	m_world._43 = m_pos.z;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを相対位置(X方向)へ移動する
//	Arguments   : x / 移動量
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::TranslationX(float x)
{
	// ----- 移動量反映
	m_pos.x += x;
	
	// ----- 移動処理
	m_world._41 = m_pos.x;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを相対位置(Y方向)へ移動する
//	Arguments   : y / 移動量
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::TranslationY(float y)
{
	// ----- 移動量反映
	m_pos.y += y;
	
	// ----- 移動処理
	m_world._42 = m_pos.y;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを相対位置(Z方向)へ移動する
//	Arguments   : z / 移動量
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::TranslationZ(float z)
{
	// ----- 移動量反映
	m_pos.z += z;
	
	// ----- 移動処理
	m_world._43 = m_pos.z;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを絶対位置へ移動する
//	Arguments   : pos / 移動先座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::Translate(const D3DXVECTOR3& pos)
{
	// ----- 現在位置更新
	m_pos = pos;

	// ----- 移動処理
	m_world._41 = m_pos.x;
	m_world._42 = m_pos.y;
	m_world._43 = m_pos.z;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを絶対位置(X方向)へ移動する
//	Arguments   : x / 移動先座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::TranslateX(float x)
{
	// ----- 現在位置更新
	m_pos.x = x;

	// ----- 移動処理
	m_world._41 = m_pos.x;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを絶対位置(Y方向)へ移動する
//	Arguments   : y / 移動先座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::TranslateY(float y)
{
	// ----- 現在位置更新
	m_pos.y = y;

	// ----- 移動処理
	m_world._42 = m_pos.y;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 移動処理
//	Description : オブジェクトを絶対位置(Z方向)へ移動する
//	Arguments   : z / 移動先座標
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::TranslateZ(float z)
{
	// ----- 現在位置更新
	m_pos.z = z;

	// ----- 移動処理
	m_world._43 = m_pos.z;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを相対的に回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::Rotation(const D3DXVECTOR3& angle)
{
	// ----- 回転角更新
	m_rot += angle;

	// ----- 回転角調整
	if(m_rot.x >= 360.0f)
		m_rot.x -= 360.0f;
	if(m_rot.y >= 360.0f)
		m_rot.y -= 360.0f;
	if(m_rot.z >= 360.0f)
		m_rot.z -= 360.0f;

	// ----- ワールド変換行列再計算
	RecalcMatrix();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを相対的にX軸回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::RotationX(float angle)
{
	// ----- 回転角更新
	m_rot.x += angle;

	// ----- 回転角調整
	if(m_rot.x >= 360.0f)
		m_rot.x -= 360.0f;

	// ----- ワールド変換行列再計算
	RecalcMatrix();



	//// ----- 変数宣言
	//D3DXMATRIX matRot;

	//// ----- 回転処理
	//D3DXVECTOR3 axis(m_world._11, m_world._12, m_world._13);
	//D3DXMatrixRotationAxis(&matRot, &axis, D3DXToRadian(angle));
	//m_world *= matRot;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを相対的にY軸回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::RotationY(float angle)
{
	// ----- 回転角更新
	m_rot.y += angle;

	// ----- 回転角調整
	if(m_rot.y >= 360.0f)
		m_rot.y -= 360.0f;

	// ----- ワールド変換行列再計算
	RecalcMatrix();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを相対的にZ軸回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::RotationZ(float angle)
{
	// ----- 回転角更新
	m_rot.z += angle;

	// ----- 回転角調整
	if(m_rot.z >= 360.0f)
		m_rot.z -= 360.0f;

	// ----- ワールド変換行列再計算
	RecalcMatrix();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを相対的にクォータニオンで任意軸回転する
//	Arguments   : pV    / 回転軸の方向ベクトル
//				  angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//void CObject::RotationQuaternion(const D3DXVECTOR3* pV, float angle)
//{
//	// ----- 変数宣言
//	D3DXMATRIX		matQuat;
//	D3DXQUATERNION	quat;
//
//	// ----- 回転処理
//	D3DXQuaternionRotationAxis(&quat, pV, D3DXToRadian(angle));
//	D3DXMatrixRotationQuaternion(&matQuat, &quat);
//	m_world *= matQuat;
//}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを絶対的に回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::Rotate(const D3DXVECTOR3& angle)
{
	// ----- 回転角更新
	m_rot = angle;

	// ----- 回転角調整
	if(m_rot.x >= 360.0f)
		m_rot.x -= 360.0f;
	if(m_rot.y >= 360.0f)
		m_rot.y -= 360.0f;
	if(m_rot.z >= 360.0f)
		m_rot.z -= 360.0f;

	// ----- ワールド変換行列再計算
	RecalcMatrix();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを絶対的にX軸回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::RotateX(float angle)
{
	// ----- 回転角更新
	m_rot.x = angle;

	// ----- 回転角調整
	if(m_rot.x >= 360.0f)
		m_rot.x -= 360.0f;

	// ----- ワールド変換行列再計算
	RecalcMatrix();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを絶対的にY軸回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::RotateY(float angle)
{
	// ----- 回転角更新
	m_rot.y = angle;

	// ----- 回転角調整
	if(m_rot.y >= 360.0f)
		m_rot.y -= 360.0f;

	// ----- ワールド変換行列再計算
	RecalcMatrix();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを絶対的にZ軸回転する
//	Arguments   : angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::RotateZ(float angle)
{
	// ----- 回転角更新
	m_rot.z = angle;

	// ----- 回転角調整
	if(m_rot.z >= 360.0f)
		m_rot.z -= 360.0f;

	// ----- ワールド変換行列再計算
	RecalcMatrix();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 回転処理
//	Description : オブジェクトを絶対的にクォータニオンで任意軸回転する
//	Arguments   : pV    / 回転軸の方向ベクトル
//				  angle / 回転角
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//void CObject::RotateQuaternion(const D3DXVECTOR3* pV, float angle)
//{
//	// ----- 変数宣言
//	D3DXMATRIX		matQuat;
//	D3DXQUATERNION	quat;
//	
//	// ----- 回転をリセット
//	m_world._12 = 0;
//	m_world._13 = 0;
//	m_world._21 = 0;
//	m_world._23 = 0;
//	m_world._31 = 0;
//	m_world._32 = 0;
//
//	// ----- 回転処理
//	D3DXQuaternionRotationAxis(&quat, pV, D3DXToRadian(angle));
//	D3DXMatrixRotationQuaternion(&matQuat, &quat);
//	m_world *= matQuat;
//}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 拡縮処理
//	Description : オブジェクトを相対的に拡縮する
//	Arguments   : scale / 拡縮率
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::Scaling(const D3DXVECTOR3& scale)
{
	// ----- 拡縮率更新
	m_scale += scale;
	
	// ----- ワールド変換行列再計算
	RecalcMatrix();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 拡縮処理
//	Description : オブジェクトを相対的にX方向に拡縮する
//	Arguments   : x / 拡縮率
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::ScalingX(float x)
{
	// ----- 拡縮率更新
	m_scale.x += x;
	
	// ----- ワールド変換行列再計算
	RecalcMatrix();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 拡縮処理
//	Description : オブジェクトを相対的にY方向に拡縮する
//	Arguments   : y / 拡縮率
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::ScalingY(float y)
{
	// ----- 拡縮率更新
	m_scale.y += y;
	
	// ----- ワールド変換行列再計算
	RecalcMatrix();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 拡縮処理
//	Description : オブジェクトを相対的にZ方向に拡縮する
//	Arguments   : z / 拡縮率
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::ScalingZ(float z)
{
	// ----- 拡縮率更新
	m_scale.z += z;
	
	// ----- ワールド変換行列再計算
	RecalcMatrix();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 拡縮処理
//	Description : オブジェクトを絶対的に拡縮する
//	Arguments   : scale / 拡縮率
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::Scale(const D3DXVECTOR3& scale)
{
	// ----- 拡縮率更新
	m_scale = scale;
	
	// ----- ワールド変換行列再計算
	RecalcMatrix();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 拡縮処理
//	Description : オブジェクトを絶対的にX方向に拡縮する
//	Arguments   : x / 拡縮率
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::ScaleX(float x)
{
	// ----- 拡縮率更新
	m_scale.x = x;
	
	// ----- ワールド変換行列再計算
	RecalcMatrix();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 拡縮処理
//	Description : オブジェクトを絶対的にY方向に拡縮する
//	Arguments   : y / 拡縮率
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::ScaleY(float y)
{
	// ----- 拡縮率更新
	m_scale.y = y;
	
	// ----- ワールド変換行列再計算
	RecalcMatrix();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 拡縮処理
//	Description : オブジェクトを絶対的にZ方向に拡縮する
//	Arguments   : z / 拡縮率
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::ScaleZ(float z)
{
	// ----- 拡縮率更新
	m_scale.z = z;
	
	// ----- ワールド変換行列再計算
	RecalcMatrix();
}


//========================================================================================
// protected:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : オブジェクトデータの初期化を行う
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CObject::Initialize()
{
	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : ワールド変換行列再計算
//	Description : ワールド変換行列の再計算を行う
//	Arguments   : None.
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CObject::RecalcMatrix()
{
	// ----- 事前準備
	D3DXMATRIX matTrans, matRot, matScale;

	// ----- 変換行列生成
	D3DXMatrixScaling(&matScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&matRot, D3DXToRadian(m_rot.y), D3DXToRadian(m_rot.x), D3DXToRadian(m_rot.z));
	D3DXMatrixTranslation(&matTrans, m_pos.x, m_pos.y, m_pos.z);

	// ----- ワールド変換行列更新
	m_world = (matScale * matRot * matTrans);
}


//========================================================================================
//	End of File
//========================================================================================