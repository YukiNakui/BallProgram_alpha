#include "Player.h"
#include <DxLib.h>
#include <assert.h>
#include"Camera.h"
#include"Field.h"
#include"TestScene.h"
#include"Ball.h"
#include"Enemy.h"
#include"ItemBox.h"

namespace {
	const float MOVE_SPEED = 2.5f;
	const float GROUND = 600.0f;
	const float JUMP_HIGHT = 64.0f * 4.0f;//�W�����v�̍���
	const float GRAVITY = 9.8f / 60.0f;//�d�͉����x
	const float PLAYER_WIDTH = 128.0f;
	const float PLAYER_HEIGHT = 128.0f;
	const float CORRECT_WIDTH = 50.0f;
	const float CORRECT_HEIGHT = 3.0f;
	const float CORRECT_TOP = 55.0f;
	const float CORRECT_BOTTOM = 3.0f;
};

Player::Player(GameObject* parent) : GameObject(sceneTop)
{
	/*hImage = LoadGraph("Assets/chara.png");
	assert(hImage > 0);*/
	LoadDivGraph("Assets/PlayerWalk.png", PAF::WALK_MAXFRAME, PAF::WALK_MAXFRAME, 1, 128, 128, hWalkImage);
	for (int i = 0; i < PAF::WALK_MAXFRAME; i++) {
		assert(hWalkImage[i] > 0);
	}

	transform_.position_.x = 128.0f;
	transform_.position_.y = GROUND;
	jumpSpeed = 0.0f;
	onGround = true;
	isRight = true;
	pBall = nullptr;
	isBallAlive = false;
	tossCount = 0;
	animFrame = 0;
	frameCounter = 0;
	/*frameCounter = 0;
	animType = 0;
	animFrame = 0;
	state = S_Walk;*/
}

Player::~Player()
{
	/*if (hImage > 0)
	{
		DeleteGraph(hImage);
	}*/
	for (int i = 0; i < PAF::WALK_MAXFRAME; i++) {
		if (hWalkImage[i] > 0)
		{
			DeleteGraph(hWalkImage[i]);
		}
	}
}

