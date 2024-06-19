#pragma once
#include "Engine/GameObject.h"
class Ball :
    public GameObject
{
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	Ball(GameObject* parent);
	~Ball();

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	void SetPosition(float x,float y);
	void Spike(bool isRight);
	void FirstToss();
	void SecondToss();
	bool IsBallAlive();
	XMFLOAT3 GetPos();
	bool IsBallCatch(float x,float y);
	void SetIsAlive(bool isalive) { isAlive = isalive; }
	int GetBallSize();
private:
	int hImage;
	XMVECTOR moveVec;
	bool isAlive;
	float r;
	float speed;
	float accsel;
};

