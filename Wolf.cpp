#include "Wolf.h"
#include"Field.h"
#include"Camera.h"
#include"ItemBox.h"
#include"PlayScene.h"
#include"Player.h"

namespace {
	const int WALK_MAXFRAME{ 11 };
	const int ATTACK_MAXFRAME{ 9 };
	const int DEAD_MAXFRAME{ 2 };

	const float WOLF_WIDTH = 128.0f;
	const float WOLF_HEIGHT = 128.0f;

	const float MOVE_SPEED = 1.5f;
	const float GRAVITY = 9.8f / 60.0f;//�d�͉����x
	
	const float CORRECT_WIDTH = 35.0f;
	const float CORRECT_BOTTOM = 1.0f;
	const float CORRECT_TOP = 35.0f;
	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;
	const float COLLIDE_WIDTH = 40.0f;
	const float COLLIDE_HEIGHT = 70.0f;
}

Wolf::Wolf(GameObject* parent):Enemy(parent)
{
	hAnimImg = LoadGraph("Assets/Enemy/Wolf2.png");
	assert(hAnimImg > 0);

	animType = 0;
	animFrame = 0;
	frameCounter = 0;

	isRight = false;
	onGround = true;
	jumpSpeed = 0.0f;
	cdTimer = 0.0f;
	accel = 0.0f;
	attackRight = false;

	state = NORMAL;
}

Wolf::~Wolf()
{
	if (hAnimImg > 0) {
		DeleteGraph(hAnimImg);
	}
}

void Wolf::Update()
{
	Field* pField = GetParent()->FindGameObject<Field>();
	std::list<ItemBox*> pIBoxs = GetParent()->FindGameObjects<ItemBox>();
	Player* pPlayer = GetParent()->FindGameObject<Player>();

	PlayScene* scene = dynamic_cast<PlayScene*>(GetParent());
	if (!scene->CanMove())
		return;

	float x = (int)transform_.position_.x;
	float y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	if (x > WINDOW_WIDTH+ WOLF_WIDTH)//��ʊO�ɓG�������瓮�����Ȃ�
		return;
	else if (x < -WOLF_WIDTH) {
		return;
	}
	if (y < 0 || y > WINDOW_HEIGHT)
		KillMe();

	if (state == DEAD) {
		if (animFrame < DEAD_MAXFRAME - 1) {
			frameCounter++;
			if (frameCounter > 10) {
				animFrame = (animFrame + 1) % DEAD_MAXFRAME;
				frameCounter = 0;
			}
		}
		else
			cdTimer++;
		if(cdTimer>100.0f)
			KillMe();
	}


	if (state == NORMAL) {
		frameCounter++;
		if (frameCounter > 6) {
			animFrame = (animFrame + 1) % WALK_MAXFRAME;
			frameCounter = 0;
		}
		if (isRight) {
			transform_.position_.x += MOVE_SPEED;
		}
		else {
			transform_.position_.x -= MOVE_SPEED;
		}
		if (pPlayer != nullptr) {
			if (ViewInPlayer(pPlayer->GetPosition())) {
				attackRight = isRight;
				targetPos = pPlayer->GetPosition();
				animType = 1;
				animFrame = 0;
				state = ATTACK;
			}
		}
	}

	if (state == ATTACK) {
		float len = targetPos.x - transform_.position_.x;
		frameCounter++;
		if (frameCounter > 6) {
			animFrame = (animFrame + 1) % ATTACK_MAXFRAME;
			frameCounter = 0;
		}
		if (attackRight) {
			if (len > 0) {
				accel += 0.05f;
				transform_.position_.x += accel;
			}
			else {
				accel -= 0.1f;
				transform_.position_.x += accel;
			}
		}
		else {
			if (len < 0) {
				accel += 0.05f;
				transform_.position_.x -= accel;
			}
			else {
				accel -= 0.1f;
				transform_.position_.x -= accel;
			}
		}
		if (accel <= 0.0f) {
			accel = 0.0f;
			animFrame = 0;
			animType = 0;
			state = NORMAL;
		}
	}
	
	if (pField != nullptr) {
		float cx = WOLF_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = WOLF_HEIGHT / 2.0f;

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

		jumpSpeed += GRAVITY;
		transform_.position_.y += jumpSpeed;

		int pushRbottom = pField->CollisionDown(transform_.position_.x + cx - 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushLbottom = pField->CollisionDown(transform_.position_.x - cx + 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushBottom = max(pushRbottom, pushLbottom);//2�̑����̂߂荞�݂̑傫���ق�
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
			jumpSpeed = 0.0f;
		}

		int pushRtop = pField->CollisionUp(transform_.position_.x + cx - 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushLtop = pField->CollisionUp(transform_.position_.x - cx + 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushTop = max(pushRtop, pushLtop);//2�̓��̂߂荞�݂̑傫���ق�
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			jumpSpeed = 0.0f;
		}
	}

	for (ItemBox* pIBox : pIBoxs) {
		float cx = WOLF_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = WOLF_HEIGHT / 2.0f;

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

		//transform_.position_.y += GRAVITY;

		int pushRbottom = pIBox->CollisionDown(transform_.position_.x + cx - 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushLbottom = pIBox->CollisionDown(transform_.position_.x - cx + 1.0f, transform_.position_.y + cy - CORRECT_BOTTOM);
		int pushBottom = max(pushRbottom, pushLbottom);//2�̑����̂߂荞�݂̑傫���ق�
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
			jumpSpeed = 0.0f;
		}

		int pushRtop = pIBox->CollisionUp(transform_.position_.x + cx - 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushLtop = pIBox->CollisionUp(transform_.position_.x - cx + 1.0f, transform_.position_.y - cy + CORRECT_TOP);
		int pushTop = max(pushRtop, pushLtop);//2�̓��̂߂荞�݂̑傫���ق�
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			jumpSpeed = 0.0f;
		}
	}
}

void Wolf::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValueX();
		y -= cam->GetValueY();
	}
	//DrawRotaGraph(x, y,1.0f, 0, hWalkImage[animFrame], TRUE,!isRight);
	DrawRectGraph(x-WOLF_WIDTH/2.0, y - WOLF_HEIGHT / 2.0-CORRECT_BOTTOM, animFrame*WOLF_WIDTH, animType*WOLF_HEIGHT, WOLF_WIDTH, WOLF_HEIGHT, hAnimImg, TRUE, !isRight);
}

