#pragma once
#include "Engine/GameObject.h"
#include"Ball.h"

//namespace PLAYER_ANIMFRAME {
//	const int IDLE_MAXFRAME{ 6 };
//	const int WALK_MAXFRAME{ 6 };
//	const int TOSS_MAXFRAME{ 4 };
//	const int SPIKE_MAXFRAME{ 4 };
//	const int DEAD_MAXFRAME{ 5 };
//}
//namespace PAF = PLAYER_ANIMFRAME;

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
	
private:
	
	//int hWalkImage[PAF::WALK_MAXFRAME];
	int hAnimImage;
	int hBallImg;
	
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
	
	bool IsTouchBall(XMFLOAT3 pos);
	void KnockBack(XMFLOAT3& pos);//�_���[�W���̃m�b�N�o�b�N

};

