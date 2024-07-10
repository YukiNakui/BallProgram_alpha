#include"PlayScene.h"
//#include"Player.h"
//#include"GoalObj.h"
////#include"Field.h"
//#include"Camera.h"
//#include"BackGround.h"

//�R���X�g���N�^
PlayScene::PlayScene(GameObject * parent)
	: GameObject(parent, "PlayScene")
{
}

//������ 
void PlayScene::Initialize()
{
	pCam = Instantiate<Camera>(this);
	pBG = Instantiate<BackGround>(this);
	pField = Instantiate<Field>(this);
	pGObj = Instantiate<GoalObj>(this);
	pPlayer = Instantiate<Player>(this);
	StartReady();
}

//�X�V
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
	case s_Stop:
		UpdateStop();
		break;
	case s_Dead:
		UpdateDead();
		break;
	}
}

//�`��
void PlayScene::Draw()
{
}

//�J��
void PlayScene::Release()
{
}

bool PlayScene::CanMove()
{
	return (state == s_Play);
}

void PlayScene::GameObjectsReset()
{
	if (pCam != nullptr) {
		pCam = nullptr;
	}
	if (pBG != nullptr) {
		pBG = nullptr;
	}
	if (pField != nullptr) {
		pField = nullptr;
	}
	if (pGObj != nullptr) {
		pGObj = nullptr;
	}
	if (pPlayer != nullptr) {
		pPlayer = nullptr;
	}
	if (pField != nullptr) {
		pField = nullptr;
	}
}

void PlayScene::StartReady()
{
	state = s_Ready;
	pField->Reset();
	readyTimer = 2.0f;
}

void PlayScene::StartClear()
{
	state = s_Clear;
}

void PlayScene::StartStop(float time)
{
	pCam->VibrationX(100.0f);
	state = s_Stop;
	timerEnd = time;
}

void PlayScene::UpdateReady()
{
	readyTimer -= 1.0f / 60.0f;
	if (readyTimer <= 0.0f) {
		StartPlay();
	}
}

void PlayScene::StartPlay()
{
	state = s_Play;
}

void PlayScene::UpdatePlay()
{
}

void PlayScene::UpdateClear()
{
	StartReady();
	pField->SetNextStage();//�X�e�[�W�ԍ���i�߂�
	pField->Reset();//�X�e�[�W�̍ēǂݍ���
}

void PlayScene::UpdateStop()
{
	readyTimer -= 1.0f / 60.0f;
	if (readyTimer <= timerEnd) {
		StartPlay();
	}
}

void PlayScene::StartDead()
{
	pCam->VibrationX(100.0f);
	state = s_Dead;
	readyTimer = 1.0f;
}

void PlayScene::UpdateDead()
{
	readyTimer -= 1.0f / 60.0f;
	if (readyTimer <= 0.0f) {
		StartPlay();
	}
}
