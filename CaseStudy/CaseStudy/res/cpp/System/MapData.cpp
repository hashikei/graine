//========================================================================================
//		File        : MapData.cpp
//		Program     : マップデータクラス
//
//		Description : マップデータクラスの実装
//
//		History     : 2015/05/18	作成開始
//
//																Author : Kei Hashimoto
//========================================================================================


//――――――――――――――――――――――――――――――――――――――――――――
// 警告無効化
//――――――――――――――――――――――――――――――――――――――――――――
#pragma warning(disable:4996)

//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/PreInclude.h"
#
#include <tchar.h>
#include <fstream>
#include <string>
#include <sstream>
#include "../../h/System/MapData.h"

//――――――――――――――――――――――――――――――――――――――――――――
// メンバ実体宣言
//――――――――――――――――――――――――――――――――――――――――――――
// ----- 定数
// private:
LPCTSTR CMapData::MAPDATA_LIST[MAX_STAGEID] = {		// マップデータのファイル名リスト
	_T("res/data/map/Stage1.csv"),
};
const int	CMapData::INIT_OBJECT_NUM	= 1000;		// 初期オブジェクト数

// ----- 変数
// private:
LPFIELDOBJECT_ARRAY	CMapData::m_pFieldObj;	// フィールドオブジェクトリスト
LPCHARACTER_ARRAY	CMapData::m_pLayoutObj;	// レイアウトオブジェクトリスト
D3DXVECTOR2			CMapData::m_startPoint;	// 開始位置

//――――――――――――――――――――――――――――――――――――――――――――
// グローバル変数宣言
//――――――――――――――――――――――――――――――――――――――――――――
static CMapData& g_mapData = CMapData::GetInstance();		// マップデータクラスの実体生成


