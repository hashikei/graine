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
	for(int i = 0; i < MAX_DATAPARAM; ++i) {
		if(!getline(ss, tmp, ',')) {
#ifdef _DEBUG_MESSAGEBOX
			MessageBox(hWnd, _T("MapData::map data error!"), _T("error"), MB_OK | MB_ICONERROR);
#endif
			return false;
		}
	}

	// データ読み込み
	int		cnt		= 0;
	float	width	= 0.0f;
	float	height	= 0.0f;
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
				CFieldObject* pFieldObj = CFieldObject::Create(ws);
				pFieldObj->Init();
				m_pFieldObj.push_back(pFieldObj);
				delete[] ws;
				break;
			}

			case DP_POSX:
				m_pFieldObj.back()->TranslateX(stof(tmp));
				break;

			case DP_POSY:
				m_pFieldObj.back()->TranslateY(stof(tmp));
				break;

			case DP_POSZ:
				m_pFieldObj.back()->TranslateZ(stof(tmp));
				break;

			case DP_WIDTH:
				width = stof(tmp);
				break;

			case DP_HEIGHT:
				height = stof(tmp);
				m_pFieldObj.back()->Resize(D3DXVECTOR2(width, height));
				break;

			case DP_ANGLE:
				m_pFieldObj.back()->RotateZ(stof(tmp));
				break;

			case DP_COLFLG:
			{
				stoi(tmp) > 0 ? m_pFieldObj.back()->EnableCol() : m_pFieldObj.back()->DisableCol();
				break;
			}

			case DP_TYPE:
				m_pFieldObj.back()->SetrType(stoi(tmp));
				break;

			default:
				break;
		}

		++cnt;	// 次のデータへ
	}

	return true;
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