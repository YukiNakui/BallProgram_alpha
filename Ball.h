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

	//�J��
	void Release() override;

	void SetPosition(int x, int y);
	void ThrowBall();
	void TossBall();
private:
	int hImage;
	float speed;
	float bound;
};

