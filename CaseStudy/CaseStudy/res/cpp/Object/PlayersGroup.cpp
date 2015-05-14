//========================================================================================
//		File        : PlayersGroup.h
//		Program     : プレイヤーども
//
//		Description : プレイヤーどもの動き,やばそう
//
//		History     : 2015/05/6	作成開始
//						   
//
//																Author : きんたまズ
//========================================================================================


//――――――――――――――――――――――――――――――――――――――――――――
// インクルード
//――――――――――――――――――――――――――――――――――――――――――――
#include "../../h/System/Input.h"
#include "../../h/Object/PlayersGroup.h"

//========================================================================================
// public:
//========================================================================================
// ――――――――――――――――――――――――――――――――――――――――――――
// using宣言
//――――――――――――――――――――――――――――――――――――――――――――
using namespace Input;
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : コンストラクタ
//	Arguments   : None.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CPlayersGroup::CPlayersGroup()
{
	m_nCurrentControllNo = 0;		// 操作するやつを先頭にセット
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 生成
//	Description : オブジェクトを生成する
//	Arguments   : pszFName / 読み込みファイル名
//	Returns     : オブジェクトデータ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CPlayersGroup* CPlayersGroup::Create(const LPCTSTR pszFName)
{
	// ----- 変数宣言
	CPlayersGroup* pObj;

	// ----- 初期化処理
	pObj = new CPlayersGroup();

	pObj->SetTexture(pszFName);

	return pObj;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 初期化
//	Description : いろんな初期化
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayersGroup::Init()
{
	
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 後始末
//	Description : いろんな後始末
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayersGroup::Uninit()
{
	// 要素全部削除
	for(m_listIt = m_list.begin(); m_listIt != m_list.end();)
	{
		// これな、UTSUWAがないと中いじれないの
		CPlayer* p = *m_listIt;

		// 後始末
		p->Uninit();

		// リストから要素を削除
		if( (*m_listIt) == 0 ) {
			 m_listIt = m_list.erase( m_listIt );
			 continue;
		}

		// これ絶対に最後な☆(てか今回これいらなけども)
		++m_listIt;
	}

	m_pField = NULL;

	CObject2D::Uninit();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 更新
//	Description : いろんな更新
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayersGroup::Update()
{
	int currentNo = 0;			// 今の番号
	CPlayer* Player = NULL;			// 操作するやつ

	// 要素全部更新
	for(m_listIt = m_list.begin(); m_listIt != m_list.end();)
	{
		// UTSUWAがないと中いじれないの
		CPlayer* p = *m_listIt;

		p->SetField(m_pField);

		// プレイヤーに現在の番号をセット
		p->SetNo(currentNo);
		// 現在の番号が操作番号と同じならPlayerを操作設定にそれ以外はその他設定に
		if(m_nCurrentControllNo == p->GetNo()){
			p->SetPlayerType(P_TYPE_PLAYER);
			Player = p;
		}else{
			p->SetPlayerType(P_TYPE_OTHER);
			// 操作するやつ設定
			p->SetPlayer(Player);
		}
		// 更新
		p->Update();

		if(p->GetNo() == 1)
			Player = p;
	
		// 番号を更新する
		++currentNo;

		// これ絶対に最後な☆
		++m_listIt;
	}

	// デバッグ用
	if (GetTrgKey(DIK_1)){		// 1
		AddPlayer();
	}
	if (GetTrgKey(DIK_2)){		// 2
		RedusePlayer();
	}
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : 描画
//	Description : いろんな更新
//	Arguments   : ないよ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayersGroup::Draw()
{
	// 要素全部描画
	for(m_listIt = m_list.begin(); m_listIt != m_list.end();)
	{
		// UTSUWAがないと中いじれないの
		CPlayer* p = *m_listIt;

		// 描画
		p->DrawAlpha();

		// これ絶対に最後な☆
		++m_listIt;
	}
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : Player情報取得
//	Description : Player情報取得
//	Arguments   : Playerの番号
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
CPlayer* CPlayersGroup::GetPlayer(int no)
{
	for(m_listIt = m_list.begin(); m_listIt != m_list.end();)
	{
		// UTSUWAがないと中いじれないの
		CPlayer* p = *m_listIt;

		// 識別番号が一緒だったら返す
		if(p->GetNo() == no){
			return p;
		}

		// これ絶対に最後な☆
		++m_listIt;
	}

	// なかったらNULLぽ
	return NULL;
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : Field情報
//	Description : 
//	Arguments   : Fieldのポインタ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayersGroup::SetField(CFieldObject* f)
{
	for(m_listIt = m_list.begin(); m_listIt != m_list.end();)
	{
		// UTSUWAがないと中いじれないの
		CPlayer* p = *m_listIt;

		m_pField = f;
		p->SetField(f);

		// これ絶対に最後な☆
		++m_listIt;
	}
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : Player追加
//	Description : Player追加
//	Arguments   : Playerのポインタ
//	Returns     : ないよ
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayersGroup::AddPlayer(CPlayer* p)
{
	m_list.push_back(p);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : Player追加
//	Description : 最後尾にPlayer追加
//	Arguments   : Playerのポインタ
//	Returns     : nai!
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayersGroup::AddPlayer()
{
	CPlayer* p;		// 追加するやつ
	// 生成
	p = CPlayer::Create(m_lpTex);
	// 初期化
	p->Init();

	// :::: リストに追加 ::::: //
	m_list.push_back(p);
}
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name        : Player削除
//	Description : 最後尾のPlayer削除
//	Arguments   : Playerのポインタ
//	Returns     : ない！
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayersGroup::RedusePlayer()
{
	// 1体しかいなかったら減らさない
	if(m_list.size() == 1)
		return;

	// UTSUWA
	CPlayer* p = m_list.back();
	// 後始末
	p->Uninit();
	// 開放
	SAFE_RELEASE(p)

	// :::: リストから削除 ::::: //
	m_list.pop_back();
}

//========================================================================================
//	End of File
//========================================================================================