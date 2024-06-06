#include "TestScene.h"
#include"Player.h"
#include"Field.h"

//�R���X�g���N�^
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//������
void TestScene::Initialize()
{
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