void Wolf::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}

bool Wolf::CollideRectToRect(float x, float y, float w, float h)
{
	if (state != DEAD) {
		if (state == NORMAL) {
			// �G�̋�`�͈̔͂��v�Z
			float myRectRight = transform_.position_.x + WOLF_WIDTH / 2.0f - COLLIDE_WIDTH;
			float myRectLeft = transform_.position_.x - WOLF_WIDTH / 2.0f + COLLIDE_WIDTH;
			float myRectTop = transform_.position_.y - WOLF_HEIGHT / 2.0f + COLLIDE_HEIGHT;
			float myRectBottom = transform_.position_.y + WOLF_HEIGHT / 2.0f - CORRECT_BOTTOM;

			// �w�肳�ꂽ��`�ƓG�̋�`���������Ă��邩�`�F�b�N
			if ((x - w / 2.0f < myRectRight && x + w / 2.0f > myRectLeft) &&
				(y - h / 2.0f < myRectBottom && y + h / 2.0f > myRectTop)) {
				return true;
			}
			else
				return false;
		}
		else if (state == ATTACK) {
			// �G�̋�`�͈̔͂��v�Z
			float myRectRight = transform_.position_.x + WOLF_WIDTH / 2.0f - COLLIDE_WIDTH/2.0f;
			float myRectLeft = transform_.position_.x - WOLF_WIDTH / 2.0f + COLLIDE_WIDTH/2.0f;
			float myRectTop = transform_.position_.y;
			float myRectBottom = transform_.position_.y + WOLF_HEIGHT / 2.0f - CORRECT_BOTTOM;

			// �w�肳�ꂽ��`�ƓG�̋�`���������Ă��邩�`�F�b�N
			if ((x - w / 2.0f < myRectRight && x + w / 2.0f > myRectLeft) &&
				(y - h / 2.0f < myRectBottom && y + h / 2.0f > myRectTop)) {
				return true;
			}
			else
				return false;
		}
	}
	else
		return false;
}

void Wolf::KillEnemy()
{
	state = DEAD;
	animType = 2;
	animFrame = 0;
	frameCounter = 0;
}

bool Wolf::IsSteppedOnHead(float x, float y, float w, float h)
{
	if (state != DEAD) {
		// �G�̋�`�͈̔͂��v�Z
		float myHeadRight = transform_.position_.x + WOLF_WIDTH / 2.0f - COLLIDE_WIDTH;
		float myHeadLeft = transform_.position_.x - WOLF_WIDTH / 2.0f + COLLIDE_WIDTH;
		float myHeadTop = transform_.position_.y - WOLF_HEIGHT / 2.0f + COLLIDE_HEIGHT;
		float myHeadBottom = transform_.position_.y - 30.0f;

		// �w�肳�ꂽ��`�ƓG�̋�`���������Ă��邩�`�F�b�N
		if ((x - w / 2.0f < myHeadRight && x + w / 2.0f > myHeadLeft) &&
			(y - h / 2.0f < myHeadBottom && y + h / 2.0f > myHeadTop)) {
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool Wolf::ViewInPlayer(XMFLOAT3 pos)
{
	float lenX = pos.x - transform_.position_.x;
	float lenY = pos.y - transform_.position_.y;
	float len = sqrt((lenX * lenX) + (lenY * lenY));
	if (len < 200.0f) {
		XMVECTOR playerVec = XMLoadFloat3(&pos);
		playerVec = XMVector3Normalize(playerVec);
		XMVECTOR eVec = XMLoadFloat3(&transform_.position_);
		eVec = XMVector3Normalize(eVec);
		XMVECTOR dot = XMVector3Dot(playerVec, eVec);
		float c = XMVectorGetX(dot);
		float angle = acos(c);
		if (c <= 15.0f && c >= -15.0f)
			return true;
		else
			return false;
	}
	return false;
}