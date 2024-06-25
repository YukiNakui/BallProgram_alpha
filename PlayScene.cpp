#include"PlayScene.h"
#include"Player.h"
#include"GoalObj.h"
#include"Field.h"
#include"Camera.h"
#include"BackGround.h"

//コンストラクタ
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene")
{
}

//初期化 
void PlayScene::Initialize()
{
	Instantiate<Camera>(this);
	Instantiate<BackGround>(this);
	Field* pField = Instantiate<Field>(this);
	Instantiate<GoalObj>(this);
	Instantiate<Player>(this);
	pField->Reset();
}

//更新
void PlayScene::Update()
{
	switch (state) {
	case s_Ready:
		UpdateReady();
		break;
	case s_Play:
		UpdatePlay();
		break;
	case s_Clear:
		UpdateClear();
		break;
	case s_Dead:
		UpdateDead();
		break;
	}
}

//描画
void PlayScene::Draw()
{
}

//開放
void PlayScene::Release()
{
}

bool PlayScene::CanMove()
{
	return (state == s_Play);
}

void PlayScene::StartReady()
{
}

void PlayScene::UpdateReady()
{
}

void PlayScene::StartPlay()
{
}

void PlayScene::UpdatePlay()
{
}

void PlayScene::UpdateClear()
{
}

void PlayScene::StartDead()
{
}

void PlayScene::UpdateDead()
{
}