void Player::Update()
{
	Field* pField = GetParent()->FindGameObject<Field>();
	ItemBox* pIBox = GetParent()->FindGameObject<ItemBox>();
	/*if (state == S_Cry) {
		frameCounter++;
		if (frameCounter >= 16)
		{
			frameCounter = 0;
			animFrame = (animFrame + 1) % 2;
		}
		return;
	}
	TestScene* scene = dynamic_cast<TestScene*>(GetParent());
	if (!scene->CanMove())
		return;*/
	
	if (CheckHitKey(KEY_INPUT_K))
	{
		if (!prevAttackKey){
			if (!isBallAlive ||(pBall != nullptr && !pBall->IsBallAlive())) {
				pBall = nullptr;
				pBall = Instantiate<Ball>(GetParent());
				isBallAlive = true;
				pBall->SetPosition(transform_.position_.x, transform_.position_.y - PLAYER_HEIGHT / 2.0f + CORRECT_TOP);
				pBall->FirstToss();
				tossCount += 1;
			}
			else {
				if (tossCount > 0){
					if (IsTouchBall(pBall->GetPos())){
						if (tossCount == 1) {
							pBall->SecondToss();
								tossCount += 1;
						}
					}
					else{
						float lenX = pBall->GetPos().x - transform_.position_.x;
						float lenY = pBall->GetPos().y - transform_.position_.y;
						float len = sqrt(lenX * lenX + lenY * lenY);
						//�v���C���[�ƃ{�[������苗������Ă��āA���v���C���[�������̍����Ƀ{�[��������Ƃ�
						if (len > PLAYER_HEIGHT && pBall->GetPos().y <= transform_.position_.y - 64.0f) {
							SetPosition(pBall->GetPos().x, pBall->GetPos().y + PLAYER_HEIGHT/2.0f - CORRECT_TOP);
							pBall->Spike(isRight);
							tossCount = 0;
						}
					}
				}
				
			}
			prevAttackKey = true;
		}
	}
	else {
		prevAttackKey = false;
	}

	if (pBall!=nullptr){ 
		if (!pBall->IsBallAlive() || pBall->IsBallCatch(transform_.position_.x, transform_.position_.y + PLAYER_HEIGHT / 4.0f)) {
			pBall->SetPosition(0.0f, 800.0f);
			isBallAlive = false;
			pBall = nullptr;
			tossCount = 0;
		}
	}
	
	if (CheckHitKey(KEY_INPUT_D))
	{
		transform_.position_.x += MOVE_SPEED;
		float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = PLAYER_HEIGHT / 2.0f;
		if (++frameCounter >= 6) {
			animFrame = (animFrame + 1) % PAF::WALK_MAXFRAME;
			frameCounter = 0;
		}
		if (pField != nullptr) {
			float pushTright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y - (cy - CORRECT_TOP) + 1.0f);
			float pushBright = pField->CollisionRight(transform_.position_.x + cx, transform_.position_.y + (cy - CORRECT_BOTTOM) -1.0f);
			float pushRight = max(pushBright, pushTright);//�E���̓��Ƒ����œ����蔻��
			if (pushRight > 0.0f) {
				transform_.position_.x -= pushRight - 1.0f;
			}
		}
		if (pIBox != nullptr) {
			float pushTright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y - (cy - CORRECT_TOP) + 1.0f);
			float pushBright = pIBox->CollisionRight(transform_.position_.x + cx, transform_.position_.y + (cy - CORRECT_BOTTOM) - 1.0f);
			float pushRight = max(pushBright, pushTright);//�E���̓��Ƒ����œ����蔻��
			if (pushRight > 0.0f) {
				transform_.position_.x -= pushRight - 1.0f;
			}
		}
		isRight = true;
	}
	else if (CheckHitKey(KEY_INPUT_A))
	{
		float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH;
		float cy = PLAYER_HEIGHT / 2.0f;
		transform_.position_.x -= MOVE_SPEED;
		if (++frameCounter >= PAF::WALK_MAXFRAME) {
			animFrame = (animFrame + 1) % PAF::WALK_MAXFRAME;
			frameCounter = 0;
		}
		if (pField != nullptr) {
			float pushTleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - (cy - CORRECT_TOP) + 1.0f);
			float pushBleft = pField->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + (cy - CORRECT_BOTTOM) - 1.0f);
			float pushLeft = max(pushBleft, pushTleft);//�����̓��Ƒ����œ����蔻��
			if (pushLeft > 0.0f) {
				transform_.position_.x += pushLeft - 1.0f;
			}
		}
		if (pIBox != nullptr) {
			float pushTleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y - (cy - CORRECT_TOP) + 1.0f);
			float pushBleft = pIBox->CollisionLeft(transform_.position_.x - cx, transform_.position_.y + (cy - CORRECT_BOTTOM) - 1.0f);
			float pushLeft = max(pushBleft, pushTleft);//�����̓��Ƒ����œ����蔻��
			if (pushLeft > 0.0f) {
				transform_.position_.x += pushLeft - 1.0f;
			}
		}
		isRight = false;
	}
	else {
		animFrame = 0;
		frameCounter = 0;
	}

	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		if (prevSpaceKey == false) {
			if (onGround) {
				jumpSpeed = -sqrt(2 * GRAVITY * JUMP_HIGHT);
				onGround = false;
				animType = 2;
			}
		}
		prevSpaceKey = true;
	}
	else {
		prevSpaceKey = false;
	}

	jumpSpeed += GRAVITY; //���x += �����x
	transform_.position_.y += jumpSpeed; //���W += ���x

	/*if (animType == 2)
	{
		if (jumpSpeed < 0) {
			animFrame = 1;
		}
		else {
			animFrame = 2;
		}
	}*/

	if (pField != nullptr) {
		float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH-5.0f;
		float cy = PLAYER_HEIGHT / 2.0f;
		float pushRbottom = pField->CollisionDown(transform_.position_.x + cx, transform_.position_.y + (cy - CORRECT_BOTTOM));
		float pushLbottom = pField->CollisionDown(transform_.position_.x - cx, transform_.position_.y + (cy - CORRECT_BOTTOM));
		float pushBottom = max(pushRbottom, pushLbottom);//2�̑����̂߂荞�݂̑傫���ق�
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
			jumpSpeed = 0.0f;
			onGround = true;
			animType = 0;
		}
		else {
			onGround = false;
		}
		float pushRtop = pField->CollisionUp(transform_.position_.x + cx, transform_.position_.y - (cy - CORRECT_TOP));
		float pushLtop = pField->CollisionUp(transform_.position_.x - cx, transform_.position_.y - (cy - CORRECT_TOP));
		float pushTop = max(pushRtop, pushLtop);//2�̓��̂߂荞�݂̑傫���ق�
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			jumpSpeed = 0.0f;
		}
	}
	if (pIBox != nullptr) {
		float cx = PLAYER_WIDTH / 2.0f - CORRECT_WIDTH - 5.0f;
		float cy = PLAYER_HEIGHT / 2.0f;
		float pushRbottom = pIBox->CollisionDown(transform_.position_.x + cx, transform_.position_.y + (cy - CORRECT_BOTTOM));
		float pushLbottom = pIBox->CollisionDown(transform_.position_.x - cx, transform_.position_.y + (cy - CORRECT_BOTTOM));
		float pushBottom = max(pushRbottom, pushLbottom);//2�̑����̂߂荞�݂̑傫���ق�
		if (pushBottom > 0.0f) {
			transform_.position_.y -= pushBottom - 1.0f;
			jumpSpeed = 0.0f;
			onGround = true;
			animType = 0;
		}
		float pushRtop = pIBox->CollisionUp(transform_.position_.x + cx, transform_.position_.y - (cy - CORRECT_TOP));
		float pushLtop = pIBox->CollisionUp(transform_.position_.x - cx, transform_.position_.y - (cy - CORRECT_TOP));
		float pushTop = max(pushRtop, pushLtop);//2�̓��̂߂荞�݂̑傫���ق�
		if (pushTop > 0.0f) {
			transform_.position_.y += pushTop - 1.0f;
			jumpSpeed = 0.0f;
		}
	}
	

	std::list<Enemy*> pEnemies = GetParent()->FindGameObjects<Enemy>();
	for (Enemy* pEnemy : pEnemies) {
		if (pEnemy->CollideRectToRect(transform_.position_.x, transform_.position_.y, PLAYER_WIDTH/2.0f,PLAYER_HEIGHT/2.0f)) {
			/*animType = 4;
			animFrame = 0;
			state = S_Cry;
			scene->StartDead();*/
			KillMe();
		}
	}

	//�����ŃJ�����ʒu�𒲐�
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		int x = (int)transform_.position_.x - cam->GetValue();
		if (x > 640) {
			x = 640;
			cam->SetValue((int)transform_.position_.x - x);//�J�����̒l���o���ɂ͏�̎����ڍ�����
		}
		else if(x<64){
			x = 64;
			cam->SetValue((int)transform_.position_.x - x);
		}
	}
	
}

void Player::Draw()
{
	int x = (int)transform_.position_.x;
	int y = (int)transform_.position_.y;
	Camera* cam = GetParent()->FindGameObject<Camera>();
	if (cam != nullptr) {
		x -= cam->GetValue();//�v���C���[�̈ʒu����J����������
	}
	//DrawRectGraph(x, y, animFrame * 64, animType * 64, 64, 64, hImage, TRUE);
	DrawRotaGraph(x, y, 1.0, 0, hWalkImage[animFrame], TRUE, !isRight);
	
	unsigned int Cr = GetColor(0, 0, 255);
	DrawCircle(10, 10, 10,Cr, isBallAlive);
}

void Player::SetPosition(int x, int y)
{
	transform_.position_.x = x;
	transform_.position_.y = y;
}

bool Player::IsTouchBall(XMFLOAT3 pos)
{
	float cx = PLAYER_WIDTH / 2.0f;
	float cy = PLAYER_HEIGHT / 2.0f;
	if ((pos.x <= transform_.position_.x + cx) && (pos.x >= transform_.position_.x - cx)) {
		if ((pos.y <= transform_.position_.y) && (pos.y >= transform_.position_.y - cy)) {
			return true;
		}
	}
	return false;
}