//========================================================================================
// public:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : インスタンス取得
//	Description : マップデータクラスのインスタンスを取得する
//	Arguments   : None.
//	Returns     : マップデータクラス
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CMapData& CMapData::GetInstance()
{
	// ----- インスタンス生成
	static CMapData mapData;

	return mapData;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : マップデータロード
//	Description : 任意のマップデータをロードする
//	Arguments   : id / ステージID
//	Returns     : 成否(true:成功)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
bool CMapData::LoadData(int id)
{
	// ----- 初期化処理
	m_pFieldObj.clear();
	m_pLayoutObj.clear();

	// ----- マップデータ読み込み
	std::ifstream ifs(MAPDATA_LIST[id]);
	if(ifs.fail()) {
#ifdef _DEBUG_MESSAGEBOX
		MessageBox(hWnd, _T("MapData::Load map data error!"), _T("error"), MB_OK | MB_ICONERROR);
#endif
		return false;
	}
	std::istreambuf_iterator<char> it(ifs);
	std::istreambuf_iterator<char> last;
	std::string str(it, last);

	// ----- マップデータ登録
	// 登録準備
	std::stringstream ss(str);
	std::string tmp;

	// コメント行をスキップ
//	for(int i = 0; i < MAX_DATAPARAM; ++i) {
//		if(!getline(ss, tmp, ',')) {
//#ifdef _DEBUG_MESSAGEBOX
//			MessageBox(hWnd, _T("MapData::map data error!"), _T("error"), MB_OK | MB_ICONERROR);
//#endif
//			return false;
//		}
//	}

	// 開始位置読み込み
	getline(ss, tmp, ',');		// X座標登録
	m_startPoint.x = stof(tmp);
	getline(ss, tmp, ',');		// Y座標登録
	m_startPoint.y = stof(tmp);

	// フィールドブロックのデータ読み込み
	CCharacter* pObj = NULL;
	int			cnt		= 0;
	float		width	= 0.0f;
	float		height	= 0.0f;
	D3DXVECTOR3	color(0.0f, 0.0f, 0.0f);
	while(getline(ss, tmp, ',')) {
		switch(cnt % MAX_DATAPARAM) {
			case DP_ID:
				// TODO:IDを保存したい場合は追加
				break;

			case DP_TEX:
			{
				LPTSTR ws = new TCHAR[tmp.size() + 1];
				mbstowcs(ws, tmp.c_str(), tmp.size());
				ws[tmp.size()] = '\0';
				pObj = CCharacter::Create(ws);
				pObj->Init();
				delete[] ws;
				break;
			}

			case DP_POSX:
				pObj->TranslateX(stof(tmp));
				break;

			case DP_POSY:
				pObj->TranslateY(stof(tmp));
				break;

			case DP_POSZ:
				pObj->TranslateZ(stof(tmp));
				break;

			case DP_WIDTH:
				width = stof(tmp);
				break;

			case DP_HEIGHT:
				height = stof(tmp);
				pObj->Resize(D3DXVECTOR2(width, height));
				break;

			case DP_ANGLE:
				pObj->RotateZ(stof(tmp));
				break;

			case DP_COLR:
				color.x = stof(tmp);
				break;

			case DP_COLG:
				color.y = stof(tmp);
				break;

			case DP_COLB:
				color.z = stof(tmp);
				pObj->SetColor(color);
				break;

			case DP_COLA:
				pObj->SetAlpha(stof(tmp));
				break;

			case DP_COLFLG:
				stoi(tmp) > 0 ? m_pFieldObj.push_back((CFieldObject*)pObj) : m_pLayoutObj.push_back(pObj);
				break;

			case DP_TYPE:
			{
				// 0:普通のフィールドブロック
				// 1:クリア条件フィールドブロック
				// 2:障害フィールドブロック
				// 3:レイアウトブロック
				// 4:レイアウトオブジェクト
				int type = stoi(tmp);
				if(type <= BT_OVER)
					m_pFieldObj.back()->SetType(type);
				break;
			}

			default:
				break;
		}

		++cnt;	// 次のデータへ
	}

	return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : マップデータ破棄
//	Description : 現在保存しているマップデータを破棄する
//	Arguments   : id / ステージID
//	Returns     : 成否(true:成功)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CMapData::DeleteData()
{
	for(LPFIELDOBJECT_ARRAY_IT it = m_pFieldObj.begin(); it != m_pFieldObj.end(); ++it) {
		SAFE_RELEASE((*it));
	}
	for(LPCHARACTER_ARRAY_IT it = m_pLayoutObj.begin(); it != m_pLayoutObj.end(); ++it) {
		SAFE_RELEASE((*it));
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : フィールドオブジェクトリスト取得
//	Description : フィールドオブジェクトリストを取得する
//	Arguments   : pObjList / フィールドオブジェクトリストの格納先ポインタ
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CMapData::GetFieldObjList(LPFIELDOBJECT_ARRAY* pObjList)
{
	pObjList->resize(m_pFieldObj.size());
	std::copy(m_pFieldObj.begin(), m_pFieldObj.end(), pObjList->begin());
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : レイアウトオブジェクトリスト取得
//	Description : レイアウトオブジェクトリストを取得する
//	Arguments   : pObjList / レイアウトオブジェクトリストの格納先ポインタ
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CMapData::GetLayoutObjList(LPCHARACTER_ARRAY* pObjList)
{
	pObjList->resize(m_pLayoutObj.size());
	std::copy(m_pLayoutObj.begin(), m_pLayoutObj.end(), pObjList->begin());
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : クリア条件ブロック数取得
//	Description : クリア条件フィールドブロック数を取得する
//	Arguments   : None.
//	Returns     : クリア条件ブロック数
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
int CMapData::GetClearBlockNum()
{
	int num = 0;
	for(LPFIELDOBJECT_ARRAY_IT it = m_pFieldObj.begin(); it != m_pFieldObj.end(); ++it) {
		if((*it)->GetType() == BT_CLEAR)
			++num;
	}

	return num;
}


//========================================================================================
// private:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CMapData::CMapData()
{
	m_pFieldObj.reserve(INIT_OBJECT_NUM);
	m_pLayoutObj.reserve(INIT_OBJECT_NUM);
	m_startPoint = D3DXVECTOR2(0.0f, 0.0f);
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : デストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CMapData::~CMapData()
{
}


//========================================================================================
//	End of File
//========================================================================================