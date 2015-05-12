//========================================================================================
//		File        : System.cpp
//		Program     : ゲーム共通データ/システム
//
//		Description : ゲームの共通データ及びシステムの実装
//
//		History     : 2013/12/05	作成開始
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


//========================================================================================
// namespace:
//========================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 2次元ベクトルの外積
//	Description : D3DXVECTOR2を用いた2次元ベクトルの外積を算出する
//	Arguments   : v1 / 1つめのベクトル
//				  v2 / 2つめのベクトル
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
float System::D3DXVec2Cross(const D3DXVECTOR2* v1, const D3DXVECTOR2* v2)
{
	return v1->x * v2->y - v1->y * v2->x;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : ファイル名検索
//	Description : 指定したパスのディレクトリ内にあるファイルを検索する
//				  【※関数内部でメモリ確保を行うため注意！】
//	Arguments   : pflist / ファイル名
//				  path   / 検索ディレクトリのパス
//	Returns     : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void System::SearchFileName(LPTSTR_ARRAY *pflist, LPCTSTR path)
{
	// ----- 変数宣言
	HANDLE			hFind;		// 検索ファイルハンドル
	WIN32_FIND_DATA	fd;			// データバッファ
	LPTSTR			tmpPath;	// パスバッファ

	// ----- パスのコピーを作成
	tmpPath = new TCHAR[_MAX_PATH];
	lstrcpy(tmpPath, path);

	// ----- 最初のファイルを検索
	hFind = FindFirstFile(tmpPath, &fd);
	if(hFind == INVALID_HANDLE_VALUE)
	{
#ifdef _DEBUG_MESSAGEBOX
		::MessageBox(NULL, _T("System::SearchFileName FindFirstFile Error!"), _T("error"), MB_OK);
#endif
		FindClose(hFind);
		return;
	}
	tmpPath[lstrlen(tmpPath) - 1] = '\0';	// ワイルドカードを削除

	// ----- 次のファイルを検索
	do
	{
		// ディレクトリなら再検索
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			lstrcmp(fd.cFileName, _T(".")) != 0 && lstrcmp(fd.cFileName, _T("..")) != 0)
		{
			LPTSTR subPath = new TCHAR[_MAX_PATH];						// メモリ確保
			wsprintf(subPath, _T("%s%s\\*"), tmpPath, fd.cFileName);	// 文字列結合
			SearchFileName(pflist, subPath);							// サブディレクトリ下を検索
			SAFE_DELETE_ARRAY(subPath);									// メモリ解放
		}

		// ファイルならパスを記録
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			LPTSTR filePath = new TCHAR[_MAX_PATH];
			wsprintf(filePath, _T("%s%s"), tmpPath, fd.cFileName);
			pflist->push_back(filePath);
		}
	} while(FindNextFile(hFind, &fd));
	
	// ----- 事後処理
	SAFE_DELETE_ARRAY(tmpPath);
	FindClose(hFind);
}


//========================================================================================
//	End of File
//========================================================================================