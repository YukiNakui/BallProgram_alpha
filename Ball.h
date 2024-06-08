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

	void SetPosition(XMFLOAT3 pos);
	void SpikeBall(bool isRight);
	void TossBall();
private:
	int hImage;
	XMVECTOR moveVec;
};

