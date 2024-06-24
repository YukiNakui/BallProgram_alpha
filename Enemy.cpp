#include "Enemy.h"
#include"Field.h"
#include"Camera.h"
#include"ItemBox.h"

namespace {
	const float ENEMY_WIDTH = 128.0f;
	const float ENEMY_HEIGHT = 128.0f;
	const float MOVE_SPEED = 1.5f;
	const float GRAVITY = 15.0f / 60.0f;//�d�͉����x
	const float CORRECT_WIDTH = 35.0f;
	const float CORRECT_BOTTOM = 1.0f;
	const float CORRECT_TOP = 35.0f;
	static const int SCREEN_WIDTH = 1280;
}

Enemy::Enemy(GameObject* scene)
{
	LoadDivGraph("Assets/EnemyWalk.png", EAF::WALK_MAXFRAME, EAF::WALK_MAXFRAME, 1, 128, 128, hWalkImage);
	for (int i = 0; i < EAF::WALK_MAXFRAME; i++) {
		assert(hWalkImage[i] > 0);
	}
	hWalkImage[EAF::DEAD_MAXFRAME];
	LoadDivGraph("Assets/EnemyDead.png", EAF::DEAD_MAXFRAME, EAF::DEAD_MAXFRAME, 1, 128, 128, hDeadImage);
	for (int i = 0; i < EAF::DEAD_MAXFRAME; i++) {
		assert(hDeadImage[i] > 0);
	}

	animFrame = 0;
	frameCounter = 0;
	isRight = false;
}

Enemy::~Enemy()
{
	for (int i = 0; i < EAF::WALK_MAXFRAME; i++) {
		if (hWalkImage[i] > 0)
		{
			DeleteGraph(hWalkImage[i]);
		}
	}
}

void Enemy::Update()
{
	Field* pField = GetParent()->FindGameObject<Field>();
	std::list<ItemBox*> pIBoxs = GetParent()->FindGameObjects<ItemBox>();

	int x = (int)transform_.position_.x;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
	}
	if (x > SCREEN_WIDTH)//��ʊO�ɓG�������瓮�����Ȃ�
		return;
	else if (x < -64) {
		KillMe();
		return;
	}

	frameCounter ++;
	if (frameCounter > 6) {
		animFrame = (animFrame + 1) % EAF::WALK_MAXFRAME;
		frameCounter = 0;
	}
	if (isRight) {
		transform_.position_.x += MOVE_SPEED;
	}
	else {
		transform_.position_.x -= MOVE_SPEED;
	}

	

	if (pField != nullptr) {
		float cx = ENEMY_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = ENEMY_HEIGHT / 2.0f;

		float pushTright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushRight = max(pushBright, pushTright);//�E���̓��Ƒ����œ����蔻��
		if (pushRight > 0.0f) {
			isRight = false;
			transform_.position_.x -= pushRight - 1.0f;
		}

		float pushTleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushLeft = max(pushBleft, pushTleft);//�����̓��Ƒ����œ����蔻��
		if (pushLeft > 0.0f) {
			isRight = true;
			transform_.position_.x += pushLeft - 1.0f;
		}

		transform_.position_.y += GRAVITY;

		int pushRbottom = pField->CollisionDown(transform_.position_.x + cx - 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushLbottom = pField->CollisionDown(transform_.position_.x - cx + 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushBottom = max(pushRbottom, pushLbottom);//2�̑����̂߂荞�݂̑傫���ق�
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
		}

		int pushRtop = pField->CollisionUp(transform_.position_.x + cx - 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushLtop = pField->CollisionUp(transform_.position_.x - cx + 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushTop = max(pushRtop, pushLtop);//2�̓��̂߂荞�݂̑傫���ق�
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			
		}
	}
	for (ItemBox* pIBox : pIBoxs) {
		float cx = ENEMY_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = ENEMY_HEIGHT / 2.0f;

		float pushTright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushRight = max(pushBright, pushTright);//�E���̓��Ƒ����œ����蔻��
		if (pushRight > 0.0f) {
			isRight = false;
			transform_.position_.x -= pushRight - 1.0f;
		}

		float pushTleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + cy - CORRECT_TOP - 1.0f);
		float pushBleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - cy + CORRECT_BOTTOM + 1.0f);
		float pushLeft = max(pushBleft, pushTleft);//�����̓��Ƒ����œ����蔻��
		if (pushLeft > 0.0f) {
			isRight = true;
			transform_.position_.x += pushLeft - 1.0f;
		}

		transform_.position_.y += GRAVITY;

		int pushRbottom = pIBox->CollisionDown(transform_.position_.x + cx - 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushLbottom = pIBox->CollisionDown(transform_.position_.x - cx + 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushBottom = max(pushRbottom, pushLbottom);//2�̑����̂߂荞�݂̑傫���ق�
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
		}

		int pushRtop = pIBox->CollisionUp(transform_.position_.x + cx - 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushLtop = pIBox->CollisionUp(transform_.position_.x - cx + 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushTop = max(pushRtop, pushLtop);//2�̓��̂߂荞�݂̑傫���ق�
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;

		}
	}
}

void Enemy::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	DrawRotaGraph(x, y,1.0f, 0, hWalkImage[animFrame], TRUE,!isRight);
}

void Enemy::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}

bool Enemy::CollideRectToRect(float x, float y, float w, float h)
{
	// �G�̋�`�͈̔͂��v�Z
	float myRectRight = transform_.position_.x + ENEMY_WIDTH / 2.0f - CORRECT_WIDTH;
	float myRectLeft = transform_.position_.x - ENEMY_WIDTH / 2.0f + CORRECT_WIDTH;
	float myRectTop = transform_.position_.y - ENEMY_HEIGHT / 2.0f + CORRECT_TOP;
	float myRectBottom = transform_.position_.y + ENEMY_HEIGHT / 2.0f - CORRECT_BOTTOM;

	// �w�肳�ꂽ��`�ƓG�̋�`���������Ă��邩�`�F�b�N
	if (x - w / 2.0f < myRectRight && x + w / 2.0f > myRectLeft) {
		if (y - h / 2.0f < myRectBottom && y + h / 2.0f > myRectTop) {
			return true;
		}
	}
	return false;
}
