#include "TestScene.h"
#include"Player.h"
#include"Field.h"
#include"Camera.h"
#include"BackGround.h"

//�R���X�g���N�^
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//������ 
void TestScene::Initialize()
{
	Instantiate<Camera>(this);
	Instantiate<BackGround>(this);
	Field* pField = Instantiate<Field>(this);
	Instantiate<Player>(this);
	pField->Reset();
}

//�X�V
void TestScene::Update()
{
}

//�`��
void TestScene::Draw()
{
}

//�J��
void TestScene::Release()
{
}
