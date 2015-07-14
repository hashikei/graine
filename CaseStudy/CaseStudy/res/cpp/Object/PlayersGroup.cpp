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
#include "../../h/Scene/GameMain.h"
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
	m_bOver = false;
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
	for(m_listIt = m_list.begin(); m_listIt != m_list.end();)
	{
		// これな、UTSUWAがないと中いじれないの
		CPlayer* p = *m_listIt;

		// 初期化
		p->Init();

		// これ絶対に最後な☆(てか今回これいらなけども)
		++m_listIt;
	}

	if(m_list.size() == 0)
		AddPlayer(D3DXVECTOR3(CMapData::GetStartPoint().x,CMapData::GetStartPoint().y,0));

	m_bOver = false;
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

		p->Uninit();
		SAFE_RELEASE(p)
		m_listIt = m_list.erase( m_listIt );
		continue;
		// これ絶対に最後な☆(てか今回これいらなけども)
		++m_listIt;
	}

	m_nCurrentControllNo = 0;
	m_pStage = NULL;

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

	// 投げ用
	int			throwNo = 0;

	if (GetTrgKey(DIK_UP)){
		CGameMain::PlaySE(SE_RIDE);
		if(m_list.size() > (unsigned int)m_nCurrentControllNo + 1){
			CGameMain::PlaySE(SE_RIDE);
			// 着地時のみ限定
			if(!(GetPlayer(m_nCurrentControllNo + 1)->GetStatus() & ST_FLYING))
				m_nCurrentControllNo++;
		}
	}
	if(GetTrgKey(DIK_DOWN)){
		if(m_nCurrentControllNo > 0){
			for(int i = 0;i < m_nCurrentControllNo;i++){
				GetPlayer(i)->EnableDelete();
			}
			switch(m_nCurrentControllNo)
			{
			case PLAYER_ARROW:
				GetPlayer(m_nCurrentControllNo)->SetGrane(PLAYER_ARROW);
				break;
			case PLAYER_JACK:
				GetPlayer(m_nCurrentControllNo)->SetGrane(PLAYER_JACK);
				break;
			case PLAYER_STONE:
				GetPlayer(m_nCurrentControllNo)->SetGrane(PLAYER_STONE);
				break;
			}
			m_nCurrentControllNo = 0;
		}
	}

	bool bThrow =false;
	if (GetTrgKey(DIK_Z)){
		bThrow = true;
	}

	if(m_list.size() == 0)
	{
		m_bOver = true;
		return ;
	}

	// 要素全部更新
	for(m_listIt = m_list.begin(); m_listIt != m_list.end();)
	{
		// UTSUWAがないと中いじれないの
		CPlayer* p = *m_listIt;

		p->SetStage(m_pStage);

		// プレイヤーに現在の番号をセット
		p->SetNo(currentNo);

		// 現在の番号が操作番号と同じならPlayerを操作設定にそれ以外はその他設定に
		if(p->GetNo() == m_nCurrentControllNo){
			p->SetType(P_TYPE_PLAYER);
			Player = p;
		}

		if(p->GetType() != P_TYPE_WAIT){
			// 後ろに付いてくる奴ら
			if(p->GetNo() > m_nCurrentControllNo){
				p->SetType(P_TYPE_OTHER);
				// 操作するやつ設定
				for(unsigned int i = 1;i < m_list.size() + 1;i++)
				{
					if(GetPlayer(p->GetNo() - i)->GetType() != P_TYPE_WAIT){
						p->SetPlayer(GetPlayer(p->GetNo() - i));
						Player = p;
						break;
					}
				}
			}
			// 投げる連中
			if(p->GetNo() < m_nCurrentControllNo){
				if(p->GetType() == P_TYPE_THROW_READY_READY){
				
				}
				if(p->GetType() == P_TYPE_PLAYER){
					// 追従するプレイヤーを後ろの奴に
					Player = GetPlayer(p->GetNo() + 1);
					p->SetPlayer(Player);
					p->SetType(P_TYPE_THROW_READY_READY);
					p->SetLastTime();
				}
				if(bThrow){
					for(int i = 0;i < m_nCurrentControllNo;++i){
						if(p->GetType() == P_TYPE_THROW_READY){
							p->SetType(P_TYPE_THROW);
							bThrow = false;
						}
					}
				}
			}
		}
		// 更新
		p->Update();

		// 落ちたら削除
		if(p->GetPosY() < -1000){
			p->EnableDelete();
		}

		if(p->GetDelete()){
			// 操作するやつの場合他の奴を操作設定にする
			switch(p->GetType())
			{
			case P_TYPE_PLAYER:
				if(m_nCurrentControllNo > 0){
					m_nCurrentControllNo--;
					GetPlayer(m_nCurrentControllNo)->SetType(P_TYPE_PLAYER);
				}
				else if(GetPlayer(m_nCurrentControllNo + 1)){
					GetPlayer(m_nCurrentControllNo + 1)->SetType(P_TYPE_PLAYER);
				}
				break;
			case P_TYPE_OTHER:
				break;
			case P_TYPE_THROW_READY_READY:
				break;
			case P_TYPE_THROW_READY:
				break;
			case P_TYPE_THROW:
				m_nCurrentControllNo--;
				break;
			case P_TYPE_FLOWER:
				m_nCurrentControllNo--;
				break;
			}
			// 削除
			p->Uninit();
			SAFE_RELEASE(p)
			m_listIt = m_list.erase(m_listIt);
			continue;
		}
		
		if( p->GetType() == P_TYPE_FLOWER){
		
		}

		// 番号を更新する
		++currentNo;

		// これ絶対に最後な☆
		++m_listIt;
	}

	// デバッグ用
	if (GetTrgKey(DIK_1)){		// 1
		if(m_list.size() < 9){
		AddPlayer(D3DXVECTOR3(GetPlayer(m_nCurrentControllNo)->GetPosition().x,
			GetPlayer(m_nCurrentControllNo)->GetPosition().y + 100,
			GetPlayer(m_nCurrentControllNo)->GetPosition().z));
		}
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
		p->Draw();

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
	for(std::list<CPlayer*>::iterator it = m_list.begin(); it != m_list.end();)
	{
		// UTSUWAがないと中いじれないの
		CPlayer* p = *it;

		// 識別番号が一緒だったら返す
		if(p->GetNo() == no){
			return p;
		}

		// これ絶対に最後な☆
		++it;
	}

	// なかったらNULLぽ
	return NULL;
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
//	Name        : Player追加
//	Description : 最後尾に座標
//	Arguments   : Playerのポインタ
//	Returns     : nai!
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void CPlayersGroup::AddPlayer(D3DXVECTOR3 pos)
{
	CPlayer* p;	// 追加するやつ
	// 生成
	p = CPlayer::Create(m_lpTex);
	// 初期化
	p->Init(pos);
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