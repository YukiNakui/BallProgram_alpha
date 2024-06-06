#pragma once
#include "Engine/GameObject.h"
class Field :
    public GameObject
{
public:
	Field(GameObject* scene);
	~Field();
	void Reset();
	void Update() override;
	void Draw() override;

	/// <summary>
	/// �E���̓_���������Ă��邩���ׂ�
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <returns>�߂荞�񂾗ʁi�h�b�g�j</returns>
	int CollisionRight(int x, int y);

	/// <summary>
	/// ���̓_���������Ă��邩���ׂ�
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <returns>�߂荞�񂾗ʁi�h�b�g�j</returns>
	int CollisionDown(int x, int y);
	int CollisionLeft(int x, int y);
	int CollisionUp(int x, int y);
private:
	int hImage;

	bool IsWallBlock(int x, int y);
	int* Map;
	int width;
	int height;

};

