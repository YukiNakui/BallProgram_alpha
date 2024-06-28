#pragma once
#include "Engine/GameObject.h"
#include"Ball.h"

namespace PLAYER_ANIMFRAME {
	const int IDLE_MAXFRAME{ 6 };
	const int WALK_MAXFRAME{ 6 };
	const int TOSS_MAXFRAME{ 4 };
	const int SPIKE_MAXFRAME{ 4 };
	const int DEAD_MAXFRAME{ 5 };
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
	enum AnimID {
		NORMAL = 0,
		TOSS,
		SPIKE,
		DAMAGE,
		DEAD,
		MAX
	};
	int hAnimData[MAX];
	int hIdleImage[PAF::IDLE_MAXFRAME];
	int hWalkImage[PAF::WALK_MAXFRAME];
	int hTossImage[PAF::TOSS_MAXFRAME];
	int hSpikeImage[PAF::SPIKE_MAXFRAME];
	int hDeadImage[PAF::DEAD_MAXFRAME];
	
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

