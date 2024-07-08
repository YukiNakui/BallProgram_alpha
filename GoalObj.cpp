#include "GoalObj.h"
#include"Camera.h"
#include"Field.h"
#include"Engine/SceneManager.h"

namespace {
	const int GOAL_MAXFRAME = 3;
	const int GOAL_WIDTH = 515;
	const int GOAL_HEIGHT = 384;
	const float CORRECT_LEFT = 150;
	const float CORRECT_RIGHT = 40.0f;
	const float CORRECT_X = 277;
	const float CORRECT_Y = 16.0f;
}

GoalObj::GoalObj(GameObject* scene)
{
	/*LoadDivGraph("Assets/Goal.png", GAF::GOAL_MAXFRAME, GAF::GOAL_MAXFRAME, 1, IMAGE_WIDTH, IMAGE_HEIGHT, hImage);
	for (int i = 0; i < GAF::GOAL_MAXFRAME; i++) {
		assert(hImage[i] > 0);
	}*/
	hAnimImage = LoadGraph("Assets/Goal.png");
	assert(hAnimImage > 0);

	animFrame = 0;
	//animType = 0;
	frameCounter = 0;
	cdTimer = 0.0f;

	state = NOTBRAKE;
	UpdateRect();
}

GoalObj::~GoalObj()
{
	/*for (int i = 0; i < GAF::GOAL_MAXFRAME; i++) {
		if (hImage[i] > 0)
		{
			DeleteGraph(hImage[i]);
		}
	}*/
	if (hAnimImage > 0)
	{
		DeleteGraph(hAnimImage);
	}
}

void GoalObj::Update()
{
	Field* pField = GetParent()->FindGameObject<Field>();
	if (state == BRAKE) {//�S�[��������
		if (animFrame < GOAL_MAXFRAME - 1) {
			frameCounter++;
			if (frameCounter > 60) {
				animFrame = (animFrame + 1) % GOAL_MAXFRAME;
				frameCounter = 0;
			}
		}
		else {
			cdTimer++;
			if (cdTimer > 60.0f) {
				if (pField->CanNextStageChange()) {//���̃X�e�[�W����������
					pField->SetNextStage();//�X�e�[�W�ԍ���i�߂�
					pField->Reset();//�X�e�[�W�̍ēǂݍ���
					state = NOTBRAKE;
				}
				else {//���̃X�e�[�W���Ȃ�������N���A�V�[����
					SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
					pSceneManager->ChangeScene(SCENE_ID_CLEAR);
				}
				animFrame = 0;
				frameCounter = 0;
				cdTimer = 0;
			}
		}
	}

	UpdateRect();
}

void GoalObj::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	//DrawRotaGraph(x, y, 1.0f, 0, hImage[animFrame], TRUE);
	DrawRectGraph(x - GOAL_WIDTH / 2.0, y - GOAL_HEIGHT / 2.0, animFrame * GOAL_WIDTH, 0, GOAL_WIDTH, GOAL_HEIGHT, hAnimImage, TRUE);

	// �f�o�b�O�p�̓����蔻��̋�`��`��
    //unsigned int Cr = GetColor(0, 0, 255); // �F�̒l���擾
    //DrawBox((int)gRectLeft, (int)gRectTop, (int)gRectRight, (int)gRectBottom, Cr, FALSE); // �l�p�`��`��
}

void GoalObj::SetPosition(int x, int y)
{
	transform_.position_.x = x - CORRECT_X;
	transform_.position_.y = y - CORRECT_Y;
	UpdateRect();
}

bool GoalObj::CollideRectToRect(float x, float y, float w, float h)
{
	if (x - w / 2.0f < gRectRight && x + w / 2.0f > gRectLeft) {
		if (y - h / 2.0f < gRectBottom && y + h / 2.0f > gRectTop) {
			return true;
		}
	}
	return false;
}

void GoalObj::Goal()
{
	animFrame = 0;
	state = BRAKE;
}

void GoalObj::UpdateRect()
{
	gRectTop = transform_.position_.y - GOAL_HEIGHT / 2.0f;
	gRectBottom = transform_.position_.y + GOAL_HEIGHT / 2.0f - CORRECT_Y;
	gRectLeft = transform_.position_.x + CORRECT_LEFT;
	gRectRight = transform_.position_.x + GOAL_WIDTH / 2.0f - CORRECT_RIGHT;
}
