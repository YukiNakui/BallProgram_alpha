#pragma once
#include "Engine/GameObject.h"
#include"Ball.h"

namespace PLAYER_ANIMFRAME {
	const int WALK_MAXFRAME{ 6 };
	const int SPIKE_FRAME{ 4 };
}
namespace PAF = PLAYER_ANIMFRAME;

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
	int hImage;
	int hWalkImage[PAF::WALK_MAXFRAME];
	
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
	Ball* pBall;
	bool isBallAlive;
	int tossCount;
	
	bool IsTouchBall(XMFLOAT3 pos);

	void UpdateNormal();
	void UpdateToss();
	void UpdateSpike();
	void UpdateDamage();
	void UpdateDead();
};

