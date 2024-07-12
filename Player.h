#pragma once
#include "Engine/GameObject.h"
#include"Ball.h"

class Player :
    public GameObject
{
public:
	Player(GameObject* scene);
	~Player();
	void Update() override;
	void Draw() override;

	/// <summary>
	/// �v���C���[�̍��W���Z�b�g����
	/// </summary>
	/// <param name="x">X���W�i���j</param>
	/// <param name="y">Y���W�i��j</param>
	void SetPosition(int x, int y);
	XMFLOAT3 GetPosition();
private:
	
	//int hWalkImage[PAF::WALK_MAXFRAME];
	int hAnimImage;
	int hBallImg;

	int hp;
	
	enum State {
		NORMAL = 0,
		TOSS,
		SPIKE,
		DAMAGE,
		DEAD,
	};
	State state;

	GameObject* sceneTop;
	bool prevSpaceKey;
	bool prevAttackKey;
	float moveSpeed;
	float jumpSpeed;
	bool onGround;

	int animType;//��
	int animFrame;//�R�}
	int frameCounter;
	
	bool isRight;
	bool canMove;

	Ball* pBall;

	bool isBallAlive;
	bool canSpike;
	int tossCount;
	float cdTimer;
	
	float damageTimer;
	bool nowDamage;
	bool blink;
	bool fallNow;
	
	bool IsTouchBall(XMFLOAT3 pos);
	

};

